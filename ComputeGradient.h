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

void PrecomputeKernels(int maxstdv){
    for(int k=1;k<2*maxstdv+2;k++){
        double stdv=(double)k/2.0;
        int ksize=(6*stdv);
        if(ksize%2==0) ksize+=1;
        gaussgradvar=stdv;
        initializeKernels(ksize);
        int kernelindex=k;

        int kmid=ksize/2;
        int mid=maxkernelsize/2;

        for(int i=-kmid;i<=kmid;i++){
            for(int j=-kmid;j<=kmid;j++){
                PrecomputedKernelsX[kernelindex][mid+i][mid+j]=kernelX[kmid+i][kmid+j];
                PrecomputedKernelsY[kernelindex][mid+i][mid+j]=kernelY[kmid+i][kmid+j];
            }
        }

        deallocateMemory<double>(kernelX,ksize,ksize);
        deallocateMemory<double>(kernelY,ksize,ksize);

    }
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

void compute_Gradient(unsigned short** arr,int Rows, int Cols, bool usigned=false,bool writeimage=true){
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
    if(usigned && writeimage){
        CImg<unsigned short> scalejpgimage(Xdim,Ydim,1,1);
        writeImageunsigned< double, unsigned short>(gradientImage,Rows,Cols,scalejpgimage,gaussgradientimagename);
        //writeImagefile< unsigned short>(gradientImage,Rows,Cols,gaussgradientimagename);
        gradient_color_Image(Gradientangle,gradientImage,gausscolorimagename);
    }
    else if(!usigned && writeimage){
        CImg<unsigned short> scalejpgimage(Xdim,Ydim,1,1);
        writeImage< double, unsigned short>(gradientImage,Rows,Cols,scalejpgimage,gaussgradientimagename);
        gradient_color_Image(Gradientangle,gradientImage,gausscolorimagename);
    }

}

void computeScaleBasedGradient(unsigned short** arr){
    for(int x=0;x<Xdim;x++){
        for(int y=0;y<Ydim;y++){
            int Scale=MaximizedScale[x][y];
            //cout<<"Scale : "<<Scale<<"\n";
            int ks=(Scale*3);
            if(ks%2==0) ks+=1;
            int loopcnts=ks/2;
            int mid=maxkernelsize/2;
            double GradX=0.0;
            for(int i=-loopcnts;i<=loopcnts;i++){
                for(int j=-loopcnts;j<=loopcnts;j++){
                    if(x+i>=0&&y+j>=0&&x+i<Xdim&&y+j<Ydim){
                        GradX=GradX+(((double)arr[x+i][y+j])*PrecomputedKernelsX[Scale][i+mid][j+mid]);
                    }
                }
            }
            double GradY=0.0;
            for(int i=-loopcnts;i<=loopcnts;i++){
                for(int j=-loopcnts;j<=loopcnts;j++){
                    if(x+i>=0&&y+j>=0&&x+i<Xdim&&y+j<Ydim){
                        GradY=GradY+(((double)arr[x+i][y+j])*PrecomputedKernelsY[Scale][i+mid][j+mid]);
                    }
                }
            }
            GradientX[x][y]=GradX;
            GradientY[x][y]=GradY;
            gradImage[x][y]=squareroot<double>(GradX,GradY);
            Gradientangle[x][y]=computeAngle(GradX,GradY);
            //cout<<"Computation Complete\n";
        }
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
