#include "CImg.h"
#include "Global.h"
#include "Commons.h"
#include <iostream>
#include <string>
#include <time.h>
#include <fstream>
#include <iostream>
using namespace std;
const unsigned char red[] = { 255,0,0 };
int main()
{
    string imagename,orgimg;
    string scaleimagename;
    int sx, sy;
    int ex, ey;
    cout << "Image name: ";
    cin >> scaleimagename;
    cout << "Blur scale image name: ";
    cin >> imagename;
    cout<<"Enter original imagename: ";
    cin>>orgimg;

    string scaleimage=basepath+"scale-"+scaleimagename;
    string originalimage=basepath+orgimg;
    string imagefilename=basepath+imagename;
    string circledImage=basepath+"Pointsmarked-"+orgimg;


    string mapexcelfile =basepath+scaleimagename+"plotData.csv";
    
    ofstream fout;
    ifstream fin;
    fout.open(mapexcelfile.c_str());
    fin.open("scaleMap.txt");

    CImg<unsigned short> blurscaleimage(imagefilename.c_str());
    CImg<unsigned short> scaleimagecomputed(scaleimage.c_str());
    CImg<unsigned short> circled(originalimage.c_str());
    CImg<unsigned short> rgbimg=convert_to_RGB_image<unsigned short>(circled);
    Xdim = blurscaleimage.width();
    Ydim = blurscaleimage.height();
    int x, y,z;
    while(fin>>x>>y){
        fout<<x<<" "<<y<<" "<<(double)blurscaleimage(x,y)/10<<" "<<(double)scaleimagecomputed(x,y)/2<<"\n";
        rgbimg.draw_circle(x,y,1,red,1,1);
        //rgbimg.draw_circle(x,y,2,red,1,1);
    }
    fin.close();
    fout.close();
    rgbimg.display();
    rgbimg.save_tiff(circledImage.c_str());
}
