#ifndef CONTINUOUSSCALECOMPUTATION_H
#define CONTINUOUSSCALECOMPUTATION_H
#include <iostream>
#include<math.h>
#include <cmath>
#include <algorithm>
#include <fstream>
#include "MathFunctions.h"
#include "Global.h"
#include "Commons.h"
#include "ComputeGradient.h"
#include "edgelocalization.h"
using namespace std;

int percent=10;
void init_scale_params(){
    pointsunderarea=1;
    Totalweight=1;
    gradientvar=0;
    TotalweightX=TotalweightY=0;
    curgradX=curgradY=0;

}

void drawCirclePoints(double** arr, int r, int xc, int yc, int curx, int cury, int Rows, int Cols)
{

    if(xc - curx >= 0 && yc - cury >= 0 /*&& ReliableScale[xc-curx][yc-cury]>arr[xc][yc]*/ ) {
        percentile.push_back( arr[xc-curx][yc-cury]);
        scalearr[scalecount++]=arr[xc-curx][yc-cury];
    }
    if(xc - curx >= 0 && yc + cury < Cols /*&& ReliableScale[xc - curx][yc + cury]>arr[xc][yc]*/) {
        percentile.push_back( arr[xc - curx][yc + cury]);
        scalearr[scalecount++]=arr[xc - curx][yc + cury];
    }
    if(xc + curx < Rows && yc - cury >= 0 /*&& ReliableScale[xc + curx][yc - cury]>arr[xc][yc]*/) {
        percentile.push_back(arr[xc + curx][yc - cury]);
        scalearr[scalecount++]=arr[xc + curx][yc - cury];
    }
    if(yc + cury < Cols && xc + curx < Rows /*&& ReliableScale[xc + curx][yc + cury]>arr[xc][yc]*/) {
        percentile.push_back(arr[xc + curx][yc + cury]);
        scalearr[scalecount++]=arr[xc + curx][yc + cury];
    }
    if(xc - cury >= 0 && yc - curx >= 0 /*&& ReliableScale[xc - cury][yc - curx]>arr[xc][yc]*/ ) {
        percentile.push_back(arr[xc - cury][yc - curx]);
        scalearr[scalecount++]=arr[xc - cury][yc - curx];
    }
    if(yc - curx >= 0 && xc + cury < Rows /*&& ReliableScale[xc + cury][yc - curx]>arr[xc][yc]*/ ) {
        percentile.push_back(arr[xc + cury][yc - curx]);
        scalearr[scalecount++]=arr[xc + cury][yc - curx];
    }
    if(xc + cury < Rows && yc + curx < Cols /*&& ReliableScale[xc + cury][yc + curx]>arr[xc][yc]*/) {
        percentile.push_back(arr[xc + cury][yc + curx]);
        scalearr[scalecount++]=arr[xc + cury][yc + curx];
    }
    if(xc - cury >= 0 && yc + curx < Cols /*&& ReliableScale[xc - cury][yc + curx]>arr[xc][yc]*/) {
        percentile.push_back(arr[xc - cury][yc + curx]);
        scalearr[scalecount++]=arr[xc - cury][yc + curx];
    }
}

void drawCircularPixels(double** arr, int r, int xc, int yc, int curx, int cury, int Rows, int Cols)
{

    if(xc - curx >= 0 && yc - cury >= 0 ) {
        if(MaximizedScale[xc-curx][yc-cury]>arr[xc][yc]) MaximizedScale[xc-curx][yc-cury]=arr[xc][yc];
    }
    if(xc - curx >= 0 && yc + cury < Cols ) {
        if (MaximizedScale[xc-curx][yc-cury]>arr[xc][yc]) MaximizedScale[xc-curx][yc-cury]=arr[xc][yc];
    }
    if(xc + curx < Rows && yc - cury >= 0 ) {
        if(MaximizedScale[xc + curx][yc - cury]>arr[xc][yc]) MaximizedScale[xc + curx][yc - cury]=arr[xc][yc];
    }
    if(yc + cury < Cols && xc + curx < Rows ) {
        if(MaximizedScale[xc + curx][yc + cury]>arr[xc][yc])MaximizedScale[xc + curx][yc + cury]=arr[xc][yc];
    }
    if(xc - cury >= 0 && yc - curx >= 0 ) {
        if(MaximizedScale[xc - cury][yc - curx]>arr[xc][yc]) MaximizedScale[xc - cury][yc - curx]=arr[xc][yc];
    }
    if(yc - curx >= 0 && xc + cury < Rows ) {
        if(MaximizedScale[xc + cury][yc - curx]>arr[xc][yc]) MaximizedScale[xc + cury][yc - curx]=arr[xc][yc];
    }
    if(xc + cury < Rows && yc + curx < Cols ) {
        if(MaximizedScale[xc + cury][yc + curx]>arr[xc][yc]) MaximizedScale[xc + cury][yc + curx]=arr[xc][yc];
    }
    if(xc - cury >= 0 && yc + curx < Cols ) {
        if(MaximizedScale[xc - cury][yc + curx]>arr[xc][yc])MaximizedScale[xc - cury][yc + curx]=arr[xc][yc];
    }
}

