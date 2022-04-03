#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkPointSet.h"
#include "itkBSplineScatteredDataPointSetToImageFilter.h"
#include "itkPNGImageIO.h"
#include "itkJPEGImageIO.h"
#include "itkTIFFImageIO.h"
#include <string.h>
#include <iostream>
#include <fstream>
using namespace std;
/**
 * In this test, we approximate a 2-D scalar field.
 * The scattered data is derived from a segmented
 * image.  We write the output to an image for
 * comparison.
 */
int itkBSplineScatteredDataPointSetToImageFilterTest1(string inputimage, string outputimage, string outputfilename)
{
    const unsigned int ParametricDimension = 2;
    const unsigned int DataDimension = 1;
    typedef float RealType;
    typedef unsigned char PixelType;
    typedef  itk::PNGImageIO pngiotype;
    typedef  itk::JPEGImageIO jpgiotype;
    typedef  itk::TIFFImageIO TIFFIOType;
    ofstream fout;
    fout.open(outputfilename.c_str());

    TIFFIOType::Pointer tiffIO = TIFFIOType::New();
    pngiotype::Pointer pngio = pngiotype::New();
    jpgiotype::Pointer jpgio = jpgiotype::New();

    typedef itk::Image<PixelType, ParametricDimension> InputImageType;
    typedef itk::Vector<RealType, DataDimension> VectorType;
    typedef itk::Image<VectorType, ParametricDimension> VectorImageType;

    typedef itk::PointSet<VectorImageType::PixelType, ParametricDimension> PointSetType;
    PointSetType::Pointer pointSet = PointSetType::New();

    typedef itk::ImageFileReader<InputImageType> ReaderType;
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetImageIO(pngio);
    reader->SetFileName(inputimage.c_str() );
    reader->Update();

    itk::ImageRegionIteratorWithIndex<InputImageType>It( reader->GetOutput(), reader->GetOutput()->GetLargestPossibleRegion() );

    // Iterate through the input image which consists of multivalued
    // foreground pixels (=nonzero) and background values (=zero).
    // The foreground pixels comprise the input point set.

    for ( It.GoToBegin(); !It.IsAtEnd(); ++It )
    {
        if ( It.Get() != itk::NumericTraits<PixelType>::Zero )
        {
            // We extract both the 2-D location of the point
            // and the pixel value of that point.

            PointSetType::PointType point;
            reader->GetOutput()->TransformIndexToPhysicalPoint( It.GetIndex(), point );

            unsigned long i = pointSet->GetNumberOfPoints();
            pointSet->SetPoint( i, point );

            PointSetType::PixelType V( DataDimension );
            V[0] = static_cast<RealType>( It.Get() );
            pointSet->SetPointData( i, V );
        }
    }


    // Instantiate the B-spline filter and set the desired parameters.
    typedef itk::BSplineScatteredDataPointSetToImageFilter <PointSetType, VectorImageType> FilterType;
    FilterType::Pointer filter = FilterType::New();
    filter->SetSplineOrder( 3 );
    FilterType::ArrayType ncps;
    ncps.Fill( 4 );
    filter->SetNumberOfControlPoints( ncps );
    filter->SetNumberOfLevels( 3 );

    // Define the parametric domain.
    filter->SetOrigin( reader->GetOutput()->GetOrigin() );
    filter->SetSpacing( reader->GetOutput()->GetSpacing() );
    filter->SetSize( reader->GetOutput()->GetLargestPossibleRegion().GetSize() );

    filter->SetInput( pointSet );

    try
    {
        filter->Update();
    }
    catch (...)
    {
        std::cerr << "Test 1: itkBSplineScatteredDataImageFilter exception thrown"
                  << std::endl;
        return EXIT_FAILURE;
    }

    // Write the output to an image.
    typedef itk::Image<PixelType, ParametricDimension> RealImageType; //changed to pixel type to store as png or jpeg image
    RealImageType::Pointer image = RealImageType::New();
    image->SetRegions( reader->GetOutput()->GetLargestPossibleRegion() );
    image->Allocate();
    itk::ImageRegionIteratorWithIndex<RealImageType> Itt( image, image->GetLargestPossibleRegion() );

    for ( Itt.GoToBegin(); !Itt.IsAtEnd(); ++Itt )
    {
        fout<<Itt.GetIndex()[0]<<" "<<Itt.GetIndex()[1]<<" "<<filter->GetOutput()->GetPixel( Itt.GetIndex() )[0]<<"\n";
        Itt.Set( filter->GetOutput()->GetPixel( Itt.GetIndex() )[0]*10 );
    }

    typedef itk::ImageFileWriter<RealImageType> WriterType;
    WriterType::Pointer writer = WriterType::New();
    writer->SetImageIO(pngio);
    writer->SetInput( image );
    writer->SetFileName( outputimage.c_str() );
    writer->Update();
    fout.close();
    return EXIT_SUCCESS;
}

int main( )
{
    string inputimage,outputimage;
    cout<<"Enter Input Image: ";
    cin>>inputimage;
    cout<<"Output Image: ";
    cin>>outputimage;
    string basepath="/nfs/s-l011/local/vol01/i/iguha/Documents/Phd-iguha/Imagesfortesting/ECCV/QA/";
    inputimage=basepath+inputimage;
    outputimage=basepath+outputimage;
    string outputfilename=basepath+"scaleList.txt";
    bool test1 = itkBSplineScatteredDataPointSetToImageFilterTest1( inputimage,outputimage,outputfilename );

    return ( test1  );
}

