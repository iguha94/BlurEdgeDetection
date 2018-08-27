#ifndef EDGELOCALIZATION_H
#define EDGELOCALIZATION_H

#include <iostream>
#include "Global.h"
#include "Commons.h"
#include "MathFunctions.h"
#include "CImg.h"
#include "ComputeGradient.h"
using namespace std;
using namespace cimg_library;

double Non_Maxima_Interpolation(double** arr,double x, double y){
    int x1,y1,x2,y2,x3,y3,x4,y4;
    double averageweight_X=0;
    x1=ceil(x); y1=ceil(y); //ceil(x), ceil(Y) -- Point 1 for interpolation
    y2=floor(y); //ceil(x),floor(y) -- Point 2 for interpolation
    x3=floor(x);y3=ceil(y); //floor(x),ceil(y) -- Point 3 for interpolation
    y4=floor(y); //floor(x),floor(y) -- Point 4 for interpolation

    int pcount=0;
    if( x1>=0 && x1<Xdim && y1>=0 && y1<Ydim ){averageweight_X+=arr[x1][y1];++pcount;}
    if( x1>=0 && x1<Xdim && y2>=0 && y2<Ydim ){averageweight_X+=arr[x1][y2];++pcount;}
    if( x3>=0 && x3<Xdim && y3>=0 && y3<Ydim ){averageweight_X+=arr[x3][y3];++pcount;}
    if( x3>=0 && x3<Xdim && y4>=0 && y4<Ydim ){averageweight_X+=arr[x3][y4];++pcount;}
    if(pcount==0) return averageweight_X;
    return averageweight_X/pcount;
}

double Bilinear_interpolation(double** arr, double x, double y){
    int x1,y1,x2,y2;
    x1=ceil(x); y1=ceil(y);
    x2=floor(x);
    y2=floor(y);
    if(x1<0) x1=0; if(x1>=Xdim) x1=Xdim-1;
    if(y1<0) y1=0; if(y1>=Ydim) y1=Ydim-1;
    if(x2<0) x2=0; if(x2>=Xdim) x2=Xdim-1;
    if(y2<0) y2=0; if(y2>=Ydim) y2=Ydim-1;
    double fq11=arr[x2][y1];
    double fq12=arr[x2][y2];
    double fq22=arr[x1][y2];
    double fq21=arr[x1][y1];
    double coeff;
    if((x1-x2)*(y1-y2)==0) coeff=1;
    else coeff=1.0/((x1-x2)*(y1-y2));
    double num1=(y-y2)*fq11*(x1-x)+fq21*(x-x2)*(y-y2);
    double num2=(y1-y)*(x1-x)*fq12+(y1-y)*(x-x2)*fq22;
    double val= (num1+num2)*coeff;
    if(coeff!=1) cout<<"Coefficient: "<<coeff<<"\n";
    if(val==0) return arr[x1][y1];
    else return val;
}

double Weighted_Non_Maxima_Interpolation(double** arr,double x, double y){
    int x1,y1,x2,y2,x3,y3,x4,y4;
    double w1=0;double w2=0;double w3=0;double w4=0;
    double averageweight_X=0;
    x1=ceil(x); y1=ceil(y); //ceil(x), ceil(Y) -- Point 1 for interpolation
    y2=floor(y); //ceil(x),floor(y) -- Point 2 for interpolation
    x3=floor(x);y3=ceil(y); //floor(x),ceil(y) -- Point 3 for interpolation
    y4=floor(y); //floor(x),floor(y) -- Point 4 for interpolation

    double weight=0;
    if( x1>=0 && x1<Xdim && y1>=0 && y1<Ydim ){
        w1=CartesianDistance2Double(x1,y1,x,y);
        if(w1==0) w1=1;
        averageweight_X+=((1.0/w1)*arr[x1][y1]);
        weight+=(1.0/w1);
    }
    if( x1>=0 && x1<Xdim && y2>=0 && y2<Ydim ){
        w2=CartesianDistance2Double(x1,y2,x,y);
        if(w2==0) w2=1;
        averageweight_X+=((1.0/w2)*arr[x1][y2]);
        weight+=(1.0/w2);
    }
    if( x3>=0 && x3<Xdim && y3>=0 && y3<Ydim ){
        w3=CartesianDistance2Double(x3,y3,x,y);
        if(w3==0)w3=1;
        averageweight_X+=((1.0/w3)*arr[x3][y3]);
        weight+=(1.0/w3);
    }
    if( x3>=0 && x3<Xdim && y4>=0 && y4<Ydim ){
        w4=CartesianDistance2Double(x3,y4,x,y);
        if(w4==0) w4=1;
        averageweight_X+=((1.0/w4)*arr[x3][y4]);
        weight+=(1.0/w4);
    }
    if(weight==0) return averageweight_X;
    return averageweight_X/weight;
}

