#ifndef COMPUTEGRADIENT_H
#define COMPUTEGRADIENT_H
#include <iostream>
#include "Global.h"
#include "Commons.h"
#include <math.h>
#include <cmath>
#include "MathFunctions.h"

using namespace std;

int loopcnts;

//initializing both gaussian first order filters.
void initializeKernels(int dim){
    kernelX=new double*[dim];
    kernelY=new double*[dim];
    for(int i=0;i<dim;i++){
        kernelX[i]=new double[dim];
        kernelY[i]=new double[dim];
        for(int j=0;j<dim;j++){
            kernelX[i][j]=0;
            kernelY[i][j]=0;
        }
    }
    
    loopcnts=dim/2;
    double sumX=0.0;
    double sumY=0.0;

    for(int i=-loopcnts;i<=loopcnts;i++){
        for(int j=-loopcnts;j<=loopcnts;j++){
            double dist=CartesianDistance2D(0,0,i,j);
            kernelX[loopcnts+i][loopcnts+j]=gauss(dist,mean,gaussgradvar,i);
            kernelY[loopcnts+i][loopcnts+j]=gauss(dist,mean,gaussgradvar,j);
            sumX+=fabs(kernelX[loopcnts+i][loopcnts+j]);
            sumY+=fabs(kernelY[loopcnts+i][loopcnts+j]);
        }
    }
    double sumx=0;
    double sumy=0;
    for(int i=-loopcnts;i<=loopcnts;i++){
        for(int j=-loopcnts;j<=loopcnts;j++){
            kernelX[loopcnts+i][loopcnts+j]/=(0.5*sumX);
            kernelY[loopcnts+i][loopcnts+j]/=(0.5*sumY);
            sumx+=fabs(kernelX[loopcnts+i][loopcnts+j]);
            sumy+=fabs(kernelY[loopcnts+i][loopcnts+j]);
        }
    }

    //cout<<"KernelX sum: "<<sumx<<" KernelY sum: "<<sumy<<endl;

}


double computeGradientX(unsigned short** arr, int x, int y,int Rows, int Cols, int r){
    double GradX=0.0;
    for(int i=-loopcnts;i<=loopcnts;i++){
        for(int j=-loopcnts;j<=loopcnts;j++){
            if(x+i>=0&&y+j>=0&&x+i<Rows&&y+j<Cols){
                GradX=GradX+(((double)arr[x+i][y+j])*kernelX[i+loopcnts][j+loopcnts]);
            }
        }
    }
    return GradX;
}

double computeGradientY(unsigned short** arr, int x, int y, int Rows, int Cols, int r){
    double GradY=0.0;
    for(int j=-loopcnts;j<=loopcnts;j++){
        for(int i=-loopcnts;i<=loopcnts;i++){
            if(x+i>=0&&y+j>=0&&x+i<Rows&&y+j<Cols){
                GradY=GradY+(((double)arr[x+i][y+j])*kernelY[i+loopcnts][j+loopcnts]);
            }
        }
    }
    return GradY;
}

void computegaussGradient(unsigned short** arr, int x, int y,int Rows, int Cols,int r){
    double gradX=computeGradientX(arr,x,y,Rows,Cols,r);
    double gradY=computeGradientY(arr,x,y,Rows,Cols,r);
    GradientX[x][y]=gradX;
    GradientY[x][y]=gradY;
    isComputed[x][y]=true;
}

void compute_Gradient(unsigned short** arr,int Rows, int Cols, bool usigned=false){
    for(int i=0;i<Rows;i++){
        for(int j=0;j<Cols;j++){
            if(arr[i][j]>=minIntensity ){
                computegaussGradient(arr,i,j,Rows,Cols,0);
                gradientImage[i][j]=squareroot(GradientX[i][j],GradientY[i][j]);
                Gradientangle[i][j]=computeAngle(GradientX[i][j],GradientY[i][j]);
               // cout<<gradientImage[i][j]<<endl;
            }
        }
    }
    if(usigned){
        CImg<unsigned short> scalejpgimage(Xdim,Ydim,1,1);
        writeImageunsigned< double, unsigned short>(gradientImage,Rows,Cols,scalejpgimage,gaussgradientimagename);
        //writeImagefile< unsigned short>(gradientImage,Rows,Cols,gaussgradientimagename);
        gradient_color_Image(Gradientangle,gradientImage,gausscolorimagename);
    }
    else{
        CImg<unsigned short> scalejpgimage(Xdim,Ydim,1,1);
        writeImage< double, unsigned short>(gradientImage,Rows,Cols,scalejpgimage,gaussgradientimagename);
        gradient_color_Image(Gradientangle,gradientImage,gausscolorimagename);
    }

}

void print_Kernel(){
    cout<<"Kernel Matrix ====>\n";
    cout<<"-----------------------------------\n";
    cout<<"\n";
    for(int i=-loopcnts;i<=loopcnts;i++){
        for(int j=-loopcnts;j<=loopcnts;j++){
            cout<<kernelX[i+loopcnts][j+loopcnts]<<" ";
        }
        cout<<"\n";
    }
    cout<<"-----------------------------------\n";
    cout<<"\n";

    for(int i=-loopcnts;i<=loopcnts;i++){
        for(int j=-loopcnts;j<=loopcnts;j++){
            cout<<kernelY[i+loopcnts][j+loopcnts]<<" ";
        }
        cout<<"\n";
    }
    cout<<"-----------------------------------\n";
}

#endif // COMPUTEGRADIENT_H
