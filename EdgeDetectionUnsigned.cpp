#include "CImg.h"
#include "Global.h"
#include "Commons.h"
#include "ComputeGradient.h"
#include "continuousscaleComputation.h"
#include <iostream>
#include <string>
#include <time.h>

using namespace std;
using namespace cimg_library;


int main() {

    string curimagefile;
    while(true){
        cout<<"Enter Image Name: ";
        cin>>curimagefile;

        cout<<"Enter dimension of the image: ";
        cin>>Xdim>>Ydim;
        
        confidence=0.05;
        bool b=true;

        gaussvar=1;
        int kernelsize=7;
        cout<<"Enter gaussian kernel size: ";
        cin>>kernelsize;

        cout<<"Enter stdV for the gaussian kernel: ";
        cin>>gaussgradvar;

        cout<<"Enter Color Gaussian variance: ";
        cin>>colorgaussvar;

        cout<<"Enter maximum Scale: ";
        cin>>maxscale;

        cout<<"Enter Sampling Rate: ";
        cin>>sampling_rate;

        cout<<"Enter Noise standard deviation: ";
        cin>>snr;

        snr*=3;

        //        cout<<"Enter the pixel of interest: ";
        //        cin>>Xc>>Yc;

        //        cout<<"Enter intermediate scale: ";
        //        cin>>intermediatescale;
        string sfname=basepath+"gradMap.txt";
        sout.open(sfname.c_str());

        no_of_interpolating_points=3;

        imagefilename=basepath+curimagefile;
        scaleimagename=basepath+"scale-"+curimagefile+".jpg";
        gradientimagename=basepath+"scale-gradient-"+curimagefile;
        gaussgradientimagename=basepath+"point-gradient-"+curimagefile;
        sordergradientimagename=basepath+"sogradient-"+curimagefile;
        sorderscaleimagename=basepath+"soscale-"+curimagefile;
        gradientcolorimagename=basepath+"scale-gradientcolor-"+curimagefile;
        gausscolorimagename=basepath+"point-gradient-color-"+curimagefile;

        //CImg<unsigned short> jpgimage(imagefilename.c_str());
        //Xdim=jpgimage.width();
        // Ydim=jpgimage.height();
        cout<<"Image Size: "<<Xdim<< " X "<<Ydim<<"\n";

        allocateMemory(Xdim,Ydim);
        allocateGradientImageMemory(Xdim,Ydim);
        initializeKernels(kernelsize);
        //print_Kernel();
        //readImage<unsigned short>(jpgimage,image);
        readImagefile(imagefilename,image);

        cout<<"Variance: "<<snr<<"\n";
        gradienthresh=snr;
        cout<<"confidence: "<<confidence<<"\n";
        cout<<"Base threshold: "<<compute_threshold(6)<<"\n";
        //interpolatingpoint_weight=butterworth(0.5,1,2);
        compute_Gradient(image,Xdim,Ydim,true);
        compute_scale_Gradient_with_continuous_interpolation(image,Xdim,Ydim,b);
        sout.close();
    }
    return 0;
}

