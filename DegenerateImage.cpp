#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <string.h>
#include <cmath>
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkAdditiveGaussianNoiseImageFilter.h"

using namespace std;
using namespace itk;

string basepath="/local/vol00/scratch/Iguha/MTAPReview/TobeUsed/Shadow/Gray/";

const unsigned short Dimension = 2;
typedef unsigned char InputPixelType;
typedef unsigned char OutputPixelType;

short** AllocateMemory(int Xdim, int Ydim){
    short** arr = new short*[Xdim];
    for(int i=0;i<Xdim;i++){
        arr[i]=new short[Ydim];
        for(int j=0;j<Ydim;j++){
            arr[i][j]=0;
        }
    }
    return arr;
}

double  gaussian(double intensity,double mean, double var){
    double power=((intensity-mean)*(intensity-mean))/(2*var*var);
    double gaussval=exp(-power);
    double denominator=1/(2*3.14*var*var);
    return gaussval*denominator;
}

double GaussianSmoothing(short** arr, int x, int y,int dim,double variance,int Xdim, int Ydim){
    double** kernelX=new double*[dim];
    for(int i=0;i<dim;i++){
        kernelX[i]=new  double[dim];
        for(int j=0;j<dim;j++){
            kernelX[i][j]=0;
        }
    }
    double totalweight=0;
    int loopcnt=dim/2;
    for(int i=-loopcnt;i<=loopcnt;i++){
        for(int j=-loopcnt;j<=loopcnt;j++){
            double dist=sqrt(i*i+j*j);
            kernelX[loopcnt+i][loopcnt+j]=gaussian(dist,0,variance);
            totalweight+=kernelX[loopcnt+i][loopcnt+j];
        }
    }
    
    for(int i=-loopcnt;i<=loopcnt;i++){
        for(int j=-loopcnt;j<=loopcnt;j++){
            kernelX[loopcnt+i][loopcnt+j]/=totalweight;
        }

    }

    double GradX=0.0;
    for(int i=-loopcnt;i<=loopcnt;i++){
        for(int j=-loopcnt;j<=loopcnt;j++){
            if(x+i>=0&&y+j>=0&&x+i<Xdim&&y+j<Ydim){
                GradX=GradX+((double)arr[x+i][y+j])*kernelX[i+loopcnt][j+loopcnt];
            }
        }
    }

    return GradX;

}

int main(int argc, char* argv[]){
    typedef itk::Image<InputPixelType, Dimension> InputImageType;
    typedef itk::ImageFileReader<InputImageType> ReaderType;
    typedef itk::ImageFileWriter<InputImageType> WriterType;

    ReaderType::Pointer reader1 = ReaderType::New();
    ReaderType::Pointer reader2 = ReaderType::New();
    WriterType::Pointer writer1 = WriterType::New();
    WriterType::Pointer writer2 = WriterType::New();
    
    fstream fin;
    fin.open(argv[1],ios::in);
    string ID;
    while(getline(fin,ID)){
        string directory=basepath+"Image"+ID+"/";
        string TrueImage=directory+"Image"+ID+".png";
        string Blurmap=directory+"Smoothblurmap.png";
        string Noisyimg=directory+"NoisyImg.png";
        string Degenimg=directory+"Image"+ID+"_deg.png";

        reader1->SetFileName(TrueImage.c_str());
        reader1->Update();
        InputImageType::Pointer trueimage=reader1->GetOutput(); 

        reader2->SetFileName(Blurmap.c_str());
        reader2->Update();
        InputImageType::Pointer blurmapimage=reader2->GetOutput();

        InputImageType::RegionType region=trueimage->GetLargestPossibleRegion();
        InputImageType::SizeType size=region.GetSize();
        InputImageType::IndexType index;

        typedef itk::AdditiveGaussianNoiseImageFilter<InputImageType, InputImageType> FilterType;
        FilterType::Pointer filter = FilterType::New();
        filter->SetInput(trueimage);
        filter->SetMean(0);
        filter->SetStandardDeviation(10);
        filter->Update();
        InputImageType::Pointer noisyimage=filter->GetOutput();
        writer1->SetFileName(Noisyimg);
        writer1->SetInput(filter->GetOutput());
        writer1->Update();

        short** imgarr=AllocateMemory(size[0],size[1]);
        short** blurmaparr=AllocateMemory(size[0],size[1]);
        short** blurimgarr=AllocateMemory(size[0],size[1]);
        short** noisyarr=AllocateMemory(size[0],size[1]);

        for(int i=0;i<size[0];i++){
            for(int j=0;j<size[1];j++){
                index[0]=i;index[1]=j;
                imgarr[i][j]=trueimage->GetPixel(index);
                blurmaparr[i][j]=blurmapimage->GetPixel(index);
                noisyarr[i][j]=noisyimage->GetPixel(index);
            }
        }

        InputImageType::IndexType start;
        start[0] = 0; 
        start[1] = 0; 

        InputImageType::RegionType region1;
        region1.SetSize(size);
        region1.SetIndex(start);

        InputImageType::Pointer oimage = InputImageType::New();
        oimage->SetRegions(region1);
        oimage->Allocate();

        for(int i=0;i<size[0];i++){
            for(int j=0;j<size[1];j++){
                double sigma=blurmaparr[i][j]/15;
                if(sigma<1) sigma=1;
                int sigma1=int(sigma+1.0);
                int window=6*sigma1+1;
                index[0]=i;index[1]=j;
                unsigned char value=(unsigned char) GaussianSmoothing(noisyarr,i,j,window,sigma,size[0],size[1]);
                oimage->SetPixel(index,value);
            }
        }

        writer2->SetFileName(Degenimg);
        writer2->SetInput(oimage);
        writer2->Update();
    }
}

    

    
