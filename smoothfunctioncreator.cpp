#include <iostream>
#include "CImg.h"
#include "Commons.h"
#include "Global.h"
#include <fstream>
#include <string>
#include <utility>
#include <list>
#include <map>
using namespace std;
using namespace cimg_library;

class  CPoint{
public:
    int x;
    int y;
    int value;
    CPoint(int x, int y, int value){
        this->x=x;
        this->y=y;
        this->value=value;
    }
};

int main(){
    string imagename;
    int interval;
    cout<<"Enter ImageName: ";
    cin>>imagename;
    cout<<"Enter sampling interval (square)";
    cin>>interval;
    int range;
    cout<<"Maximum scale";
    cin>>range;
    string imagefile=basepath+imagename;
    string blurimagefile=basepath+"blur3.png";
    fstream fin;
    fin.open("samplepoints.txt");

    CImg<unsigned short> image(imagefile.c_str());
    CImg<unsigned short> blurimage(image.width(), image.height(), 1, 1, 0);;
    allocateMemory(image.width(), image.height());
    list<CPoint > controlpoints;
    map<long long int, CPoint*> pointsmap;
    map<long long int, CPoint*>::iterator it;

    int x,y,value;

//    for(int i=0;i<image.width();i+=interval){
//        for(int j=0;j<image.height();j+=interval){
//            long long int index=calculateIndex(i,j,image.width(),image.height());
//            int value=rand()%range;
//            CPoint* cp=new CPoint(i,j,value);
//            CPoint cp1(x,y,value);
//            controlpoints.push_back(cp1);
//            pointsmap.insert(pair<long long int,CPoint*>(index, cp));
//        }
//    }

        while(fin>>x>>y>>value){
            //if(value>3 && value<8) value+=5;
            CPoint* cp=new CPoint(x,y,value);
            CPoint cp1(x,y,value);
            controlpoints.push_back(cp1);
            pointsmap.insert(pair<long long int,CPoint*>(calculateIndex(x,y,image.width(),image.height()), cp));
            blurimage(x,y)=value;
        }

    for(int i=0;i<image.width();i+=interval){
        for(int j=0;j<image.height();j+=interval){
            long long int index=calculateIndex(i,j,image.width(),image.height());
            if(pointsmap.find(index)!=pointsmap.end()){
                blurimage(i,j)=pointsmap[index]->value;
                //cout<<"Here\n";
            }
            else{
                double totalsum=0;
                double totalweight=0;
                double minimum=INT_MAX;
                double smin=INT_MAX;
                double sminval,minval;
                for (list<CPoint>::iterator it=controlpoints.begin(); it != controlpoints.end(); ++it){
                    int x=(*it).x;
                    int y=(*it).y;
                    int val=(*it).value;
                    double distance=CartesianDistance2D(i,j,x,y);
                    if(distance<minimum) {smin=minimum;minimum=distance;sminval=minval;minval=val;}
                    else if(distance<smin) {smin=distance;sminval=val;}
                }
                double sminw=1/smin;
                double minw=1/minimum;
                totalweight=(sminval*sminw+minval*minw);
                totalsum=sminw+minw;
               blurimage(i,j)=totalweight/totalsum;
               //blurimage(i,j)=maximum;
            }
        }
    }

    blurimage.display();
    blurimage.save_tiff(blurimagefile.c_str());
}