void drawScale(double** arr, int xc, int yc, int Rows, int Cols, int r)
{
    int curx = 0;
    int cury = r;
    int p = 3 - r * 2;
    drawCircularPixels(arr, r, xc, yc, curx, cury, Rows, Cols);
    while(curx < cury) {
        curx++;
        if(p < 0){
            p += 4 * curx + 6;
        } else {
            p += 4 * (curx - cury) + 10;
            cury--;
        }
        drawCircularPixels(arr, r, xc, yc, curx, cury, Rows, Cols);
    }
}

void drawCircle(double** arr, int xc, int yc, int Rows, int Cols, int r)
{
    int curx = 0;
    int cury = r;
    int p = 3 - r * 2;
    drawCirclePoints(arr, r, xc, yc, curx, cury, Rows, Cols);
    while(curx < cury) {
        curx++;
        if(p < 0) {
            p += 4 * curx + 6;
        } else {
            p += 4 * (curx - cury) + 10;
            cury--;
        }
        drawCirclePoints(arr, r, xc, yc, curx, cury, Rows, Cols);
    }
}

void OptimizeScale(){
    for(int i=0;i<Xdim;i++){
        for(int j=0;j<Ydim;j++){
            MaximizedScale[i][j]= ReliableScale[i][j];
        }
    }
    for(int i=0;i<Xdim;i++){
        for(int j=0;j<Ydim;j++){
            ncount=0;
            percentile.clear();
            int r=(int)(ReliableScale[i][j]/2);
            drawScale(ReliableScale,i,j,Xdim,Ydim,r);
        }
    }
}

void MaxmizeScale(){

    for(int i=0;i<Xdim;i++){
        for(int j=0;j<Ydim;j++){
            ncount=0;
            percentile.clear();
            int r=(int)(ReliableScale[i][j]/2);
            drawCircle(ReliableScale,i,j,Xdim,Ydim,r);
            sort(percentile.begin(),percentile.end());
            int index=percentile.size();
            MaximizedScale[i][j]= percentile[index-1];

        }
    }
}


void DialateScale(int prcntile=25){
    for(int i=0;i<Xdim;i++){
        for(int j=0;j<Ydim;j++){
            ncount=0;
            //for(int k=1;k<(ReliableScale[i][j]-1)/2;k++)
            percentile.clear();
            int r=(int)(ReliableScale[i][j]/2);
            drawCircle(gradImage,i,j,Xdim,Ydim,r);
            sort(percentile.begin(),percentile.end());
            int index=(prcntile)*percentile.size();
            index/=100;
            LocalMinima[i][j]= percentile[index];
        }
    }
}

void FinaOptimalScaleforEdge(double** arr, int rows, int cols){
    for(int i=0;i<Xdim;i++){
        for (int j=0;j<Ydim;j++){
            double stdv=0.5;
            double gradthreshold=arr[i][j];
            double curgradient=-1;
            //cout<<" gradient threshold: "<<gradthreshold<<"\n";
            while(curgradient<gradthreshold){
                stdv*=2;
                int ksize=(6*stdv);
                if(ksize%2==0) ksize+=1;
                gaussgradvar=stdv;
                initializeKernels(ksize);
                computegaussGradient(image,i,j,rows, cols,0);
                gradientImage[i][j]=squareroot(GradientX[i][j],GradientY[i][j]);
                deallocateMemory<double>(kernelX,ksize,ksize);
                deallocateMemory<double>(kernelY,ksize,ksize);
                //cout<<" curgradient: "<<gradientImage[i][j]<<" gradient threshold: "<<gradthreshold<<"\n";
                curgradient = gradientImage[i][j]*ReliableScale[i][j]*0.5;
                if(stdv>=maxscale) break;
            }
            //cout<<"Processed Pixel: "<<i<<" , "<<j<<" Scale: "<<stdv<<endl;
            optimalstdv[i][j]=stdv;
        }
    }
}