template<class GradType, class AngleType>
void NonmaximaSuppression(GradType** scalegradientImg, AngleType** angleArray,double threshold){
    for(int i=0;i<Xdim;i++){
        for(int j=0;j<Ydim;j++){
            double angle=angleArray[i][j]; //in degree
            double gradientMagnitude=scalegradientImg[i][j];
            double radius=(double)((double)ReliableScale[i][j]/2.0);
            double rad_angle=degtorad(angle);//convert to radian
            double start=0.5;
            LocalMaxima[i][j]=scalegradientImg[i][j];
            bool localMaxima=true;
            double min1=gradientMagnitude;
            double min2=gradientMagnitude;

            for(double r=-radius;r<=-0.5;r+=0.5){
                double xcoord=i-r*cos(rad_angle);
                double ycoord=j+r*sin(rad_angle);
                double interpolated_gradient_value=Bilinear_interpolation(scalegradientImg,xcoord,ycoord);
                if(interpolated_gradient_value>gradientMagnitude){localMaxima=false; break;}
                if(min1>interpolated_gradient_value)min1=interpolated_gradient_value;
            }

            if(localMaxima){
                for(double r=0.5;r<=radius;r+=0.5){
                    double xcoord=i-r*cos(rad_angle);
                    double ycoord=j+r*sin(rad_angle);
                    double interpolated_gradient_value=Bilinear_interpolation(scalegradientImg,xcoord,ycoord);
                    if(interpolated_gradient_value>gradientMagnitude){localMaxima=false; break;}
                    if(min2>interpolated_gradient_value)min2=interpolated_gradient_value;
                }
            }
            if(!localMaxima) LocalMaxima[i][j]=0;
            else{
                double diff1=gradientMagnitude-min1;
                double diff2=gradientMagnitude-min2;
                double depth_thresh=sqrt(diff1*diff2)/(2*radius);
                if(depth_thresh<threshold) LocalMaxima[i][j]=0;
            }
        }
    }
    CImg<int> localmaximaimage(Xdim,Ydim,1,1);
    writeImage<double, int>(LocalMaxima,Xdim,Ydim,localmaximaimage,localmaximaimagename,true);
}

void HysteresisThresholding(double MinThresh, double MaxThresh){
    bool changed=true;
    for(int i=0;i<Xdim;i++){
        for(int j=0;j<Ydim;j++){
            if(LocalMaxima[i][j]>=MaxThresh) Hysteresis[i][j]=255;
            else if(LocalMaxima[i][j]>=MinThresh) Hysteresis[i][j]=128;
            else Hysteresis[i][j]=0;
        }
    }
    cout<<"Initialized Hysteresis\n";
    while(changed){
        changed=false;
        for(int x=0;x<Xdim;x++){
            for(int y=0;y<Ydim;y++){
                if(Hysteresis[x][y]==255){
                    for(int i=-1;i<=1;i++)
                        for(int j=-1;j<=1;j++){
                            if(i==0 && j==0) continue;
                            else{
                                if(x+i>=0 && x+i<Xdim && y+j>=0 && y+j<Ydim){
                                    if(Hysteresis[x+i][y+j]==128) {Hysteresis[x+i][y+j]=255; changed=true;}
                                }
                            }
                        }
                }
            }
        }

    }

    for(int i=0;i<Xdim;i++)
        for(int j=0;j<Ydim;j++){
            if(Hysteresis[i][j]==128) Hysteresis[i][j]=0;
        }

    CImg<unsigned short> Hysteresisimage(Xdim,Ydim,1,1);
    writeImage<double, unsigned short>(Hysteresis,Xdim,Ydim,Hysteresisimage,Hysteresisimagename,true);
}


#endif // EDGELOCALIZATION_H
