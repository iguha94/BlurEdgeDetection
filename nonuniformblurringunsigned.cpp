
#include "CImg.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include "Global.h"
#include "Commons.h"
#include "ComputeGradient.h"
using namespace std;
using namespace cimg_library;


//initializing  gaussian smoothing.
void GaussianSmoothingKernel(int dim,double variance,bool isprint=false){
    kernelX=new double*[dim];
    for(int i=0;i<dim;i++){
        kernelX[i]=new  double[dim];
        for(int j=0;j<dim;j++){
            kernelX[i][j]=0;
        }
    }
    double totalweight=0;
    loopcnts=dim/2;
    for(int i=-loopcnts;i<=loopcnts;i++){
        for(int j=-loopcnts;j<=loopcnts;j++){
            double dist=CartesianDistance2D(0,0,i,j);
            kernelX[loopcnts+i][loopcnts+j]=gaussian(dist,0,variance);
            totalweight+=kernelX[loopcnts+i][loopcnts+j];
        }
    }
    if(isprint)
        cout<<"Totalweight: "<<totalweight<<"\n";
    for(int i=-loopcnts;i<=loopcnts;i++){
        for(int j=-loopcnts;j<=loopcnts;j++){
            kernelX[loopcnts+i][loopcnts+j]/=totalweight;
        }

    }

}

unsigned short gaussianSmoothing(unsigned short** arr, int x, int y, int dim){
    double GradX=0.0;
    for(int i=-loopcnts;i<=loopcnts;i++){
        for(int j=-loopcnts;j<=loopcnts;j++){
            if(x+i>=0&&y+j>=0&&x+i<Xdim&&y+j<Ydim){
                GradX=GradX+arr[x+i][y+j]*kernelX[i+loopcnts][j+loopcnts];
            }
        }
    }
//    /cout<<"Smooth intensity value: "<<GradX<<"\n";
    return (unsigned short)GradX;
}

void uniformBlurring(string displayimage, string writeimage){
    int window;
    int sigma;
    cout<<"Enter sigma: ";
    cin>>sigma;

    CImg<unsigned short> jpgimage(displayimage.c_str());
    Xdim=jpgimage.width();Ydim=jpgimage.height();
    cout<<Xdim<<": "<<Ydim<<"\n";
    CImg<unsigned short> smoothjpg(Xdim,Ydim,1,1);
    allocateMemory(Xdim,Ydim);
    readImage<unsigned short>(jpgimage,image);
    readImage<unsigned short>(jpgimage,gradImage);

    for(int i=0;i<Xdim;i++){
        for(int j=0;j<Ydim;j++){
            if(sigma%2==0) window=3*sigma+1;
            else window=sigma*3;
            GaussianSmoothingKernel(window,sigma,false);
            gradImage[i][j]=gaussianSmoothing(image,i,j,window);
        }
    }
    writeImage<unsigned short>(gradImage,Xdim,Ydim,smoothjpg,writeimage);
}

void blurComputationfromimage(string displayimage, string smoothimagepath, string writeImagename){
    int window;
    int x,y;
    double sigma;
    bool flag;
    //CImg<unsigned short> jpgimage(displayimage.c_str());
    CImg<unsigned short> smoothimage(smoothimagepath.c_str());
    //Xdim=jpgimage.width();Ydim=jpgimage.height();
    allocateMemory(Xdim,Ydim);
    readImagefile(displayimage,image,true);
    //readImage<unsigned short>(jpgimage,image);
    //readImage<unsigned short>(jpgimage,gradImage);
    readImage<int>(smoothimage,gradientImage);
    for(int i=0;i<Xdim;i++){
        for(int j=0;j<Ydim;j++){
            flag=false;
            double sigma=(double)((double)gradientImage[i][j]/10.0);
            //cout<<"Sigma: "<<sigma<<"\n";
            int sigma1=(sigma)+1;
            //if(sigma%2==0) window=8*sigma+1;
            //else window=sigma*8;
            window=6*sigma1+1;
            GaussianSmoothingKernel(window,sigma,flag);

            gradImage[i][j]=gaussianSmoothing(image,i,j,window);

        }
    }
    writeImagefile<unsigned short>(gradImage,Xdim,Ydim,writeImagename);
}

void blurComputationfromfile(string displayimage,string writeImagename,string filename){
    ifstream fin;
    fin.open(filename.c_str());
    int window;
    int x,y;
    double sigma;
    CImg<unsigned short> jpgimage(displayimage.c_str());
    Xdim=jpgimage.width();Ydim=jpgimage.height();

    CImg<unsigned short> smoothjpg(Xdim,Ydim,1,1);
    allocateMemory(Xdim,Ydim);
    readImage<unsigned short>(jpgimage,image);
    readImage<unsigned short>(jpgimage,gradImage);

    while(fin>>x>>y>>sigma){
        int sig=sigma;
        if(sig==0) sig+=1;
        if(sigma==0) sigma+=1;
        if(sigma<1) {window=3;}
        else if(sig%2==0) window=3*sig+1;
        else window=sig*3;
        GaussianSmoothingKernel(window,sigma);
        gradImage[x][y]=gaussianSmoothing(image,x,y,window);

    }
    writeImage<unsigned short>(gradImage,Xdim,Ydim,smoothjpg,writeImagename);
}

int main() {

    string displayimagefilename;
    string scaleimage;
    string smoothimagepath;
    cout<<"Display Image: ";
    cin>>displayimagefilename;
    cout<<"Enter the dimension of the image: ";
    cin>>Xdim>>Ydim;
    string displayimage=basepath+displayimagefilename;
    string writeImagename=basepath+"smooth-"+displayimagefilename;

    int selection;
    cout<<"1. read scale from image\n";
    cout<<"2. read scale from file\n";
    cout<<"3. uniformBlurring\n";
    cout<<"Choose an option.. ";
    cin>>selection;
    switch(selection){
    case 1:
        cout<<"Enter scale Image: ";
        cin>>scaleimage;
        smoothimagepath=basepath+scaleimage;
        blurComputationfromimage(displayimage,smoothimagepath,writeImagename);
        break;
    case 3:
        uniformBlurring(displayimage,writeImagename);
        break;
    case 2: default:
        cout<<"Enter scale file: ";
        cin>>scaleimage;
        smoothimagepath=basepath+scaleimage;
        blurComputationfromfile(displayimage,writeImagename,smoothimagepath);
    }

}
