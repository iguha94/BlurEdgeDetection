#include "CImg.h"
#include "Global.h"
#include <iostream>
using namespace std;
using namespace cimg_library;

int main(int argc, char* argv[]) {

    string displayimagefilename;
     string basepath(argv[1]);
//string basepath;
    while(true){
    cout<<"Enter the name of the file to be displayed: ";
    cin>>displayimagefilename;
    //string basepath(argv[1]);
    string displayimage=basepath+displayimagefilename;
    CImg<unsigned short> jpgimage(displayimage.c_str());
    jpgimage.display();
    }
    return 0;
}
  
