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

double Linear_interpolation(double** arr, double x, double y,double angle){
    int x1,y1,x2,y2,xo1,yo1,xo2,yo2,xo3,yo3,xo4,yo4;
    x1=ceil(x); y1=ceil(y);
    x2=floor(x);
    y2=floor(y);

    xo1=x1; yo1=y1; xo4=x2; yo4=y1;
    xo3=x2; yo3=y2; xo2=x1; yo2=y2;

    if(angle>360) angle=angle-360;

    double val=0;
    if((angle>=0 && angle<45)||(angle>225 && angle<=270)){
        xo1=x1; yo1=y1; xo4=x2; yo4=y1;
        xo3=x2; yo3=y2; xo2=x1; yo2=y2;

    }
    else if((angle>45 && angle<=90)||(angle>180 && angle<=225)){
        xo1=x2; yo1=y2;  xo2=x2;  yo2=y2-1;
        xo4=x2-1; yo4=y2;  xo3=x2-1;  yo3=y2-1;

    }
    else if((angle>90 && angle<=135) || (angle>315 && angle<=360)){
        xo4=x2; yo4=y2;  xo1=x1; yo1=y2;
        xo3=x2; yo3=y2-1;  xo2=x1;  yo2=y2-1;

    }
    else if((angle>135 && angle<=180)|| (angle>270 && angle<=315)){
        xo1=x2; yo1=y1;  xo2=x2; yo2=y2;
        xo3=x2-1; yo3=y2; xo4=x2-1; yo4=y1;

    }

    if(xo1<0) xo1=0; if(xo1>=Xdim) xo1=Xdim-1; if(yo1<0) yo1=0; if(yo1>=Ydim) yo1=Ydim-1;
    if(xo2<0) xo2=0; if(xo2>=Xdim) xo2=Xdim-1; if(yo2<0) yo2=0; if(yo2>=Ydim) yo2=Ydim-1;
    if(xo3<0) xo3=0; if(xo3>=Xdim) xo3=Xdim-1; if(yo3<0) yo3=0; if(yo3>=Ydim) yo3=Ydim-1;
    if(xo4<0) xo4=0; if(xo4>=Xdim) xo4=Xdim-1; if(yo4<0) yo4=0; if(yo4>=Ydim) yo4=Ydim-1;

    double fq11=arr[xo4][yo4];
    double fq12=arr[xo3][yo3];
    double fq22=arr[xo2][yo2];
    double fq21=arr[xo1][yo1];
    double coeff;
    if((xo2-xo3)*(yo1-yo2)==0) coeff=1;
    else coeff=1.0/((xo2-xo3)*(yo1-yo2));
    double num1=(y-yo2)*fq11*(xo1-x)+fq21*(x-xo4)*(y-yo2);
    double num2=(yo1-y)*(xo1-x)*fq12+(yo4-y)*(x-xo4)*fq22;
    val= (num1+num2)*coeff;
    if(coeff!=1) cout<<"Coefficient: "<<coeff<<"\n";
    if(val==0) val= arr[xo1][yo1];
    cout<<"Gradient Value: "<<val<<"\n";
    return val;
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
                if(r>basesig && (sqrt(fabs(gradientImage[i][j]-Minarr[i][j]))*(fabs(gradientMagnitude-LocalMinima[i][j])/(2*r)))<threshold) LocalMaxima[i][j]=0;
            }
        }
    }

    //CImg<int> localmaximaimage(Xdim,Ydim,1,1);
    //writeImage<double, int>(LocalMaxima,Xdim,Ydim,localmaximaimage,localmaximaimagename,true);
}

