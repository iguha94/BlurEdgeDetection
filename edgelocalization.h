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

template<class GradType, class AngleType>
double NonmaximaSuppression_Canny(GradType** scalegradientImg,GradType** sgradX,GradType** sgradY, AngleType** angleArray,double threshold){
    int yb1,yt1,yb2,yt2;
    int xb1,xb2,xt1,xt2;
    for(int i=1; i<Xdim-1;i++){
        for(int j=1;j<Ydim-1;j++){
            double gmx=i;double gmy=j;
            double angle=angleArray[i][j];
            double gradientMagnitude=scalegradientImg[i][j];
            double px=sgradX[i][j];
            double py=sgradY[i][j];
            int r=(int)((int)ReliableScale[i][j]/2.0);
            if(r==0) r=1;
            LocalMaxima[i][j]=scalegradientImg[i][j];
            double localmaxima=true;
            double gm;
            if((angle>=0 && angle<=45)||(angle>=180 && angle<225)){

                yb1=j;xb1=i+1;yb2=j-1;xb2=i+1;
                yt1=j;xt1=i-1;yt2=j+1;xt2=i-1;
                double xest=fabs(py/px);
                double val1= xest*scalegradientImg[xt2][yt2]+(1-xest)*scalegradientImg[xt1][yt1];
                double val2= xest*scalegradientImg[xb2][yb2]+(1-xest)*scalegradientImg[xb1][yb1];
                gm=sqrt(fabs(gradientMagnitude-val1)*fabs(gradientMagnitude-val2));
                if(val1>gradientMagnitude || val2>gradientMagnitude) LocalMaxima[i][j]=0;

            }
            else if((angle>45 && angle<=90)||(angle>225 && angle<=270)){

                yb1=j+1;xb1=i;yb2=j+1;xb2=i-1;
                yt1=j-1;xt1=i;yt2=j-1;xt2=i+1;
                double xest=fabs(px/py);
                double val1= xest*scalegradientImg[xt2][yt2]+(1-xest)*scalegradientImg[xt1][yt1];
                double val2= xest*scalegradientImg[xb2][yb2]+(1-xest)*scalegradientImg[xb1][yb1];
                gm=sqrt(fabs(gradientMagnitude-val1)*fabs(gradientMagnitude-val2));
                if(val1>gradientMagnitude || val2>gradientMagnitude) LocalMaxima[i][j]=0;

            }
            else if((angle>90 && angle<=135)||(angle>270 && angle<=315)){

                yb1=j+1;xb1=i;yb2=j+1;xb2=i+1;
                yt1=j-1;xt1=i;yt2=j-1;xt2=i-1;
                double xest=fabs(px/py);
                double val1= xest*scalegradientImg[xt2][yt2]+(1-xest)*scalegradientImg[xt1][yt1];
                double val2= xest*scalegradientImg[xb2][yb2]+(1-xest)*scalegradientImg[xb1][yb1];
                gm=sqrt(fabs(gradientMagnitude-val1)*fabs(gradientMagnitude-val2));
                if(val1>gradientMagnitude || val2>gradientMagnitude) LocalMaxima[i][j]=0;

            }
            else if((angle>135 && angle<=180)||(angle>315 && angle<=360)){

                yb1=j;xb1=i+1;yb2=j+1;xb2=i+1;
                yt1=j;xt1=i-1;yt2=j-1;xt2=i-1;
                double xest=fabs(py/px);
                double val1= xest*scalegradientImg[xt2][yt2]+(1-xest)*scalegradientImg[xt1][yt1];
                double val2= xest*scalegradientImg[xb2][yb2]+(1-xest)*scalegradientImg[xb1][yb1];
                gm=sqrt(fabs(gradientMagnitude-val1)*fabs(gradientMagnitude-val2));
                if(val1>gradientMagnitude || val2>gradientMagnitude) LocalMaxima[i][j]=0;
            }
            if(LocalMaxima[i][j]!=0){
                if((sqrt(fabs(gradientImage[i][j]-Minarr[i][j]))*(fabs(gradientMagnitude-LocalMinima[i][j])/(2*r)))<threshold) LocalMaxima[i][j]=0;
            }
        }
    }

    CImg<int> localmaximaimage(Xdim,Ydim,1,1);
    writeImage<double, int>(LocalMaxima,Xdim,Ydim,localmaximaimage,localmaximaimagename,true);
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
                if((sqrt(fabs(gradientImage[i][j]-Minarr[i][j])*(fabs(gradientMagnitude-LocalMinima[i][j])/(2*radius))))<threshold) LocalMaxima[i][j]=0;
            }
        }
    }
    CImg<int> localmaximaimage(Xdim,Ydim,1,1);
    writeImage<double, int>(LocalMaxima,Xdim,Ydim,localmaximaimage,localmaximaimagename,true);
}

void HysteresisThresholding(double MinThresh, double MaxThresh, string filename){
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
    writeImage<double, unsigned short>(Hysteresis,Xdim,Ydim,Hysteresisimage,filename,true);
}


#endif // EDGELOCALIZATION_H
