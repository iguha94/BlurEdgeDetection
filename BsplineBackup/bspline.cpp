///*=========================================================================

//  Program:   Insight Segmentation & Registration Toolkit
//  Module:    $RCSfile: itkGaussianInterpolateImageFunctionTest.cxx,v $
//  Language:  C++
//  Date:      $Date: $
//  Version:   $Revision: $

//  Copyright (c) Insight Software Consortium. All rights reserved.
//  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

//     This software is distributed WITHOUT ANY WARRANTY; without even
//     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//     PURPOSE.  See the above copyright notices for more information.

//=========================================================================*/
//#include "itkGaussianInterpolateImageFunction.h"
//#include "itkBSplineInterpolateImageFunction.h"
//#include "itkIdentityTransform.h"
//#include "itkImage.h"
//#include "itkImageFileReader.h"
//#include "itkImageFileWriter.h"
//#include "itkLinearInterpolateImageFunction.h"
//#include "itkNearestNeighborInterpolateImageFunction.h"
//#include "itkResampleImageFilter.h"
//#include "itkTimeProbe.h"
//#include "itkJPEGImageIO.h"
//#include "itkPNGImageIO.h"
//#include "itkContinuousIndex.h"
//#include <stdio.h>
//#include <string>
//#include <vector>
//#include <string>
//#include <iostream>
//#include <fstream>
//#include <itkIndex.h>
//#include "itkCastImageFilter.h"
//using namespace std;

//template<class TValue>
//TValue Convert( std::string optionString )
//{
//    TValue value;
//    std::istringstream iss( optionString );
//    iss >> value;
//    return value;
//}

//template<class TValue>
//std::vector<TValue> ConvertVector( std::string optionString )
//{
//    std::vector<TValue> values;
//    std::string::size_type crosspos = optionString.find( 'x', 0 );

//    if ( crosspos == std::string::npos )
//    {
//        values.push_back( Convert<TValue>( optionString ) );
//    }
//    else
//    {
//        std::string element = optionString.substr( 0, crosspos ) ;
//        TValue value;
//        std::istringstream iss( element );
//        iss >> value;
//        values.push_back( value );
//        while ( crosspos != std::string::npos )
//        {
//            std::string::size_type crossposfrom = crosspos;
//            crosspos = optionString.find( 'x', crossposfrom + 1 );
//            if ( crosspos == std::string::npos )
//            {
//                element = optionString.substr( crossposfrom + 1, optionString.length() );
//            }
//            else
//            {
//                element = optionString.substr( crossposfrom + 1, crosspos ) ;
//            }
//            std::istringstream iss( element );
//            iss >> value;
//            values.push_back( value );
//        }
//    }
//    return values;
//}

//template <unsigned int ImageDimension>
//int ResampleImage(string inputimage, string outputimage,string pointfile,int spsize,int interpolatorno )
//{
//    fstream fin;
//    fin.open(pointfile.c_str());

//    typedef double RealType;
//    typedef unsigned char PixelType;
//    typedef itk::Image<PixelType, ImageDimension> ImageType;
//    typedef itk::Image<RealType, ImageDimension> realImageType;

//    typedef  itk::JPEGImageIO jpegiotype;
//    jpegiotype::Pointer jpgio = jpegiotype::New();

//    typedef  itk::PNGImageIO pngiotype;
//    pngiotype::Pointer pngio = pngiotype::New();


//    jpgio->SetPixelType(itk::ImageIOBase::SCALAR);
//    typedef itk::ImageFileReader<ImageType> ReaderType;
//    typename ReaderType::Pointer reader = ReaderType::New();
//    reader->SetImageIO(jpgio);
//    reader->SetFileName( inputimage.c_str() );
//    reader->Update();

//    cout<<"Dimension: "<<reader->GetOutput()->GetLargestPossibleRegion();

//    typedef itk::IdentityTransform<RealType, ImageDimension> TransformType;
//    typename TransformType::Pointer transform = TransformType::New();
//    transform->SetIdentity();

//    typedef itk::LinearInterpolateImageFunction<ImageType, RealType>LinearInterpolatorType;
//    typename LinearInterpolatorType::Pointer interpolator= LinearInterpolatorType::New();
//    interpolator->SetInputImage( reader->GetOutput() );