template<class GradType, class AngleType>
void SoftMaxComputation(GradType** scalegradientImg, AngleType** angleArray,GradType maximumval){
    for(int i=0;i<Xdim;i++){
        for(int j=0;j<Ydim;j++){
            double angle=angleArray[i][j]; //in degree
            double gradientMagnitude=scalegradientImg[i][j];
            double radius=(double)((double)ReliableScale[i][j]/2.0);
            double rad_angle=degtorad(angle);//convert to radian
            double start=0.5;
            softmax[i][j]=scalegradientImg[i][j];
            double max=INT_MIN;

            for(double r=-radius;r<=-0.5;r+=1){
                double xcoord=i-r*cos(rad_angle);
                double ycoord=j+r*sin(rad_angle);
                double interpolated_gradient_value=Bilinear_interpolation(scalegradientImg,xcoord,ycoord); //produces best result
                if(interpolated_gradient_value>max){max=interpolated_gradient_value;}
            }

            for(double r=0.5;r<=radius;r+=1){
                double xcoord=i-r*cos(rad_angle);
                double ycoord=j+r*sin(rad_angle);
                double interpolated_gradient_value=Bilinear_interpolation(scalegradientImg,xcoord,ycoord);
                if(interpolated_gradient_value>max){max=interpolated_gradient_value;}
            }

            double stdv=(max/2.0)>0?max/2.0:1;
            double softmean=max;
            double mu=colorgaussian(softmax[i][j],softmean,stdv);
            double sigval=1;//ComputeSigmoid(softmax[i][j],maximumval);//ComputeDecay(maximumval,softmax[i][j],1); 
            /*if(ReliableScale[i][j]==31){
            cout<<"Decay: "<<sigval<<",";
            cout<<"Max Value: "<<maximumval<<" , Scale: "<<ReliableScale[i][j]<<"\n";
            }*/
            softmax[i][j]=softmax[i][j]*mu*sigval;
            ScaleGradientNewX[i][j]=ScaleGradientX[i][j]*mu*sigval;
            ScaleGradientNewY[i][j]=ScaleGradientY[i][j]*mu*sigval;
        }
    }
    CImg<int> softlocalmaximaimage(Xdim,Ydim,1,1);
    writeImage<double, int>(softmax,Xdim,Ydim,softlocalmaximaimage,softlocalmaximaimagename);
}

template<class GradType, class AngleType>
void NonmaximaSuppression(GradType** scalegradientImg, AngleType** angleArray,double threshold,double threshold2){
    for(int i=0;i<Xdim;i++){
        for(int j=0;j<Ydim;j++){
            double angle=angleArray[i][j]; //in degree
            double gradientMagnitude=scalegradientImg[i][j];
            double radius=(double)((double)ReliableScale[i][j]/2.0);
            //double radius=(double)((double)MaximizedScale[i][j]/2.0);
            double rad_angle=degtorad(angle);//convert to radian
            double start=0.5;
            LocalMaxima[i][j]=scalegradientImg[i][j];
            bool localMaxima=true;
            double min1=gradientMagnitude;
            double min2=gradientMagnitude;

            for(double r=-radius;r<=-0.5;r+=1){
                double xcoord=i-r*cos(rad_angle);
                double ycoord=j+r*sin(rad_angle);
                double interpolated_gradient_value=Bilinear_interpolation(scalegradientImg,xcoord,ycoord); //produces best result
                //double interpolated_gradient_value=Linear_interpolation(scalegradientImg,xcoord,ycoord,angle+180);
                if(interpolated_gradient_value>gradientMagnitude){localMaxima=false; break;}
                if(min1>interpolated_gradient_value)min1=interpolated_gradient_value;
            }

            if(localMaxima){
                for(double r=0.5;r<=radius;r+=1){
                    double xcoord=i-r*cos(rad_angle);
                    double ycoord=j+r*sin(rad_angle);
                    double interpolated_gradient_value=Bilinear_interpolation(scalegradientImg,xcoord,ycoord); //produces best result
                    //double interpolated_gradient_value=Linear_interpolation(scalegradientImg,xcoord,ycoord,angle);
                    if(interpolated_gradient_value>gradientMagnitude){localMaxima=false; break;}
                    if(min2>interpolated_gradient_value)min2=interpolated_gradient_value;
                }
            }
            if(!localMaxima) LocalMaxima[i][j]=0;
            else if(radius>basesig && (Dividorarr[i][j]/radius)<threshold || ((sqrt(fabs(gradientImage[i][j]-Minarr[i][j])*(fabs(gradientMagnitude-LocalMinima[i][j])/(2*radius))))<threshold2))
                //cout<<"Here\n";    
		        LocalMaxima[i][j]=0;
            
            //}
            /*else{
                if((sqrt(fabs(gradientImage[i][j]-min1)*(fabs(gradientMagnitude-min2))))<threshold)
                    LocalMaxima[i][j]=0;
                
            }*/
        }
    }
    CImg<int> localmaximaimage(Xdim,Ydim,1,1);
    writeImage<double, int>(LocalMaxima,Xdim,Ydim,localmaximaimage,localmaximaimagename);
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
