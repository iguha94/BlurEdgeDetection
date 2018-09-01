#ifndef COMMONS_H
#define COMMONS_H
#include<iostream>
#include "Global.h"
#include "MathFunctions.h"
#include <fstream>
#include "CImg.h"
#include <string>
#include <fstream>
#include <math.h>
using namespace std;
using namespace cimg_library;

long long int calculateIndex(int x, int y,int Rows=0,int Cols=0){
    return x*Cols+y;
}

double** allocatearr2D(double** arr){
    arr=new double*[2];
     for(int i=0;i<2;i++){
         arr[i]=new double[2];
         for(int j=0;j<2;j++){
             arr[i][j]=0;
         }
     }
     return arr;
}

void deallocategradImageMemory(){
    GradientX=NULL;
    GradientY=NULL;
    kernelX=kernelY=NULL;
    ReliableScale=NULL;
    gradientImage=NULL;
}

void deallocateKernels(){
    kernelX=NULL;
    kernelY=NULL;
}

void allocateGradientImageMemory(int Rows, int Cols){
    deallocategradImageMemory();
    ReliableScale=new double*[Rows];
    gradientImage=new double*[Rows];
    isComputed=new bool*[Rows];
    LocalMaxima=new double*[Rows];
    LocalMinima=new double*[Rows];
    Hysteresis=new double*[Rows];
    dtImage=new unsigned short*[Rows];
    skelImage=new unsigned short*[Rows];
    prunnedskelImage=new unsigned short*[Rows];
    thresholdedimg=new unsigned short*[Rows];
    secondderivativeimg=new double*[Rows];
    DialatedScale=new unsigned short*[Rows];
    Dividorarr=new double*[Rows];
    countarr=new int*[Rows];
    ScaleGradientX=new double*[Rows];
    ScaleGradientY=new double*[Rows];
    Minarr=new double*[Rows];
    for(int i=0;i<Rows;i++){
        ReliableScale[i]=new double[Cols];
        gradientImage[i]=new double[Cols];
        isComputed[i]=new bool[Cols];
        LocalMaxima[i]=new double[Cols];
        LocalMinima[i]=new double[Cols];
        Hysteresis[i]=new double[Cols];
        dtImage[i]=new unsigned short[Cols];
        skelImage[i]=new unsigned short[Cols];
        prunnedskelImage[i]=new unsigned short[Cols];
        thresholdedimg[i]=new unsigned short[Cols];
        secondderivativeimg[i]=new double[Cols];
        DialatedScale[i]=new unsigned short[Cols];
        Dividorarr[i]=new double[Cols];
        countarr[i]=new int[Cols];
        ScaleGradientX[i]=new double[Cols];
        ScaleGradientY[i]=new double[Cols];
        Minarr[i]=new double[Cols];
        for(int j=0;j<Cols;j++){
            ReliableScale[i][j]=0;
            gradientImage[i][j]=0;
            isComputed[i][j]=false;
            LocalMaxima[i][j]=0;
            Hysteresis[i][j]=0;
            dtImage[i][j]=0;
            skelImage[i][j]=0;
            prunnedskelImage[i][j]=0;
            LocalMinima[i][j]=0;
            DialatedScale[i][j]=0;
            Dividorarr[i][j]=0;
            countarr[i][j]=0;
            ScaleGradientX[i][j]=0;
            ScaleGradientY[i][j]=0;
            Minarr[i][j]=0;
        }
    }

    GradientX=new double*[Rows];
    GradientY=new double*[Rows];
    Gradientangle=new double*[Rows];
    SGradientangle=new double*[Rows];
    for(int i=0;i<Rows;i++){
        GradientX[i]=new double[Cols];
        GradientY[i]=new double[Cols];
        Gradientangle[i]=new double[Cols];
        SGradientangle[i]=new double[Cols];
        for(int j=0;j<Cols;j++){
            GradientX[i][j]=GradientY[i][j]=0;
            Gradientangle[i][j]=0;
            SGradientangle[i][j]=0;
        }
    }
    
    cout<<"Allocated Gradient Memory Successfully 1! \n";
    
    int num=2*4*(maxscale+1);
    weight_gradient_matrix=new double***[maxscale*2];
    for(int i=0;i<maxscale*2;i++){
        weight_gradient_matrix[i]=new double**[num];
        for(int j=0;j<num;j++){
            weight_gradient_matrix[i][j]=new double*[no_of_interpolating_points];
            for(int k=0;k<no_of_interpolating_points;k++){
                weight_gradient_matrix[i][j][k]=new double[2];
                for(int l=0;l<2;l++){
                    weight_gradient_matrix[i][j][k][l]=0;
                }
            }
        }
    }
    colmat=new double[2];
    expected_mat=new double[2];
    cout<<"Allocated Gradient Memory Successfully! \n";

}

