#include <iostream>
#include <queue>
#include "Global.h"
#include "MathFunctions.h"
#include "CImg.h"
using namespace std;
using namespace cimg_library;

int PixelType(int x, int y){
    int Ncnt=0;
    for(int i=-1;i<=1;i++){
        for(int j=-1;j<=1;j++){
            if(i==0 && j==0) continue;
            if(x+i>=0 && x+i<Xdim && y+j>=0 && y+j<Ydim && skelImage[i][j]>0) Ncnt++;
        }
        switch(Ncnt){
        case 0:
            return ISOLATEDPOINT;
        case 1:
            return EDGEPOINT;
        case 2:
            return SIMPLEPOINT;
        default:
            return JUNCTIONPOINT;
        }
    }
}

void computeDT(string DTimagename){
    queue<Pixel> Q;
    for(int i=0;i<Xdim;i++){
        for(int j=0;j<Ydim;j++){
            if(skelImage[i][j]==0) {dtImage[i][j]=0; continue;}
            if(PixelType(i,j)==EDGEPOINT) {Q.push(Pixel(i,j,0));dtImage[i][j]=0;}
            else dtImage[i][j]=INT_MAX;
        }
    }

    while(!Q.empty()){
        Pixel p=Q.top();
        Q.pop();
        for(int i=-1;i<=1;i++){
            for(int j=-1;j<=1;j++){
                if(i==0 && j==0) continue;
                if(x+i>=0 && x+i<Xdim && y+j>=0 && y+j<Ydim ){
                    int distance=p.dtVal+get345Distance(p.x,p.y,i+p.x,j+p.y);
                    if( distance<dtImage[p.x+i][p.y+j] && PixelType(p.x+i,p.y+j)!=JUNCTIONPOINT){
                        dtImage[p.x+i][p.y+j]=distance;
                        Q.push(Pixel(p.x+i,p.y+j,distance));
                    }
                }
            }
        }
    }
    CImg<int> dtimg(Xdim,Ydim,1,1);
    writeImage<int, int>(dtImage,Xdim,Ydim,dtimg,DTimagename);
}

int main(int argc, char* argv[]){
    string curimagefile(argv[1]);
    DTimagename=basepath+"dt-"+curimagefile;
    Prunnedimagename=basepath+"prunened-"+curimagefile;
    imagefilename=basepath+curimagefile;
    CImg<unsigned short> jpgimage(imagefilename.c_str());
    Xdim=jpgimage.width();
    Ydim=jpgimage.height();
    allocateMemory(Xdim,Ydim);
    allocateGradientImageMemory(Xdim,Ydim);
    readImage<unsigned short>(jpgimage,skelImage);
    computeDT(DTimagename);
}
