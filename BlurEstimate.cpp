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
    basepath=string(argv[3]);

    int kernelsize=6*gaussgradvar;
    if(kernelsize%2==0) kernelsize+=1;
    kernelsize=kernelsize>3?kernelsize:3;


    imagefilename=basepath+curimagefile;
    gaussgradientimagename=basepath+"gauss_"+curimagefile;

    CImg<unsigned short> jpgimage1(imagefilename.c_str());
    Xdim=jpgimage1.width();
    Ydim=jpgimage1.height();
    cout<<"Read Color Image\n";
    CImg<unsigned short> jpgimage=convert_to_gray_scale(jpgimage1,Xdim,Ydim);
    cout<<"Image Size: "<<Xdim<< " X "<<Ydim<<"\n";
    allocateMemory(Xdim,Ydim);
    allocateGradientImageMemory(Xdim,Ydim);
    initializeKernels(kernelsize);
    readImage<unsigned short>(jpgimage,image);

    compute_Gradient(image,Xdim,Ydim,false,true);
    int flag=1;
    double maxThreshold,minThreshold,threshold;

    cout<<"*********************************\n";
    cout<<"Blur Estimation Marzilliano's Method\n";
    cout<<"*********************************\n";
    double totalwidthcount=0;
    double totalvedgecount=0;
    for(int i=2;i<Xdim-2;i++){
    	for(int j=2;j<Ydim-2;j++){
    	
    		if(Gradientangle[i][j]<=15 || Gradientangle[i][j]>=345 || (Gradientangle[i][j]>=165 && Gradientangle[i][j]<=195)){
    			bool findmaxima=false;
    			bool findminima=false;
    			int start=i;
    			int end=i;
    			double centerval=gradientImage[i][j];
    			if(i-1>=0){
    				start=i-1;
    				if(gradientImage[i-1][j]>=centerval) {findmaxima=true;
    				centerval=gradientImage[i-1][j];
					}
    				if(gradientImage[i-1][j]<centerval){
    					 findminima=true; centerval=gradientImage[i-1][j];
					}
				}
    			for(int x=i-2;x>=0;x--){
    				if(findmaxima){
    					if(gradientImage[x][j]>centerval){
    						centerval=gradientImage[x][j];
    						start=x;
						}
						else break;
					}
					if(findminima){
						if(gradientImage[x][j]<centerval){
    						centerval=gradientImage[x][j];
    						start=x;
						}
						else break;
					}
				}
				
			 findmaxima=false;
    		 findminima=false;
				
				if(i+1<Xdim){
    				end=i+1;
    				if(gradientImage[i+1][j]>=centerval) {findmaxima=true;
    				centerval=gradientImage[i+1][j];
					}
    				if(gradientImage[i+1][j]<centerval){
    					 findminima=true; centerval=gradientImage[i+1][j];
					}
				}
				
				for(int x=i+2;x<Xdim;x++){
    				if(findmaxima){
    					if(gradientImage[x][j]>centerval){
    						centerval=gradientImage[x][j];
    						start=x;
						}
						else break;
					}
					if(findminima){
						if(gradientImage[x][j]<centerval){
    						centerval=gradientImage[x][j];
    						start=x;
						}
						else break;
					}
				}
				cout<<"Width at location: "<<i<<" , "<<j<<" : "<<fabs(end-start)<<"\n";
				totalwidthcount+=fabs(end-start);
				totalvedgecount++;
			}
		}
		
	}
            cout<<"Image Blur Value: "<<(totalwidthcount/totalvedgecount)<<"\n";

      return 0;
}


