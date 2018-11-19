#include <iostream>
#include "CImg.h"
//#include "Global.h"
//#include "MathFunctions.h"
#include "Commons.h"

using namespace std;
using namespace cimg_library;

int r,g,b;
void ImposeonGrayImage(string imagename, string skeletonname){

    string rgb_image_name=basepath+"overlapped-"+imagename;
    imagename=basepath+imagename;
    skeletonname=basepath+skeletonname;

    CImg<unsigned short> jpgimage(imagename.c_str());
    Xdim=jpgimage.width();
    Ydim=jpgimage.height();
    CImg<unsigned short>RGBimage(jpgimage.width(), jpgimage.height(), 1, 3, 0);
    RGBimage=convert_to_RGB_image<unsigned short>(jpgimage);
    CImg<unsigned short> skeljpgimage(skeletonname.c_str());
    cimg_forXY(skeljpgimage,x,y){
        if(x>2 && y>2 && x<Xdim-3 && y<Ydim-3 && skeljpgimage(x,y)>1){
            RGBimage(x,y,0,0)=(r);
            RGBimage(x,y,0,1)=(g);
            RGBimage(x,y,0,2)=(b);
        }
    }
    RGBimage.save_tiff(rgb_image_name.c_str());
    RGBimage.display();
}

void ImposeonRGBImage(string imagename, string skeletonname){

    string rgb_image_name=basepath+"overlapped-"+imagename;
    imagename=basepath+imagename;
    skeletonname=basepath+skeletonname;

    CImg<unsigned short> RGBimage(imagename.c_str());
    Xdim=RGBimage.width();
    Ydim=RGBimage.height();
    CImg<unsigned short> skeljpgimage(skeletonname.c_str());
    cimg_forXY(skeljpgimage,x,y){
        if(skeljpgimage(x,y)>1){
            RGBimage(x,y,0,0)=(r);
            RGBimage(x,y,0,1)=(g);
            RGBimage(x,y,0,2)=(b);
        }
    }
    RGBimage.save_tiff(rgb_image_name.c_str());
    RGBimage.display();
}

int main(int argc, char*argv[]){
    string imagename(argv[1]);
    string skeletonname(argv[2]);
    r=atoi(argv[3]);
    g=atoi(argv[4]);
    b=atoi(argv[5]);
    basepath=string(argv[6]);
    ImposeonGrayImage(imagename,skeletonname);
    //ImposeonRGBImage(imagename,skeletonname);
}
