#include <iostream>
#include "CImg.h"
//#include "Global.h"
//#include "MathFunctions.h"
#include "Commons.h"

using namespace std;
using namespace cimg_library;

int r,g,b;
void ImposeonGrayImage(string imagename, string scalename,int maxscale){

    string rgb_image_name=basepath+"thresh_"+imagename;
    imagename=basepath+imagename;
    scalename=basepath+scalename;

    CImg<unsigned short> jpgimage(imagename.c_str());
    Xdim=jpgimage.width();
    Ydim=jpgimage.height();
    CImg<unsigned short> skeljpgimage(scalename.c_str());
    cimg_forXY(skeljpgimage,x,y){
        if(x>2 && y>2 && x<Xdim-3 && y<Ydim-3 && skeljpgimage(x,y)>=maxscale){
            jpgimage(x,y)=0;
        }
    }
    jpgimage.save_tiff(rgb_image_name.c_str());
    jpgimage.display();
}


int main(int argc, char*argv[]){
    basepath=string(argv[1]);
    string imagename(argv[2]);
    string scalename(argv[3]);
    maxscale=atoi(argv[4]);
    
    ImposeonGrayImage(imagename,scalename,maxscale);
    //ImposeonRGBImage(imagename,skeletonname);
}
    