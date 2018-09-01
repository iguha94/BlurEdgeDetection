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

    string logfile=basepath+"log.txt";
    logger.open(logfile.c_str());
    //confidence=1.5*pow(10,-8);
    //memconf=confidence;
    bool b=false;
    gaussvar=1;
    int kernelsize=3;
    sampling_rate=6;

    string curimagefile(argv[1]);
    gaussgradvar=atof(argv[2]);
    colorgaussvar=atof(argv[3]);
    maxscale=atof(argv[4]);
    snr=atof(argv[5]);

    kernelsize=6*gaussgradvar;
    if(kernelsize%2==0) kernelsize+=1;
    kernelsize=kernelsize>3?kernelsize:3;

    //cout<<"Enter membership value: ";
    //cin>>memconf;

    //cout<<"Enter the pixel of interest: ";
    //cin>>Xc>>Yc;

    //cout<<"Interrmediate Scale: ";
    //cin>>intermediatescale;

    string sfname=basepath+"gradMap.txt";
    sout.open(sfname.c_str());

    no_of_interpolating_points=3; //denotes the number of samples taken for each point, r,r-0.25,r+0.25

    imagefilename=basepath+curimagefile;
    scaleimagename=basepath+"scale-"+curimagefile;
    gradientimagename=basepath+"scale-gradient-"+curimagefile;
    gaussgradientimagename=basepath+"point-gradient-"+curimagefile;
    sordergradientimagename=basepath+"sogradient-"+curimagefile;
    sorderscaleimagename=basepath+"soscale-"+curimagefile;
    gradientcolorimagename=basepath+"scale-gradientcolor-"+curimagefile;
    gausscolorimagename=basepath+"point-gradient-color-"+curimagefile;
    localmaximaimagename=basepath+"localmaxima-"+curimagefile;
    Hysteresisimagename=basepath+"hysteresis-"+curimagefile;
    secondderivativeimagename=basepath+"sd-"+curimagefile;
    localizedimagename=basepath+"localized-"+curimagefile;
    dialatedimagename=basepath+"dialated-"+curimagefile;
    dividorimagename=basepath+"dividor-"+curimagefile;

    CImg<unsigned short> jpgimage(imagefilename.c_str());
    Xdim=jpgimage.width();
    Ydim=jpgimage.height();
    cout<<"Image Size: "<<Xdim<< " X "<<Ydim<<"\n";
    allocateMemory(Xdim,Ydim);
    allocateGradientImageMemory(Xdim,Ydim);
    initializeKernels(kernelsize);
    readImage<unsigned short>(jpgimage,image);

    noisestdv=snr*snr;
    gradienthresh=6;

    compute_Gradient(image,Xdim,Ydim);
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
        //NonmaximaSuppression<double,double>(gradImage,Gradientangle,threshold);
        NonmaximaSuppression_Canny<double,double>(gradientImage,GradientX,GradientY,Gradientangle,threshold);
        HysteresisThresholding(minThreshold,maxThreshold,localizedimagename);
        cout<<"Press 0 to terminate else press 1. \n";
        cin>>flag;
    }
    cout<<"Shutting Down Canny Edge and Starting Blur Edge Detection---\n";
    compute_scale_Gradient_with_continuous_interpolation(image,Xdim,Ydim,b);
    sout.close();
    logger.close();

    return 0;
}

