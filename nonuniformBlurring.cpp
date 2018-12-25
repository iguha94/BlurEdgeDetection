#include "CImg.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include "Global.h"
#include "Commons.h"
#include "ComputeGradient.h"
using namespace std;
using namespace cimg_library;

int loopcnt;

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
    loopcnt=dim/2;
    for(int i=-loopcnt;i<=loopcnt;i++){
        for(int j=-loopcnt;j<=loopcnt;j++){
            double dist=CartesianDistance2D(0,0,i,j);
            kernelX[loopcnt+i][loopcnt+j]=gaussian(dist,0,variance);
            totalweight+=kernelX[loopcnt+i][loopcnt+j];
        }
    }
    if(isprint)
        cout<<"Totalweight: "<<totalweight<<"\n";
    for(int i=-loopcnt;i<=loopcnt;i++){
        for(int j=-loopcnt;j<=loopcnt;j++){
            kernelX[loopcnt+i][loopcnt+j]/=totalweight;
        }

    }

}

unsigned short gaussianSmoothing(unsigned short** arr, int x, int y, int dim){
    double GradX=0.0;
    for(int i=-loopcnt;i<=loopcnt;i++){
        for(int j=-loopcnt;j<=loopcnt;j++){
            if(x+i>=0&&y+j>=0&&x+i<Xdim&&y+j<Ydim){
                GradX=GradX+arr[x+i][y+j]*kernelX[i+loopcnt][j+loopcnt];
            }
        }
    }
    return (unsigned short)GradX;
}

void Generate_ZuckersPhantom(string displayimage,string writeImagename){
    int window;
    int x,y;
    double sigma;
    bool flag;
    CImg<unsigned short> jpgimage(displayimage.c_str());
    Xdim=jpgimage.width();Ydim=jpgimage.height();
    CImg<unsigned short> smoothimage(Xdim,Ydim,1,1);
    allocateMemory(Xdim,Ydim);
    readImage<unsigned short>(jpgimage,image);
    readImage<double>(jpgimage,gradImage);

    for(int i=0;i<Xdim;i++){
        for(int j=0;j<Ydim;j++){
            image[i][j]=128;
            gradImage[i][j]=128;
        }
    }
    for(int i=Xdim/2;i<Xdim;i++){
        for(int j=0;j<Ydim;j++){
            image[i][j]=64;
            gradImage[i][j]=64;
        }
    }

    sigma=0;
    int div1=0;
    int div=0;
    int prevdiv=0;
    for(int j=0;j<Ydim;j++){
        div+=1;
        div1=div/10;
        if(div1!=prevdiv){
            prevdiv=div1;
            sigma+=1;
        }
        int sigma1=(sigma)+1;
        //if(sigma%2==0) window=8*sigma+1;
        //else window=sigma*8;
        window=6*sigma1+1;
        GaussianSmoothingKernel(window,sigma1,flag);

        for(int i=(Xdim/2)-sigma;i<=(Xdim/2)+sigma;i++)
            gradImage[i][j]=gaussianSmoothing(image,i,j,window);

    }
    writeImage<double,unsigned short>(gradImage,Xdim,Ydim,smoothimage,writeImagename,true);
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
    readImage<double>(jpgimage,gradImage);

    for(int i=0;i<Xdim;i++){
        for(int j=0;j<Ydim;j++){
            if(sigma%2==0) window=3*sigma+1;
            else window=sigma*3;
            GaussianSmoothingKernel(window,sigma,false);
            gradImage[i][j]=gaussianSmoothing(image,i,j,window);
        }
    }
    writeImage<double,unsigned short>(gradImage,Xdim,Ydim,smoothjpg,writeimage);
}

void blurComputationfromimage(string displayimage, string smoothimagepath, string writeImagename){
    int window;
    int x,y;
    double sigma;
    bool flag;
    CImg<unsigned short> jpgimage(displayimage.c_str());
    CImg<unsigned short> smoothimage(smoothimagepath.c_str());
    Xdim=jpgimage.width();Ydim=jpgimage.height();
    allocateMemory(Xdim,Ydim);
    readImage<unsigned short>(jpgimage,image);
    readImage<double>(jpgimage,gradImage);
    readImage<double>(smoothimage,gradientImage);
    for(int i=0;i<Xdim;i++){
        for(int j=0;j<Ydim;j++){
            flag=false;
            double sigma=(double)((double)gradientImage[i][j]/5.0);
            int sigma1=(sigma)+1;
            //if(sigma%2==0) window=8*sigma+1;
            //else window=sigma*8;
            window=6*sigma1+1;
            GaussianSmoothingKernel(window,sigma,flag);

            gradImage[i][j]=gaussianSmoothing(image,i,j,window);

        }
    }
    writeImage<double,unsigned short>(gradImage,Xdim,Ydim,smoothimage,writeImagename);
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
    readImage<double>(jpgimage,gradImage);

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
    writeImage<double,unsigned short>(gradImage,Xdim,Ydim,smoothjpg,writeImagename);
}

int main(int argc, char* argv[]) {

    string displayimagefilename;
    string scaleimage;
    string smoothimagepath;
    basepath=string(argv[1]);
    cout<<"Display Image: ";
    cin>>displayimagefilename;
    string displayimage=basepath+displayimagefilename;
    string writeImagename=basepath+"smooth-"+displayimagefilename;

    int selection;
    cout<<"1. read scale from image\n";
    cout<<"2. read scale from file\n";
    cout<<"3. uniformBlurring\n";
    cout<<"4. Zuckers Phantom\n";
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
    case 4:
        Generate_ZuckersPhantom(displayimage,writeImagename);
        break;
    case 2: default:
        cout<<"Enter scale file: ";
        cin>>scaleimage;
        smoothimagepath=basepath+scaleimage;
        blurComputationfromfile(displayimage,writeImagename,smoothimagepath);
    }
}

