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

    bool b=false;
    gaussvar=1;
    int kernelsize=3;
    sampling_rate=6;

    string curimagefile(argv[1]);
    gaussgradvar=atof(argv[2]);
    colorgaussvar=atof(argv[3]);
    maxscale=atof(argv[4]);
    snr=atof(argv[5]);
    basepath=string(argv[6]);
    Xc=atoi(argv[7]);
    Yc=atoi(argv[8]);

    basesig=gaussgradvar;
   
    no_of_interpolating_points=3; //denotes the number of samples taken for each point, r,r-0.25,r+0.25

    imagefilename=basepath+curimagefile;
    scaleimagename=basepath+"scale-"+curimagefile;
    gradientimagename=basepath+"scale-gradient-"+curimagefile+".tif";
    gaussgradientimagename=basepath+"point-gradient-"+curimagefile;
    sordergradientimagename=basepath+"sogradient-"+curimagefile;
    sorderscaleimagename=basepath+"soscale-"+curimagefile;
    gradientcolorimagename=basepath+"scale-gradientcolor-"+curimagefile;
    gausscolorimagename=basepath+"point-gradient-color-"+curimagefile;
    localmaximaimagename=basepath+"localmaxima-"+curimagefile;
    softlocalmaximaimagename=basepath+"softlocalmaxima-"+curimagefile+".tif";
    Hysteresisimagename=basepath+"scalebasededge_"+curimagefile;
    secondderivativeimagename=basepath+"sd-"+curimagefile;
    localizedimagename=basepath+"Canny-"+curimagefile;
    dialatedimagename=basepath+"softmax-"+curimagefile+".tif";
    dividorimagename=basepath+"dividor-"+curimagefile;

    distributionfilename=basepath+"distribution.txt";
    sout.open(distributionfilename.c_str(),ios::out);

    CImg<unsigned short> jpgimage(imagefilename.c_str());
    Xdim=jpgimage.width();
    Ydim=jpgimage.height();
    cout<<"Image Size: "<<Xdim<< " X "<<Ydim<<"\n";
    allocateMemory(Xdim,Ydim);
    allocateGradientImageMemory(Xdim,Ydim);
    maxkernelsize=7*maxscale;
    if(maxkernelsize%2==0) maxkernelsize+=1;
    allocateKernelMemory(maxscale,maxkernelsize);
    PrecomputeKernels(maxscale);
    cout<<"Kernels Precomputed\n";

    readImage<unsigned short>(jpgimage,image);

    noisestdv=snr*snr;
    gradienthresh=6;
    gaussgradvar=atof(argv[2]);
    kernelsize=6*gaussgradvar;
    if(kernelsize%2==0) kernelsize+=1;
    kernelsize=kernelsize>3?kernelsize:3;
    initializeKernels(kernelsize);
    cout<<"Initilaized Point Gradients\n";
    compute_Gradient(image,Xdim,Ydim);
    compute_scale_Gradient_with_continuous_interpolation(image,Xdim,Ydim,b);
    sout.close();
    logger.close();

    return 0;
}