void allocateMemory( int Rows, int Cols){
    image = NULL;
    image = new unsigned short*[Rows];
    gradImage=new double*[Rows];
    gradientImage=new double*[Rows];
    for(int i=0;i<Rows;i++){
        image[i]=new unsigned short[Cols];
        gradImage[i]=new double[Cols];
        gradientImage[i]=new double[Cols];
        for(int j=0;j<Cols;j++){
            image[i][j]=0;
            gradImage[i][j]=0;
            gradientImage[i][j]=0;
        }
    }
    cout<<"Allocated Image Memory Successfully! \n";
}

template<class T1,class T2>
void writeImage(T1** arr, int Rows, int Cols,CImg<T2> cimg,string filename, bool isPrint=false){
    for(int i=0;i<Rows;i++){
        for(int j=0;j<Cols;j++){
            T2 value=(T2) arr[i][j];
            cimg(i,j)=arr[i][j];
        }
    }

    cimg.save_tiff(filename.c_str());
    if(isPrint)
    cimg.display();
}

template<class T1,class T2>
void writeImageunsigned(T1** arr, int Rows, int Cols,CImg<T2> cimg,string filename, bool isPrint=false){
    for(int j=0;j<Cols;j++){
        for(int i=0;i<Rows;i++){
            T1 value=(T1) arr[i][j];
            if(isPrint && value==0)
                cout<<value<<" ";
            cimg(i,j)=arr[i][j];
        }
    }
    cimg.save_tiff(filename.c_str());
    cimg.display();
}

template<class T1>
void writeImagefile(T1** arr, int Rows, int Cols,string filename, bool isPrint=false){
    ofstream fout;
    fout.open(filename.c_str());
    for(int j=0;j<Cols;j++){
        for(int i=0;i<Rows;i++){
            int value= arr[i][j];
            if(isPrint && value!=0)
                cout<<value<<" ";
            fout.write((char*)&value,sizeof(unsigned short));
        }
    }
    fout.close();;
}

template<class T1>
int readImage(CImg<unsigned short> cimg,T1** arr,bool isprint=false){
    //cout<<cimg.height()<<" "<<cimg.width()<<"\n";
    cimg_forXY(cimg,x,y) {arr[x][y]=cimg(x,y);
                          if (isprint&&arr[x][y]>0) cout<<arr[x][y]<<" ";}
    return 1;
}


void readImagefile(string filename,unsigned short** arr,bool isprint=false){
    ifstream fin;
    fin.open(filename.c_str());
    for(int j=0;j<Ydim;j++){
        for(int i=0;i<Xdim;i++){
            fin.read((char*)&arr[i][j], sizeof(unsigned short));
            if(isprint && arr[i][j]!=0){
                cout<<arr[i][j]<<" ";
            }
        }
    }
}

template<class T1>
void removeNegative(T1*** arr,int Rows, int Cols){
    for(int i=0;i<Rows;i++){
        for(int j=0;j<Cols;j++){
            if(arr[i][j]>ushrtmax) {arr[i][j]=0;}
        }
    }
}