//    typedef itk::GaussianInterpolateImageFunction<ImageType, RealType>GaussianInterpolatorType;
//    typename GaussianInterpolatorType::Pointer g_interpolator= GaussianInterpolatorType::New();
//    g_interpolator->SetInputImage( reader->GetOutput() );

//    typedef itk::BSplineInterpolateImageFunction< ImageType, RealType >  InterpolatorType;
//    typename InterpolatorType::Pointer bsplineinterpolator = InterpolatorType::New();
//    bsplineinterpolator->SetInputImage(reader->GetOutput());
//    bsplineinterpolator->SetSplineOrder(3);
//    double sigma[ImageDimension];
//    for( unsigned int d = 0; d < ImageDimension; d++ )
//    {
//        sigma[d] = 1;
//    }
//    double alpha = 3;

//    g_interpolator->SetParameters( sigma, alpha );
//    bsplineinterpolator->SetSplineOrder(2);

//    typedef itk::ResampleImageFilter<ImageType, ImageType, RealType> ResamplerType;
//    typename ResamplerType::Pointer resampler = ResamplerType::New();
//    typename ResamplerType::SpacingType spacing;
//    typename ResamplerType::SizeType size;

//    //std::vector<RealType> sp = ConvertVector<RealType>( std::string( arg4 ) );
//    spacing.Fill( 1 );

//    for ( unsigned int i = 0; i < ImageDimension; i++ )
//    {
//        RealType spacing_old = reader->GetOutput()->GetSpacing()[i];
//        RealType size_old = reader->GetOutput()->GetLargestPossibleRegion().GetSize()[i];
//        size[i] = static_cast<int>( ( spacing_old * size_old ) / spacing[i] + 0.5 );
//        cout<<"Size i: "<<size[i]<<"\n";
//    }


//    resampler->SetTransform( transform );
//    resampler->SetInterpolator( g_interpolator );
//    //resampler->SetInterpolator( bsplineinterpolator );
//    resampler->SetInterpolator(interpolator);

//    resampler->SetInput( reader->GetOutput() );
//    resampler->SetOutputSpacing( spacing );
//    resampler->SetOutputOrigin( reader->GetOutput()->GetOrigin() );
//    resampler->SetSize( size );
//    resampler->SetNumberOfThreads( 1 );
//    resampler->SetOutputDirection( reader->GetOutput()->GetDirection() );

//    itk::TimeProbe timer;
//    timer.Start();
//    try
//    {
//        resampler->Update();
//    }
//    catch( itk::ExceptionObject & err)
//    {
//        std::cerr << "ExceptionObject caught !" << std::endl;
//        std::cerr << err << std::endl;
//        return EXIT_FAILURE;
//    }
//    timer.Stop();

//    std::cout << "Elapsed time: " << timer.GetMeanTime() << std::endl;

//    pngio->SetPixelType(itk::ImageIOBase::SCALAR);
//    typedef itk::ImageFileWriter<ImageType> WriterType;
//    typename WriterType::Pointer writer = WriterType::New();
//    writer->SetImageIO(jpgio);
//    writer->SetFileName( outputimage );
//    writer->SetInput( resampler->GetOutput() );
//    writer->Update();

//    return 0;
//}


//int main( int argc, char *argv[] )
//{

//    char* arr=new char[10];
//    string basepath="/user/i/iguha/Documents/Phd-iguha/Imagesfortesting/ECCV/QA/";
//    string imagename;
//    cout<<"Enter ImageName: ";
//    cin>>imagename;
//    string imagefile=basepath+imagename;
//    string blurimagefile=basepath+"blur.jpg";
//    string pointfile=basepath+"blurpoints.txt";

//    int c=2;
////    a[0]=1;
//    switch(c)
//    {
//    case 2:
//        ResampleImage<2>( imagefile,  blurimagefile, pointfile, 2, 2);
//        break;
//    case 3:
//        ResampleImage<2>(imagefile,  blurimagefile,pointfile, 2, 2);
//        break;
//    default:
//        std::cerr << "Unsupported dimension" << std::endl;
//        exit( EXIT_FAILURE );
//    }
//}

