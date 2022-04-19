#include <iostream>
#include <fstream>
#include <math.h>
#include <string.h>
#include <queue>
#include <cmath>
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCannyEdgeDetectionImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"

using namespace itk;
using namespace std;

string basepath="/local/vol00/scratch/Iguha/MTAPReview/BlurScaleImages/ImagePhantoms/";

char* SNR[3]={"6","12","18"};

const unsigned short Dimension = 2;
typedef unsigned char InputPixelType;
typedef unsigned char OutputPixelType;

class Point2D{
    public:
    int x;
    int y;
    Point2D(int x, int y){
        this->x=x;
        this->y=y;
    }
};

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

double GetEditDistance(short** testarr, short** refarr, short** visited, int x, int y, int Xdim, int Ydim){

    for(int i=0;i<Xdim;i++){
        for(int j=0;j<Ydim;j++){
            visited[i][j]=0;
        }
    }

    queue<Point2D> Q;
    visited[x][y]=1;
    if(refarr[x][y]>0) return 0;

    for(int i=-1;i<=1;i++){
        for(int j=-1;j<=1;j++){
            if(i==0 && j==0) continue;
            if(x+i>=2 && x+i<Xdim-2 && y+j>=2 && y+j<Ydim-2){
                Q.push(Point2D(x+i,y+j));
                visited[x+i][y+j]=1;
            }
        }
    }

    int flag=0;
    double sdist=10000000;

    while(!Q.empty()){
        Point2D p =Q.front();
        Q.pop();
        int x1=p.x; int y1=p.y;
        if(refarr[x1][y1]>0){
            double curdist=(x-x1)*(x-x1)+(y-y1)*(y-y1);
            if(curdist<sdist) sdist=curdist;
            flag=1;
        }

        if(flag==0){
            for(int i=-1;i<=1;i++){
                for(int j=-1;j<=1;j++){
                    if(i==0 && j==0) continue;
                if(x1+i>=2 && x1+i<Xdim-2 && y1+j>=2 && y1+j<Ydim-2 && visited[x1+i][y1+j]==0){
                    visited[x1+i][y1+j]=1;
                    Q.push(Point2D(x1+i,y1+j));
                    }
                }
            }
        }
    }

    return sdist;
}

double getSymmetricError(string refedgeimage, string testedgeimage){
    typedef itk::Image<InputPixelType, Dimension> InputImageType;
    typedef itk::ImageFileReader<InputImageType> ReaderType;

    ReaderType::Pointer reader1 = ReaderType::New();
    reader1->SetFileName(refedgeimage.c_str());
    reader1->Update();
    InputImageType::Pointer refedge=reader1->GetOutput();

    InputImageType::RegionType region=refedge->GetLargestPossibleRegion();
    InputImageType::SizeType size=region.GetSize();
    InputImageType::IndexType index;

    ReaderType::Pointer reader2 = ReaderType::New();
    reader2->SetFileName(testedgeimage.c_str());
    reader2->Update();
    InputImageType::Pointer testedge=reader2->GetOutput();

    short** refarr=AllocateMemory(size[0],size[1]);
    short** testarr=AllocateMemory(size[0],size[1]);

    for(int i=0;i<size[0];i++){
        for(int j=0;j<size[1];j++){
            index[0]=i;
            index[1]=j;
            refarr[i][j]=(short)refedge->GetPixel(index);
            testarr[i][j]=(short)testedge->GetPixel(index);
        }
    }

    short** visited = AllocateMemory(size[0],size[1]);

    double d_G_t=0;
    double d_c_t=0;

    double non_zero_cnt=0;

    for(int i=2;i<size[0]-2;i++){
        for (int j=2;j<size[1]-2;j++){
            if(testarr[i][j]>0){
                double dist=GetEditDistance(testarr,refarr,visited,i,j,size[0],size[1]);
                //d_G_t=d_G_t+sqrt(dist);
                d_G_t=d_G_t+dist;
                non_zero_cnt=non_zero_cnt+1;
            }
        }
    }

    //double non_zero_cnt=0;

    for(int i=2;i<size[0]-2;i++){
        for (int j=2;j<size[1]-2;j++){
            if(refarr[i][j]>0){
                double dist=GetEditDistance(refarr,testarr,visited,i,j,size[0],size[1]);
                //d_c_t=d_c_t+sqrt(dist);
                d_c_t=d_c_t+dist;
                non_zero_cnt=non_zero_cnt+1;
            }
        }
    }

    double FP=0;
    double FN=0;
    double countGT=0;
    for(int i=2;i<size[0]-2;i++){
        for(int j=2;j<size[1]-2;j++){
            if(refarr[i][j]==0 && testarr[i][j]>0) FP++;
            if(refarr[i][j]>0 && testarr[i][j]==0) FN++;
            if(refarr[i][j]>0) countGT+=1.0;
        }
    }
    //cout<<"d_G_t: "<<d_G_t<<" Weight: "<<(FP+FN)/countGT<<endl;

    double sum =d_c_t + d_G_t;
    //double error=((FP+FN)/(countGT*countGT))*sqrt(sum/non_zero_cnt);
    double error=((FP+FN)/(countGT*countGT))*sqrt(sum);

    return error;
}