template<class T1>
CImg<T1> convert_to_gray_scale(CImg<T1> rgbImage,int row, int col){
    CImg<T1>grayWeight(rgbImage.width(), rgbImage.height(), 1, 1, 0),
            imgR(rgbImage.width(), rgbImage.height(), 1, 3, 0),
            imgG(rgbImage.width(), rgbImage.height(), 1, 3, 0),
            imgB(rgbImage.width(), rgbImage.height(), 1, 3, 0);

    cimg_forXY(rgbImage,x,y) {
        int R = (int)rgbImage(x,y,0,0);
        int G = (int)rgbImage(x,y,0,1);
        int B = (int)rgbImage(x,y,0,2);
        // Arithmetic addition of channels for gray
        // Real weighted addition of channels for gray
        int grayValueWeight = (int)(0.299*R + 0.587*G + 0.114*B);
        // saving píxel values into image information
        grayWeight(x,y,0,0) = grayValueWeight;
    }

    return grayWeight;
}

template<class T1>
CImg<T1> convert_to_gray_scale_from_red(CImg<T1> rgbImage,int row, int col){
    CImg<T1>grayWeight(rgbImage.width(), rgbImage.height(), 1, 1, 0),
            imgR(rgbImage.width(), rgbImage.height(), 1, 3, 0),
            imgG(rgbImage.width(), rgbImage.height(), 1, 3, 0),
            imgB(rgbImage.width(), rgbImage.height(), 1, 3, 0);

    cimg_forXY(rgbImage,x,y) {
        int R = (int)rgbImage(x,y,0,0);
        int G = (int)rgbImage(x,y,0,1);
        int B = (int)rgbImage(x,y,0,2);
        // Arithmetic addition of channels for gray
        // Real weighted addition of channels for gray
        int grayValueWeight = (int)(R + 0.587*G + 0.114*B);
        // saving píxel values into image information
        grayWeight(x,y,0,0) = grayValueWeight;
    }

    return grayWeight;
}



template<class T1>
CImg<T1> convert_to_RGB_image(CImg<T1> grayscale,CImg<T1> circular){
    CImg<T1>RGBimage(grayscale.width(), grayscale.height(), 1, 3, 0);
    cimg_forXY(grayscale,x,y) {
        unsigned short val=grayscale(x,y);
        unsigned short cval=circular(x,y);
        if(cval==255) {
            RGBimage(x,y,0,0)=255;
            RGBimage(x,y,0,1)=0;
            RGBimage(x,y,0,2)=255;
        }
        else{
            RGBimage(x,y,0,0)=val;
            RGBimage(x,y,0,1)=val;
            RGBimage(x,y,0,2)=val;
        }
    }
    cout<<"Conversion to RGB is done\n";
    return RGBimage;
}

template<class T1>
CImg<T1> convert_to_RGB_image(CImg<T1> grayscale){
    CImg<T1>RGBimage(grayscale.width(), grayscale.height(), 1, 3, 0);
    cimg_forXY(grayscale,x,y) {
        unsigned short val=grayscale(x,y);
        RGBimage(x,y,0,0)=val;
        RGBimage(x,y,0,1)=val;
        RGBimage(x,y,0,2)=val;

    }
    cout<<"Conversion to RGB is done\n";
    return RGBimage;
}

template<class T1>
CImg<T1> circle_on_RGB_image(CImg<T1> colorimage,CImg<T1> circular){
    CImg<T1>RGBimage(colorimage.width(), colorimage.height(), 1, 3, 0);
    cimg_forXY(colorimage,x,y) {
        unsigned short cval=circular(x,y);
        if(cval==255) {
            RGBimage(x,y,0,0)=255;
            RGBimage(x,y,0,1)=255;
            RGBimage(x,y,0,2)=255;
        }
        else{
            RGBimage(x,y,0,0)=colorimage(x,y,0,0);
            RGBimage(x,y,0,1)=colorimage(x,y,0,1);
            RGBimage(x,y,0,2)=colorimage(x,y,0,2);
        }

    }
    cout<<"Circles drawing on Point Gradient Image is done\n";
    return RGBimage;
}


