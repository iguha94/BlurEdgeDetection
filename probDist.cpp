#include "CImg.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Global.h"
#include "Commons.h"
#include "ComputeGradient.h"
#include <sstream>
using namespace std;
using namespace cimg_library;

const unsigned char white[] = { 255,255,255 };
const unsigned char yellow[] = { 255,255,0 };
const unsigned char red[] = { 255,0,0 };
const unsigned char blue[] = { 135,206,255 };
double multiplierx,multipliery,covmult;
string oimagename;
class dataPoint{
public:
    double x,  y,  gradx, grady, r, gaussval;
    dataPoint(double x, double y, double gradx, double grady, double r, double gaussval){
        this->x=x;
        this->y=y;
        this->gradx=gradx;
        this->grady=grady;
        this->r=r;
        this->gaussval=gaussval;
    }

};

class point2D{
public :
    double x, y,gval,hu;
    point2D(double x, double y,double gval,double hu){
        this->x=x;
        this->y=y;
        this->gval=gval;
        this->hu=hu;
    }
};

double convert_to_radian(double angle){
    double radianangle=(3.14*angle)/180;
    return radianangle;
}

bool isblankpixel(CImg<unsigned short> rgb,int height, int width,int x, int y){
    int r=rgb(x,y,0,0);
    int g=rgb(x,y,0,1);
    int b=rgb(x,y,0,2);
    if(r==0&&g==0&&b==0) return true;
    return false;
}

CImg<unsigned short>  fillellipse(CImg<unsigned short> rgb,int height, int width){
    CImg<unsigned short> rgb1(400,400,1,3,0);
    bool b1=true,b2=true,b3=true,b4=true;
    for(int x=0;x<height;x++){
        for(int y=0;y<width;y++){
            if(isblankpixel(rgb,height,width,x,y)){
                if(x+1<height) b1=isblankpixel(rgb,height,width,x+1,y) ;
                if(y+1<width) b2=isblankpixel(rgb,height,width,x,y+1) ;
                if(x-1>=0) b3=isblankpixel(rgb,height,width,x-1,y) ;
                if(y-1>=0) b4=isblankpixel(rgb,height,width,x,y-1) ;

                if(!b1||!b2||!b3||!b4) {
                    rgb1(x,y,0,0)=rgb(x+1,y,0,0);
                    rgb1(x,y,0,1)=rgb(x+1,y,0,1);
                    rgb1(x,y,0,2)=rgb(x+1,y,0,2);
                }
            }
            else{
                rgb1(x,y,0,0)=rgb(x,y,0,0);
                rgb1(x,y,0,1)=rgb(x,y,0,1);
                rgb1(x,y,0,2)=rgb(x,y,0,2);
            }
        }
    }
    return rgb1;
}

double interpolateangle(double x, double y, vector<dataPoint> pointvector){
    vector<dataPoint>::iterator it;
    double minhu=INT_MAX;
    double sminhu=INT_MAX;
    double dist=INT_MAX;
    double sdist=INT_MAX;
    for(it=pointvector.begin();it!=pointvector.end();it++){
        dataPoint p=*(it);
        double x1=p.x;
        double y1=p.y;
        double distance=CartesianDistance2Double(x,y,x1,y1);
        if(distance<dist) {
            sdist=dist;
            sminhu=minhu;
            dist=distance;
            minhu=computeAngle(x1,y1);
        }
        else if(distance<sdist){
            sdist=distance;
            sminhu=computeAngle(x1,y1);
        }
    }

    return (1/dist)*minhu+(1/sdist)*sminhu;
}