void VarianceofScale(){
    for(int i=0;i<Xdim;i++){
        for(int j=0;j<Ydim;j++){
            scalearr=new double[400];
            percentile.clear();
            scalecount=0;
            int r=(int)(ReliableScale[i][j]/2);
            drawCircle(gradImage,i,j,Xdim,Ydim,r);
            if(scalecount==0) scalecount=1;
            double mean=0;
            for(int k=0;k<scalecount;k++){
                mean+=scalearr[k];
            }
            mean/=scalecount;
            double var=0;
            for(int k=0;k<scalecount;k++){
                var+=pow((scalearr[k]-mean),2);
            }

            var/=scalecount;

            Dividorarr[i][j]=var;
            delete scalearr;
        }
    }
}

void smoothGradient(double** arr, int Rows, int Cols){
    for(int i=1;i<Rows-1;i++){
        for(int j=1;j<Cols-1;j++){
            double val=0;
            for(int x=-1;x<=1;x++){
                for(int y=-1;y<=1;y++){
                    val+=arr[x+i][y+j];
                }
            }
            smoothgradImage[i][j]=val/9.0;
        }
    }

}

double getInterpolatedGradientX(double x, double y, int Rows, int Cols, int index){
    int x1,y1,x2,y2,x3,y3,x4,y4;
    double averageweight_X=0;
    x1=ceil(x); y1=ceil(y); //ceil(x), ceil(Y) -- Point 1 for interpolation
    y2=floor(y); //ceil(x),floor(y) -- Point 2 for interpolation
    x3=floor(x);y3=ceil(y); //floor(x),ceil(y) -- Point 3 for interpolation
    y4=floor(y); //floor(x),floor(y) -- Point 4 for interpolation

    int pcount=0;
    if(x1>=0 && x1<Rows && y1>=0 && y1<Cols){averageweight_X+=GradientX[x1][y1];++pcount;}
    if(x1>=0 && x1<Rows && y2>=0 && y2<Cols && index!=0 ){averageweight_X+=GradientX[x1][y2];++pcount;}
    if(x3>=0 && x3<Rows && y3>=0 && y3<Cols && index!=2 ){averageweight_X+=GradientX[x3][y3];++pcount;}
    if(x3>=0 && x3<Rows && y4>=0 && y4<Cols){averageweight_X+=GradientX[x3][y4];++pcount;}
    if(pcount==0) return averageweight_X;
    return averageweight_X/pcount;
}

double getInterpolatedGradientY(double x, double y, int Rows, int Cols, int index){
    int x1,y1,x2,y2,x3,y3,x4,y4;
    double averageweight_Y=0;
    x1=ceil(x); y1=ceil(y); //ceil(x), ceil(Y) -- Point 1 for interpolation
    x2=ceil(x);y2=floor(y); //ceil(x),floor(y) -- Point 2 for interpolation
    x3=floor(x);y3=ceil(y); //floor(x),ceil(y) -- Point 3 for interpolation
    x4=floor(x);y4=floor(y); //floor(x),floor(y) -- Point 4 for interpolation
    int pcount=0;
    if(x1>=0 && x1<Rows && y1>=0 && y1<Cols){averageweight_Y+=GradientY[x1][y1];++pcount;}
    if(x2>=0 && x2<Rows && y2>=0 && y2<Cols && index!=0 ){averageweight_Y+=GradientY[x2][y2];++pcount;}
    if(x3>=0 && x3<Rows && y3>=0 && y3<Cols && index!=2 ){averageweight_Y+=GradientY[x3][y3];++pcount;}
    if(x4>=0 && x4<Rows && y4>=0 && y4<Cols){averageweight_Y+=GradientY[x4][y4];++pcount;}
    if(pcount==0) return averageweight_Y;
    return averageweight_Y/pcount;
}

double Bilinear_interpolationX( double x, double y){
    int x1,y1,x2,y2;
    x1=ceil(x); y1=ceil(y);
    x2=floor(x);
    y2=floor(y);
    if(x1<0) x1=0; if(x1>=Xdim) x1=Xdim-1;
    if(y1<0) y1=0; if(y1>=Ydim) y1=Ydim-1;
    if(x2<0) x2=0; if(x2>=Xdim) x2=Xdim-1;
    if(y2<0) y2=0; if(y2>=Ydim) y2=Ydim-1;
    double fq11=GradientX[x2][y1];
    double fq12=GradientX[x2][y2];
    double fq22=GradientX[x1][y2];
    double fq21=GradientX[x1][y1];
    double coeff;
    if((x1-x2)*(y1-y2)==0) coeff=1;
    else coeff=1.0/((x1-x2)*(y1-y2));
    double num1=(y-y2)*fq11*(x1-x)+fq21*(x-x2)*(y-y2);
    double num2=(y1-y)*(x1-x)*fq12+(y1-y)*(x-x2)*fq22;
    double val= (num1+num2)*coeff;
    if(coeff!=1) cout<<"Coefficient: "<<coeff<<"\n"; //should be one if calculation is correct
    if(val==0) return GradientX[x1][y1];
    else return val;
}