double compute_SNR(unsigned short** arr, int Xdim, int Ydim){
    double mean=0;
    double snr=0;
    double sum=0;
    for(int i=0;i<Xdim;i++){
        for(int j=0;j<Ydim;j++){
            sum+=arr[i][j];
        }
    }
    mean=sum/(Xdim*Ydim);
    cout<<"Mean: "<<mean<<"\n";
    sum=0;
    for(int i=0;i<Xdim;i++){
        for(int j=0;j<Ydim;j++){
            sum+=(arr[i][j]-mean)*(arr[i][j]-mean);
        }
    }
    double var=sum/(Xdim*Ydim);
    var=sqrt(var);
    cout<<"SD: "<<var<<"\n";
    snr=(var*var)/mean;
    return snr;
}

void colorcoding(unsigned short** grayscale,string colorcodedimage){
    int max=INT_MIN;
    int min=INT_MAX;
    long long int sum=0;
    for(int i=0;i<Xdim;i++){
        for(int j=0;j<Ydim;j++){
            if(grayscale[i][j]>max){
                max=grayscale[i][j];
            }
            if(grayscale[i][j]<min){
                min=grayscale[i][j];
            }
        }
    }
    
    cout<<"max scale: "<<max<<" min scale: "<<min<<"\n";
    double ycutoff;
    double gcutoff;
    double ccutoff;
    double bcutoff=1;

    cout<<"Enter yellow , green and cyan cutoff: ";
    cin>>ycutoff>>gcutoff>>ccutoff;

    CImg<unsigned short>RGBimage(Xdim, Ydim, 1, 3, 0);
    int R,G,B;
    for(int x=0;x<Xdim;x++){
        for(int y=0;y<Ydim;y++){
            double frac=(double)(grayscale[x][y]-min)/(double)(max-min);
            double range1=ycutoff;
            double range2=gcutoff-ycutoff;
            double range3=ccutoff-gcutoff;
            double range4=bcutoff-ccutoff;
            if(frac>=0 && frac<=ycutoff){
                RGBimage(x,y,0,0)=0;
                RGBimage(x,y,0,1)=(short)(abs(frac/range1)*255);
                RGBimage(x,y,0,2)=255;
            }
            
            if(frac>ycutoff && frac<=gcutoff){
                RGBimage(x,y,0,0)=0;
                RGBimage(x,y,0,1)=255;
                RGBimage(x,y,0,2)=(short)(abs((gcutoff-frac)/range2)*255);
            }
            
            if(frac>gcutoff && frac<=ccutoff){
                RGBimage(x,y,0,0)=0;
                RGBimage(x,y,0,1)=255;
                RGBimage(x,y,0,2)=(short)(abs((frac-gcutoff)/range3)*255);
            }
            
            if(frac>ccutoff && frac<=bcutoff){
                RGBimage(x,y,0,0)=255;
                RGBimage(x,y,0,1)=(short)(abs((bcutoff-frac)/range4)*255);
                RGBimage(x,y,0,2)=0;
            }
        }
    }
    
    RGBimage.save_tiff(colorcodedimage.c_str());
    RGBimage.display();
}