void drawEllipse(double height, double width,double meanx, double meany,double stdx,double stdy,vector<dataPoint> pointvector,vector<dataPoint> finalpointvector, bool putpoints=true){
    CImg<unsigned short> rgb(400,400,1,3,0);
    double angle;
    cout<<"Enter rotation angle: ";
    cin>>angle;
    vector<point2D> eclipsepoints;
    double angleinradian=convert_to_radian(angle);
    double std=sqrt(stdx*stdx+stdy*stdy)*0.75;
    for(double y=-height;y<=height;y+=0.1){
        for(double x=-width;x<=width;x+=0.1){
            if(height*height*x*x+y*y*width*width<=height*height*width*width){
                double grad=CartesianDistance2Double(meanx+x,meany+y,meanx,meany);
                double gaussval=colorgaussian(grad,0,std);
                double angle=computeAngle(x+meanx,y+meany);
                double rx=meanx+x*cos(angleinradian)+y*sin(angleinradian);
                double ry=meany-x*sin(angleinradian)+y*cos(angleinradian);
                point2D p2d(rx,ry,gaussval,angle);
                eclipsepoints.push_back(p2d);

            }
        }
    }
    cout<<"Eclipse Points: "<<eclipsepoints.size()<<endl;
    vector<dataPoint>::iterator it;
    vector<point2D>::iterator it1;
    for(it1=eclipsepoints.begin();it1!=eclipsepoints.end();it1++){
        point2D p=*(it1);
        double x=p.x;
        double y=p.y;
        double gaussval=p.gval;
        int fx=200+x*10;
        int fy=200-y*10;
        int* color=getcolorMapping(gaussval,p.hu);
        rgb(fx,fy,0,0)=color[0];
        rgb(fx,fy,0,1)=color[1];
        rgb(fx,fy,0,2)=color[2];
    }
    rgb=fillellipse(rgb,400,400);
    if(!putpoints){
        for(it=finalpointvector.begin(); it!=finalpointvector.end();it++){
            dataPoint p=*(it);
            int x=p.gradx*10+200;
            int y=200-p.grady*10;
            rgb.draw_circle(x,y,1,white);
        }
    }

    for(int i=0;i<400;i++){
        rgb(200,i,0,0)=255;
        rgb(200,i,0,1)=255;
        rgb(200,i,0,2)=255;

        rgb(i,200,0,0)=255;
        rgb(i,200,0,1)=255;
        rgb(i,200,0,2)=255;
    }

    string output=basepath+oimagename+"-plot.jpg";

    rgb.display();
    rgb.save_tiff(output.c_str());

}

void circularDistribution(double meanx, double meany,double stdx, double stdy, double covxy,vector<dataPoint> pointvector, vector<dataPoint> finalpointvector,int putpoints){
    int dimx,dimy;
    cout<<"Plot Dimension: ";
    cin>>dimx>>dimy;
    CImg<unsigned short> rgb(dimx,dimy,1,3,0);
    int xs,ys;
    int scalemult;
    cout<<"Enter Origin for scaling: ";
    cin>>xs>>ys;
    cout<<"Enter scaling factor: ";
    cin>>scalemult;
    cout<<"Enter multiplier for stdvx,stdvy,cov: ";
    cin>>multiplierx>>multipliery>>covmult;
    meanx*=scalemult;
    meany*=scalemult;
    meanx+=xs;
    meany=ys-meany;
    cout<<"MeanX: "<<meanx<<" , "<<"MeanY: "<<meany<<endl;
    cout<<"Xs: "<<xs<< " Ys: "<<ys<<endl;
    stdx*=multiplierx;
    stdy*=multipliery;
    covxy*=covmult;
    for(double x=0;x<dimx;x+=1){
        for(double y=0;y<dimy;y+=1){
            //cout<<"Processed: "<<x<< " , "<<y<<endl;
            double gaussval=multivariateGaussian(x,y,meanx,meany,stdx,stdy,covxy);
            int* color=getcolorMappingfordistribution(1,(1-gaussval));
            rgb(x,y,0,0)=color[0];
            rgb(x,y,0,1)=color[1];
            rgb(x,y,0,2)=color[2];
        }
    }

    rgb(xs,ys,0,0)=255;
    rgb(xs,ys,0,1)=255;
    rgb(xs,ys,0,2)=255;

    vector<dataPoint>::iterator it;
if(putpoints){
    for(it=finalpointvector.begin(); it!=finalpointvector.end();it++){
        dataPoint p=*(it);
        int x=p.gradx*scalemult+xs;
        int y=ys-p.grady*scalemult;
        rgb.draw_circle(x,y,2,white);
    }
}
    rgb.draw_circle(meanx,meany,2,yellow,0);
    string output=basepath+oimagename+"-plot.jpg";
    string grayoutput=basepath+oimagename+"-grayplot.jpg";
    cout<<output<<endl;
    cout<<grayoutput<<endl;
    rgb.display();
    rgb.save_tiff(output.c_str());

//    CImg<unsigned short> grayscale=convert_to_gray_scale_from_red(rgb,rgb.width(),rgb.height());
//    grayscale.display();
//    grayscale.save_tiff(grayoutput.c_str());
}

