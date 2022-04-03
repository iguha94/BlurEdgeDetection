#include "CImg.h"
#include "Global.h"
#include "Commons.h"
#include "ComputeGradient.h"
#include <fstream>
#include <iostream>
#include <string>
#include <time.h>
#include <sstream>
#include <vector>

using namespace std;

int main(int argc, char* argv[]){
    const unsigned char red[] = { 255,0,0 };
    string basepath(argv[1]);
    string displayimagefilename(argv[2]);

    string displayimage=basepath+displayimagefilename;
    
    string scaleimagename1(argv[3]);
    
    string imagename=basepath+scaleimagename1;

    string circledimagename3=basepath+"circled-scale-"+displayimagefilename;

    CImg<unsigned short> jpgimage(displayimage.c_str()); //read scaled image
    CImg<unsigned short> pointimage(imagename.c_str()); //read original image
    cout<<"Xdim: "<<jpgimage.width()<<" Ydim: "<<jpgimage.height()<<"\n";
    CImg<unsigned short> circledimage(jpgimage.width(),jpgimage.height(),1,1,0); //create an RGB image of same dimension to draw the circle.
    ifstream file;
    file.open(basepath+"Points.txt");
    int x,y;
    while(file>>x>>y){
     cout<<x<<" , "<<y<<" , "<<pointimage(x,y)/2<<endl;
        circledimage.draw_circle(x,y,(float)(pointimage(x,y)/2),red,1,1); //read the points in file and draw circles
        //circledimage.draw_point(x,y,red);
    }
    circledimage.display();
    CImg<unsigned short> rgbpoint=convert_to_RGB_image<unsigned short>(jpgimage,circledimage);
    rgbpoint.display();
    rgbpoint.save_tiff(circledimagename3.c_str());
}