int*getcolorMapping(double saturation,double hu){
    int* color=new int[3];
    
    double range=60;
    if(hu<=60){
        color[0]=255*saturation;
        color[1]=((hu/range)*255*saturation);//+255*(1-saturation));
        color[2]=0;
    }
    if(hu>60&&hu<=120){
        color[0]=((120.0-hu)/range)*255*saturation;//255*(1-saturation);
        color[1]=255*saturation;
        color[2]=0;
    }
    if(hu>120&&hu<=180){
        color[0]=0;
        color[1]=255*saturation;
        color[2]=((hu-120.0)/range)*255*saturation;//+255*(1-saturation);
    }
    if(hu>180&&hu<=240){
        color[0]=0;//255*(1-saturation);
        color[1]=((240.0-hu)/range)*255*saturation;//+255*(1-saturation);
        color[2]=255*(saturation);
    }
    if(hu>240&&hu<=300){
        color[0]=((hu-240.0)/range)*255*saturation;//+255*(1-saturation);
        color[1]=0;//255*(1-saturation);
        color[2]=255*saturation;
    }
    if(hu>300&&hu<=360){

        color[0]=255*saturation;
        color[1]=0;//255*(1-saturation);
        color[2]=((360.0-hu)/range)*255*saturation;//+255*(1-saturation);
    }
    return color;
}



int*getcolorMappingfordistribution(double saturation,double hu){
    int* color=new int[3];
    double range=0.25;
    if(hu<=0.25){
        color[0]=255*saturation;
        color[1]=((hu/range)*255*saturation);
        color[2]=0;
    }
    else if(hu>0.25&&hu<=0.5){
        color[0]=((0.5-hu)/range)*255*saturation;
        color[1]=255*saturation;
        color[2]=0;
    }
    else if(hu>0.5&&hu<=0.75){
        color[0]=0;
        color[1]=255*saturation;
        color[2]=((hu-0.5)/range)*255*saturation;
    }
    else if(hu>0.75&&hu<=0.9){
        color[0]=0;
        color[1]=((0.9-hu)/0.15)*255*saturation;
        color[2]=255*(saturation);
    }

    else if(hu>0.9&&hu<=1){
        color[0]=0;
        color[1]=0;
        color[2]=((1-hu)/0.1)*255*saturation;
    }
    return color;
}

void gradient_color_Image(double** gradientangle, double** gradientImage,string imagename,bool isPrint=false){
    
    unsigned short max=0;
    int maxX,maxY;
    unsigned short min=USHRT_MAX;
    double variance=0;
    for(int i=0;i<Xdim;i++){
        for(int j=0;j<Ydim;j++){
            if(gradientImage[i][j]>max) {max=gradientImage[i][j];maxX=i;maxY=j;}
            if(gradientImage[i][j]<min) min=gradientImage[i][j];
        }
    }

    cout<<"Maximum Gradient==> "<<max<<" Minimum Gradient=> "<<min<<"\n";
    cout<<"Maximum X==> "<<maxX<< " "<<"Minimum Y==> "<<maxY<<"\n";
    CImg<unsigned short>gradientRgbimage(Xdim, Ydim, 1, 3, 0);
    for(int i=0;i<Xdim;i++){
        for(int j=0;j<Ydim;j++){
            double saturation=colorgaussian(gradientImage[i][j],0,colorgaussvar);
            saturation=1-saturation;
            double hu=gradientangle[i][j];

            int* color=getcolorMapping(saturation,hu);
            gradientRgbimage(i,j,0,0)=color[0];
            gradientRgbimage(i,j,0,1)=color[1];
            gradientRgbimage(i,j,0,2)=color[2];
        }
    }
    gradientRgbimage.save_tiff(imagename.c_str());
    if(isPrint)
    gradientRgbimage.display();
}

double computeAngle(double x, double y){
    double angle;
    if(x==0&&y==0) return 0;
    if(x==0&&y>0) return 270;
    if(x==0&&y<0) return 90;
    if(x>0&&y>0) angle=360-radtoDeg(atan(y/x));
    if(x<0&&y>=0) angle=180+radtoDeg(atan(fabs(y)/fabs(x)));
    if(x<0 && y<0) angle=180-radtoDeg(atan(fabs(y)/fabs(x)));
    if(x>0&&y<0) angle=radtoDeg(atan(fabs(y)/fabs(x)));
    return angle;
}


#endif // COMMONS_H