double getmagnierError(string refedgeimage, string testedgeimage){
    typedef itk::Image<InputPixelType, Dimension> InputImageType;
    typedef itk::ImageFileReader<InputImageType> ReaderType;

    ReaderType::Pointer reader1 = ReaderType::New();
    reader1->SetFileName(refedgeimage.c_str());
    reader1->Update();
    InputImageType::Pointer refedge=reader1->GetOutput();

    InputImageType::RegionType region=refedge->GetLargestPossibleRegion();
    InputImageType::SizeType size=region.GetSize();
    InputImageType::IndexType index;

    ReaderType::Pointer reader2 = ReaderType::New();
    reader2->SetFileName(testedgeimage.c_str());
    reader2->Update();
    InputImageType::Pointer testedge=reader2->GetOutput();

    short** refarr=AllocateMemory(size[0],size[1]);
    short** testarr=AllocateMemory(size[0],size[1]);

    for(int i=0;i<size[0];i++){
        for(int j=0;j<size[1];j++){
            index[0]=i;
            index[1]=j;
            refarr[i][j]=(short)refedge->GetPixel(index);
            testarr[i][j]=(short)testedge->GetPixel(index);
        }
    }

    short** visited = AllocateMemory(size[0],size[1]);

    double d_G_t=0;
    double non_zero_cnt=0;

    for(int i=2;i<size[0]-2;i++){
        for (int j=2;j<size[1]-2;j++){
            if(testarr[i][j]>0){
                //cout<<"Test Pixel val: "<<testarr[i][j]<<" ";
                double dist=GetEditDistance(testarr,refarr,visited,i,j,size[0],size[1]);
                d_G_t=d_G_t+dist;
                non_zero_cnt=non_zero_cnt+1;
            }
        }
    }
    d_G_t=sqrt(d_G_t);

    double FP=0;
    double FN=0;
    double countGT=0;

    for(int i=2;i<size[0]-2;i++){
        for(int j=2;j<size[1]-2;j++){
            if(refarr[i][j]==0 && testarr[i][j]>0) FP++;
            if(refarr[i][j]>0 && testarr[i][j]==0) FN++;
            if(refarr[i][j]>0) countGT+=1.0;
        }
    }
    //cout<<"d_G_t: "<<d_G_t<<" Weight: "<<(FP+FN)/countGT<<endl;
    double error=d_G_t*((FP+FN)/(countGT*countGT));
    return error;
}


