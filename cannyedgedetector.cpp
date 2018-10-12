#include "CImg.h"
#include "Global.h"
#include "Commons.h"
#include "ComputeGradient.h"
#include "continuousscaleComputation.h"
#include "MathFunctions.h"
#include <iostream>
#include <string>
#include <time.h>

using namespace std;
using namespace cimg_library;


int main(int argc, char* argv[]) {

    string curimagefile(argv[1]);
    gaussgradvar=atof(argv[2]);
    colorgaussvar=atof(argv[3]);
    basepath=string(argv[4]);

    int kernelsize=6*gaussgradvar;
    if(kernelsize%2==0) kernelsize+=1;
    kernelsize=kernelsize>3?kernelsize:3;


    imagefilename=basepath+curimagefile;

    localizedimagename=basepath+"Canny-"+curimagefile;

    CImg<unsigned short> jpgimage(imagefilename.c_str());
    Xdim=jpgimage.width();
    Ydim=jpgimage.height();
    cout<<"Image Size: "<<Xdim<< " X "<<Ydim<<"\n";
    allocateMemory(Xdim,Ydim);
    allocateGradientImageMemory(Xdim,Ydim);
    initializeKernels(kernelsize);
    readImage<unsigned short>(jpgimage,image);

    compute_Gradient(image,Xdim,Ydim,false,false);
    int flag=1;
    double maxThreshold,minThreshold,threshold;

    cout<<"*********************************\n";
    cout<<"Localization using Canny's Method\n";
    cout<<"*********************************\n";
    while(flag==1){
        cout<<"Enter Lower and Higher Hysteresis Threshold: ";
        cin>>minThreshold>>maxThreshold;
        cout<<"Enter Non maxima suppression threshhold: ";
        cin>>threshold;
        NonmaximaSuppression_Canny<double,double>(gradientImage,GradientX,GradientY,Gradientangle,threshold);
        HysteresisThresholding(minThreshold,maxThreshold,localizedimagename);
        cout<<"Press 0 to terminate else press 1. \n";
        cin>>flag;
    }
    cout<<"Shutting Down Canny Edge and Starting Blur Edge Detection---\n";
    return 0;
}