double Bilinear_interpolationY( double x, double y){
    int x1,y1,x2,y2;
    x1=ceil(x); y1=ceil(y);
    x2=floor(x);
    y2=floor(y);
    if(x1<0) x1=0; if(x1>=Xdim) x1=Xdim-1;
    if(y1<0) y1=0; if(y1>=Ydim) y1=Ydim-1;
    if(x2<0) x2=0; if(x2>=Xdim) x2=Xdim-1;
    if(y2<0) y2=0; if(y2>=Ydim) y2=Ydim-1;
    double fq11=GradientY[x2][y1];
    double fq12=GradientY[x2][y2];
    double fq22=GradientY[x1][y2];
    double fq21=GradientY[x1][y1];
    double coeff;
    if((x1-x2)*(y1-y2)==0) coeff=1;
    else coeff=1.0/((x1-x2)*(y1-y2));
    double num1=(y-y2)*fq11*(x1-x)+fq21*(x-x2)*(y-y2);
    double num2=(y1-y)*(x1-x)*fq12+(y1-y)*(x-x2)*fq22;
    double val= (num1+num2)*coeff;
    if(coeff!=1) cout<<"Coefficient: "<<coeff<<"\n";
    if(val==0) return GradientY[x1][y1];
    else return val;
}

void updateParameters(int r,double weight,int index){
    pointsunderarea+=1;
    TotalweightX+=(weight*weight_gradient_matrix[r][totalpoints][index][0]);
    TotalweightY+=( weight*weight_gradient_matrix[r][totalpoints][index][1]);
    curgradX+=(weight*weight_gradient_matrix[r][totalpoints][index][0]);
    curgradY+=(weight*weight_gradient_matrix[r][totalpoints][index][1]);
    Totalweight+=weight;
}


void draw_continuous_scale(unsigned short** arr, int xc, int yc, int Rows, int Cols, double r){
    double curx0,curx1,curx2;
    double cury0,cury1,cury2;
    double total_sample_points=r*sampling_rate;

    for(int i=0;i<total_sample_points;i++){
        double angle=(2*M_PI*i)/(total_sample_points);

        curx0=xc+(r-0.25)*cos(angle);
        cury0=yc-(r-0.25)*sin(angle);
        curx1=xc+r*cos(angle);
        cury1=yc-r*sin(angle);
        curx2=xc+(r+0.25)*cos(angle);
        cury2=yc-(r+0.25)*sin(angle);
        int scale=2*r;
        weight_gradient_matrix[scale][totalpoints][0][0]=Bilinear_interpolationX(curx0,cury0);
        weight_gradient_matrix[scale][totalpoints][0][1]=Bilinear_interpolationY(curx0,cury0);
        updateParameters(scale,0.5,0);

        weight_gradient_matrix[scale][totalpoints][1][0]=Bilinear_interpolationX(curx1,cury1);
        weight_gradient_matrix[scale][totalpoints][1][1]=Bilinear_interpolationY(curx1,cury1);
        updateParameters(scale,1,1);

        weight_gradient_matrix[scale][totalpoints][2][0]=Bilinear_interpolationX(curx2,cury2);
        weight_gradient_matrix[scale][totalpoints][2][1]=Bilinear_interpolationY(curx2,cury2);
        updateParameters(scale,0.5,2);

        double gradXcomp=0.5*weight_gradient_matrix[scale][totalpoints][0][0]+weight_gradient_matrix[scale][totalpoints][1][0]
                +0.5*weight_gradient_matrix[scale][totalpoints][2][0];
        double gradYcomp=0.5*weight_gradient_matrix[scale][totalpoints][0][1]+weight_gradient_matrix[scale][totalpoints][1][1]
                +0.5*weight_gradient_matrix[scale][totalpoints][2][1];
        double Gradmag=squareroot<double>(gradXcomp/2,gradYcomp/2);
        percentile.push_back(Gradmag);
        //curgrad+=Gradmag;
        if(Gradmag<minimumGradient) minimumGradient=Gradmag;

        if(xc==Xc && yc==Yc){
            double avgx=gradXcomp/2.0;
            double avgy=gradYcomp/2.0;
            sout<<xc<<" "<<yc<<" "<<avgx<<" "<<avgy<<" "<<scale<<endl;
        }

        totalpoints+=1;
    }
}