main(int argc,char* argv[]){
    string filename;
    double scale;
    double angle;
    int putpoints;
    vector<dataPoint> pointvector;
    vector<dataPoint> finalpointvector;
    basepath=string(argv[1]);
    oimagename=string(argv[2]);
    filename=string(argv[3]);
    scale=atoi(argv[4]);
    putpoints=atoi(argv[5]);
    //cout<<"Imagename: ";
    //cin>>oimagename;
    //cout<<"File name: ";
    //cin>>filename;

    //cout<<"intermediate scale value: ";
    //cin>>scale;
    //cout<<"Want to draw points (1 or 0): ";
    //cin>>putpoints;
    cout<<"Mean Scale: "<<scale<<endl;

    double nscale=scale;///2;

    string filepath=basepath+filename;
    ifstream fin;
    fin.open(filepath.c_str());

    double x, y, gradx, grady, stdx, stdy,covxy;
    double meanx=0;double meany=0;
    double pvcnt=0;
    double r;
    double minx=INT_MAX,miny=INT_MAX;
    double maxx=INT_MIN,maxy=INT_MIN;
    while(fin>> x>> y>> gradx>> grady>> r){
        //cout<<x<<","<<y<<","<<gradx<<","<<grady<<","<<r<<endl;
        dataPoint dp(x,y,gradx,grady,r,0);
        if(scale>=r){
            pointvector.push_back(dp);
            meanx+=gradx;
            meany+=grady;
            pvcnt++;
        }
        else if(scale+1==r){
            finalpointvector.push_back(dp);
        }
       // if(x==-1 && y==-1 && nscale==r) {
       //     meanx=gradx;meany=grady;
       // }

        if(gradx<minx) minx=gradx;
        if(gradx>maxx) maxx=gradx;

        if(grady<miny) miny=grady;
        if(grady>maxy) maxy=grady;
    }
    meanx/=pvcnt;meany/=pvcnt;
    cout<<meanx<<" :mean: "<<meany<<endl;
    double size=pointvector.size();
    vector<dataPoint>::iterator it;
    stdx=0;

    for(it=pointvector.begin(); it!=pointvector.end();it++){
        dataPoint p=*(it);
        stdx+=(meanx-p.gradx)*(meanx-p.gradx);
        stdy+=(meany-p.grady)*(meany-p.grady);
        covxy+=(meanx-p.gradx)*(meany-p.grady);
    }
    cout<<"Covariance: "<<covxy<<endl;
    stdx=sqrt(stdx/size);
    stdy=sqrt(stdy/size);
    covxy/=size;
    cout<<stdx<<" :std: "<<stdy<<endl;
    cout<<"Covxy: "<<covxy<<endl;
    cout<<minx<< " :X: "<<maxx<<endl;
    cout<<miny<< " :Y: "<<maxy<<endl;
    cout<<"Size of final scale: "<<finalpointvector.size()<<endl;
    cout<<"Size of point vector: "<<pointvector.size()<<"\n";
    circularDistribution(meanx,meany,stdx,stdy,covxy,pointvector,finalpointvector,putpoints);
    //drawEllipse(stdx,2*stdy,meanx,meany,stdx,stdy,pointvector,finalpointvector);
    fin.close();
}
