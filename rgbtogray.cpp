#include <iostream>
#include "Global.h"
#include "MathFunctions.h"
#include "Commons.h"
using namespace std;

int main(){
    string rgbimage;
    cout<<"Enter the name of the rgb image: ";
    cin>>rgbimage;
    string rgbimagefilename=basepath+rgbimage;
    string grayscaleimagefilename=basepath+"gray-"+rgbimage;
    CImg<unsigned short> jpgimage(rgbimagefilename.c_str());
    int Xdim=jpgimage.width();
    int Ydim=jpgimage.height();
    CImg<unsigned short> grayjpgimage=convert_to_gray_scale<unsigned short>(jpgimage,Xdim,Ydim);
    grayjpgimage.save_tiff(grayscaleimagefilename.c_str());
    grayjpgimage.display();
}
