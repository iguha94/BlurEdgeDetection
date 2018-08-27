#include <iostream>
#include <fstream>
#include <math.h>
#include "CImg.h"
#include "Global.h"
#include "Commons.h"
using namespace std;

int main(){
    string ccodefname;
    cout<<"Enter the name of the image: ";
    cin>>ccodefname;
    
    string ccodefilename=basepath+ccodefname;
    string colorcodedimage=basepath+"color-"+ccodefname;
    CImg<unsigned short> jpgimage(ccodefilename.c_str());
    Xdim=jpgimage.width();
    Ydim=jpgimage.height();
    
    allocateMemory(Xdim,Ydim);
    readImage<unsigned short>(jpgimage,image);
    colorcoding(image,colorcodedimage);
}