void GetCannyEdges(string refimage, string outimage, double variance, double lowerThreshold, double upperThreshold){
    const char * inputImage = refimage.c_str();
    const char * outputImage = outimage.c_str();
    typedef itk::Image<double, Dimension> InputImageType;
    typedef itk::Image<OutputPixelType, Dimension> OutputImageType;

    typedef itk::ImageFileReader<InputImageType> ReaderType;
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(inputImage);

    typedef itk::CannyEdgeDetectionImageFilter<InputImageType, InputImageType> FilterType;

    FilterType::Pointer filter = FilterType::New();
    filter->SetInput(reader->GetOutput());
    filter->SetVariance(variance);
    filter->SetLowerThreshold(lowerThreshold);
    filter->SetUpperThreshold(upperThreshold);

    typedef itk::RescaleIntensityImageFilter<InputImageType, OutputImageType> RescaleType;
    RescaleType::Pointer rescaler = RescaleType::New();
    rescaler->SetInput(filter->GetOutput());
    rescaler->SetOutputMinimum(0);
    rescaler->SetOutputMaximum(255);

    typedef itk::ImageFileWriter<OutputImageType> WriterType;
    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(outputImage);
    writer->SetInput(rescaler->GetOutput());

    writer->Update();
}

int main(int argc, char* argv[]){

    if(argc!=3){
        cout<<"USAGE: ./ErrorPhantom <LowerThreshold (5)> <UpperThreshold (10)>\n";
        return 0;
    }
    string Magerrorfile=basepath+"MagnierError.txt";
    string Symerrorfile=basepath+"SymError.txt";

    fstream fout1;
    fout1.open(Magerrorfile.c_str(),ios::out);
    fout1<<"Phantom ID,SNR,Scale_Algo_Error,Canny_Error, EZ_Error\n";

    fstream fout2;
    fout2.open(Symerrorfile.c_str(),ios::out);
    fout2<<"Phantom ID,SNR,Scale_Algo_Error,Canny_Error, EZ_Error\n";

    
    for(int i=1;i<=15;i++){
        stringstream ss;
        ss << i;
        string folder=basepath+"Phantom"+ss.str()+"/";
        string refimage=folder+"Phantom"+ss.str()+".jpg";
        string refedgeimage=folder+"Phantom"+ss.str()+"_edge.jpg";

        cout<<"Processing: "<<"Phantom"+ss.str()+".jpg"<<"\n";
        
        double variance = 1;
        double lowerThreshold = atof(argv[1]);
        double upperThreshold = atof(argv[2]);

        //GetCannyEdges(refimage,refedgeimage,variance,lowerThreshold,upperThreshold);

        for(int j=0;j<3;j++){
            string line="Phantom"+ss.str()+","+"SNR"+SNR[j]+",";
            string line2="Phantom"+ss.str()+","+"SNR"+SNR[j]+",";
            string SNRFolder=folder+"SNR"+SNR[j]+"/";
            string Scaleedge=SNRFolder+"scalebasededge_SNR-"+SNR[j]+"-smooth-Phantom"+ss.str()+".tif";
            string EZedge=SNRFolder+"EZ_SNR-"+SNR[j]+"-smooth-Phantom"+ss.str()+".jpg";
            string Cannyedge=SNRFolder+"Canny-SNR-"+SNR[j]+"-smooth-Phantom"+ss.str()+".tif";
            double err0=0; double err1=0; double err2=0; double err3=0;

            //err0= getmagnierError(refedgeimage,refedgeimage);
            
            err1= getmagnierError(refedgeimage,Scaleedge);
            err2= getmagnierError(refedgeimage,Cannyedge);
            err3= getmagnierError(refedgeimage,EZedge);

            stringstream ss1;
            ss1<<err1;
            line=line+ss1.str()+",";

            stringstream ss2;
            ss2<<err2;
            line=line+ss2.str()+",";

            stringstream ss3;
            ss3<<err3;
            line=line+ss3.str()+"\n";

            fout1<<line;

            // double err4= getSymmetricError(refedgeimage,Scaleedge);
            // double err5= getSymmetricError(refedgeimage,Cannyedge);
            // double err6= getSymmetricError(refedgeimage,EZedge);

            // stringstream ss4;
            // ss4<<err4;
            // line2=line2+ss4.str()+",";

            // stringstream ss5;
            // ss5<<err5;
            // line2=line2+ss5.str()+",";

            // stringstream ss6;
            // ss6<<err6;
            // line2=line2+ss6.str()+"\n";

            // fout2<<line2;

        }

        
    }

    fout1.close();
    fout2.close();
}