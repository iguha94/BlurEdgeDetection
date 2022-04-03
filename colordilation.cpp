//
// Created by iguha on 7/11/19.
//

#include <iostream>
#include <string>
#include "CImg.h"
using namespace std;
using namespace cimg_library;

//string basepath="/user/iibi/iguha/Documents/Phd-iguha/Journals/2019/Blur-Scale-Edge-Detection/Images/RealData/";
int red=255;
int green=0;
int blue=255;

unsigned short** getImagearray(CImg<unsigned short> cimg,unsigned short** arr){
    cimg_forXY(cimg,x,y) {arr[x][y]=cimg(x,y);}
    return arr;
}

void deallocate(unsigned short** arr,int Xdim,int Ydim){
    for(int i=0;i<Xdim;i++){
        free(arr[i]);
    }
    free(arr);
}

unsigned short** allocate( int Rows, int Cols){
    unsigned short** image = NULL;
    image = new unsigned short*[Rows];
    for(int i=0;i<Rows;i++){
        image[i]=new unsigned short[Cols];
        for(int j=0;j<Cols;j++){
            image[i][j]=0;
        }
    }
    return image;
}

void Dialate_Edge_Color(string imagename, string edgename, string dialatededgename){
    CImg<unsigned short> image(imagename.c_str());
    int Xdim=image.width();
    int Ydim=image.height();
    //cout<<"Image Size: "<<Xdim<< " X "<<Ydim<<"\n";

    CImg<unsigned short> edgeimage(edgename.c_str());

    unsigned short** imagearr=allocate(Xdim,Ydim);
    unsigned short** edgeimagearr=allocate(Xdim,Ydim);
    unsigned short** dialateimagearr=allocate(Xdim,Ydim);

    imagearr=getImagearray(image,imagearr);
    edgeimagearr=getImagearray(edgeimage,edgeimagearr);

    CImg<unsigned short>RGBimage(Xdim,Ydim, 1, 3, 0);

    for(int x=0;x<Xdim;x++){
        for(int y=0;y<Ydim;y++){
            if(edgeimagearr[x][y]<150) {
                RGBimage(x,y,0,0)=imagearr[x][y];
                RGBimage(x,y,0,1)=imagearr[x][y];
                RGBimage(x,y,0,2)=imagearr[x][y];
                continue;
            }
            double average=0;
            double max=INT_MIN;
            /*for(int i=-1;i<=1;i++){
                for(int j=-1;j<=1;j++){
                    if(i==0 && j==0) continue;
                    if(x+i>=0 && x+j<
                    average+=imagearr[x+i][y+j];
                    if(max<imagearr[x+i][y+j]) max = imagearr[x+i][y+j];
                }
            }*/
            average/=9;
            //double weight=1.0-(average/255.0);
            //double weight=1.0-(max/255.0);

            RGBimage(x,y,0,0)=0;
            RGBimage(x,y,0,1)=255;//125*weight;
            RGBimage(x,y,0,2)=0;
        }
    }
    for(int x=0;x<4;x++){
	for(int y=0;y<Ydim;y++){
	RGBimage(x,y,0,0)=imagearr[x][y];
 	RGBimage(x,y,0,1)=imagearr[x][y];
	RGBimage(x,y,0,2)=imagearr[x][y];
}
}
for(int x=Xdim-4;x<Xdim;x++){
for(int y=0;y<Ydim;y++){
 RGBimage(x,y,0,0)=imagearr[x][y];
        RGBimage(x,y,0,1)=imagearr[x][y];
        RGBimage(x,y,0,2)=imagearr[x][y];

}
}

    for(int x=0;x<Xdim;x++){
        for(int y=0;y<4;y++){
        RGBimage(x,y,0,0)=imagearr[x][y];
        RGBimage(x,y,0,1)=imagearr[x][y];
        RGBimage(x,y,0,2)=imagearr[x][y];
}
}
for(int x=0;x<Xdim;x++){
for(int y=Ydim-4;y<Ydim;y++){
 RGBimage(x,y,0,0)=imagearr[x][y];
        RGBimage(x,y,0,1)=imagearr[x][y];
        RGBimage(x,y,0,2)=imagearr[x][y];

}
}

    deallocate(imagearr,Xdim,Ydim);
    deallocate(edgeimagearr,Xdim,Ydim);

    RGBimage.save_tiff(dialatededgename.c_str());
//RGBimage.display();

}

int main(int argc, char* argv[]){
    string imagename(argv[1]);
    string skeletonname(argv[2]);
    int r=atoi(argv[3]);
    int g=atoi(argv[4]);
    int b=atoi(argv[5]);
    string basepath(argv[6]);
//
//    string folder(argv[1]);
//    string image(argv[2]);
//    basepath=basepath+folder;
      string imagepath=basepath+imagename;
      string skeletonpath=basepath+skeletonname;
//    string ez_org_edge=basepath+"EZ_edge.jpg";
//    string ez_mat_edge=basepath+"EZ_"+image;
//    string canny_edge=basepath+"Canny-"+image;
//    string scale_edge=basepath+"scalebasededge_"+image;
//
      string overlappedfile=basepath+"overlapped-"+skeletonname;
      cout<<imagepath<<endl;
//    string dial_ez_mat_edge=basepath+"EZ_dial_"+image;
//    string dial_canny_edge=basepath+"Canny-dial_"+image;
//    string dial_scale_edge=basepath+"scalebasededge_dial_"+image;

    Dialate_Edge_Color(imagepath,skeletonpath,overlappedfile);

}
