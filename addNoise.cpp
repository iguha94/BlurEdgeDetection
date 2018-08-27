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
    string displayimage=basepath+displayimagefilename;

    string noisefile;
    cout<<"Enter Noisy Image file name: ";
    cin>>noisefile;
    string noisyImagename=basepath+noisefile;



    CImg<unsigned short> jpgimage(displayimage.c_str());
    CImg<unsigned short> noisyimage(noisyImagename.c_str());

    Xdim=jpgimage.width();
    Ydim=jpgimage.height();
    cout<<"Image Size: "<<Xdim<< " X "<<Ydim<<"\n";
    CImg<unsigned short> noisejpg(Xdim,Ydim,1,1);

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
        string outputfile2=basepath+"SNR-"+str+"-"+displayimagefilename;
        CImg<unsigned short> jpgimage(displayimage.c_str());
        CImg<unsigned short> noisyimage(noisyImagename.c_str());
        cimg_forXY(jpgimage,x,y)
        {
            int oint=jpgimage(x,y);
            int nint=noisyimage(x,y);
            //cout<<"Noise Value: "<<nint<<endl;
            int val =oint+np*(nint-noisemean);
            if(val>65535) noisejpg(x,y)=2000;
            else if(val<0) noisejpg(x,y)=0;
            else
                noisejpg(x,y)=val;
        }
//        cimg_forXY(jpgimage,x,y)
//        {
//            noisejpg(x,y)=255*((double)noisejpg(x,y)/max);
//        }
        noisejpg.save_tiff(outputfile2.c_str());
        noisejpg.display();
    }
}
