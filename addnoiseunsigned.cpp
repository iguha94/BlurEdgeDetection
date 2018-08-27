
#include "CImg.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Global.h"
#include "Commons.h"
#include "ComputeGradient.h"
using namespace std;
using namespace cimg_library;

int main() {

    string displayimagefilename;
    cout<<"Enter the name of the Image: ";
    cin>>displayimagefilename;
    cout<<"Enter dimension of the image: ";
    cin>>Xdim>>Ydim;
    string displayimage=basepath+displayimagefilename;

    string noisefile;
    cout<<"Enter Noisy Image file name: ";
    cin>>noisefile;
    string noisyImagename=basepath+noisefile;
    CImg<unsigned short> noisyimage(noisyImagename.c_str());
    CImg<unsigned short> outputimg(Xdim,Ydim,1,1);
    allocateMemory(Xdim,Ydim);
    readImagefile(displayimage,image);
    //readImagefile(noisyImagename,gradientImage);

    cout<<"Image Size: "<<Xdim<< " X "<<Ydim<<"\n";

    double sigmaN;
    double contrast;
    int cnr[5];
    int noisemean;

    cout<<"Enter Noise stdv: ";
    cin>>sigmaN;
    cout<<"Enter contrast: ";
    cin>>contrast;
    cout<<"Noise Mean: ";
    cin>>noisemean;
    for(int i=0;i<5;i++){
        cout<<"Enter CNR: ";
        cin>>cnr[i];
    }

    for(int i=0;i<5;i++){
        double np=(contrast)/(cnr[i]*sigmaN);
        cout<<"Multiplier: "<<np<<endl;
        stringstream ss;
        ss << cnr[i];
        string str = ss.str();
        string outputfile2=basepath+"SNR-"+str+"-"+"phantom.raw";
        //string outputfile2=basepath+"SNR-"+str+"-"+displayimagefilename;

        for(int i=0;i<Xdim;i++){
            for(int j=0;j<Ydim;j++){
                unsigned short oint=image[i][j];
                unsigned short nint=noisyimage(i,j);
                int val =oint+np*(nint-noisemean);
                //cout<<val<<",";
                if(val<0) val=0;
                if(val>65535) val=1550;
                gradImage[i][j]=val;
            }
        }
        writeImagefile<unsigned short>(gradImage,Xdim,Ydim,outputfile2);
        //writeImageunsigned<unsigned short>(gradImage,Xdim,Ydim,outputimg,outputfile2);
    }
}