void compute_scale_Gradient_with_continuous_interpolation(unsigned short** arr,int Rows, int Cols,bool usigned=false,bool isprint=false){
    ofstream fout;
    //string fname=basepath+"scaleList.txt";
    //fout.open(fname.c_str(),ios::app);
    maxgradient=INT_MIN;
    for(int i=0;i<Rows;i++){
        for(int j=0;j<Cols;j++){
            init_scale_params();
            double scale=0;
            double gradthreshold=1;
            totalpoints=1;
            double prevcount=1;
            TotalweightX=curgradX=GradientX[i][j];
            TotalweightY=curgradY=GradientY[i][j];
            globalmeanX=TotalweightX/Totalweight;
            globalmeanY=TotalweightY/Totalweight;
            totalgradient1=totalgradient=squareroot<double>(curgradX,curgradY);
            double signal_to_noise_ratio=1;
            Minarr[i][j]=0;
            while((gradientvar<gradthreshold) || (totalgradient)<gradienthresh){
                percentile.clear();
                minimumGradient=INT_MAX;
                curgradX=curgradY=0;curgrad=0;
                prevcount=pointsunderarea;
                scale=scale+0.5;
                totalpoints=0;
                ScaleGradientX[i][j]=TotalweightX/2.0;
                ScaleGradientY[i][j]=TotalweightY/2.0;
                gradImage[i][j]=totalgradient1;
                ReliableScale[i][j]=scale*2;
                if(scale>maxscale) break;
                draw_continuous_scale(arr,i,j,Rows,Cols,scale);
                Minarr[i][j]=GetLowerPercentile(percentile,percent); //also compute the 50%-ile for scalebased gradientimage
                int flag=1;
                if(2*scale<=6) flag=0;

                double varx=compute_variance_X_from_inner(2*scale,flag);
                double vary=compute_variance_Y_from_inner(2*scale,flag);
                gradientvar=varx+vary;
                gradthreshold=compute_threshold(totalpoints);
                totalgradient1=squareroot<double>(TotalweightX/2,TotalweightY/2);
                totalgradient=curgrad/(snr*totalpoints);
                gradienthresh/=sqrt(totalpoints);
            }


            if(scale==0.5) {gradImage[i][j]/=(3);gradImage[i][j]*=(2*scale);}
            else { gradImage[i][j]/=(prevcount); gradImage[i][j]*=(2*scale);}

            //if(scale==maxscale) gradImage[i][j]/=(2*scale);
            //gradImage[i][j]+=gradientImage[i][j];
            if(gradImage[i][j]>maxgradient) maxgradient=gradImage[i][j];
            Gradientangle[i][j]=computeAngle(ScaleGradientX[i][j],ScaleGradientY[i][j]);
        }
    }
    cout<<"Max Gradient: "<<maxgradient<<endl;
    gradient_color_Image(Gradientangle,gradImage,gradientcolorimagename,false);
    CImg<unsigned short> scalejpgimage(Xdim,Ydim,1,1);
    writeImage<double, unsigned short>(ReliableScale,Rows,Cols,scalejpgimage,scaleimagename,false); //use 4

    smoothGradient(gradImage,Xdim,Ydim); //uncomment
    //cout<<"Gradient Smoothed\n";

    CImg<long int> gradientjpgimage(Xdim,Ydim,1,1);
    writeImage<double, long int>(gradImage,Rows,Cols,gradientjpgimage,gradientimagename,false); //use 5



    int flag=1;
    double maxThreshold,minThreshold,threshold,threshold2;

    cout<<"*********************************\n";
    cout<<"Localization using Canny's Method\n";
    cout<<"*********************************\n";
    while(flag==1){
        cout<<"Enter Lower and Higher Hysteresis Threshold: ";
        cin>>minThreshold>>maxThreshold;
        //cout<<"Enter Variance threshhold: ";
        //cin>>threshold;
        //cout<<"Enter Non maxima suppression threshold: ";
        //cin>>threshold2;
        threshold=0;threshold2=0;
        SoftMaxComputation<double,double>(smoothgradImage,Gradientangle,maxgradient); //if using smoothing use smoothgradImage else use gradImage
        gradient_color_Image(Gradientangle,softmax,dialatedimagename,false);
        NonmaximaSuppression_Canny<double,double>(softmax,ScaleGradientNewX,ScaleGradientNewY,Gradientangle,threshold2);
        HysteresisThresholding(minThreshold,maxThreshold,Hysteresisimagename);
        cout<<"Press 0 to terminate else press 1. \n";
        cin>>flag;
    }
    //fout.close();
}

#endif // CONTINUOUSSCALECOMPUTATION_H
