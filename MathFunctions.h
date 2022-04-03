#ifndef MATHFUNCTIONS_H
#define MATHFUNCTIONS_H

#include <iostream>
#include <math.h>
#include "Global.h"
#include <math.h>
#include "Commons.h"
using namespace std;

unsigned short minimum(unsigned short a, unsigned short b) {return a>b?a:b;}

double ComputeSigmoid(double val,double maximumval){
    double mid = maximumval/2.0;
    double gradval=val-mid;
    //gradval/=mid;
    //gradval*=5;

    double denom=1.0+exp(-(gradval));
    double v=1/denom;
    if(gradval<0) return 1-v;
    return v;
}

double ComputeDecay(double rate,double val, double k){
    if(val>=rate) {return 0;}

    return exp( k - (k / (1 - pow((val/rate),2))));
}

double  gaussian(double intensity,double mean, double var){
    double power=((intensity-mean)*(intensity-mean))/(2*var*var);
    double gaussval=exp(-power);
    double denominator=1/(2*3.14*var*var);
    return gaussval*denominator;
}

double  colorgaussian(double intensity,double mean, double var){
    double power=((intensity-mean)*(intensity-mean))/(2*var*var);
    double gaussval=exp(-power);
    return gaussval;
}

double GetLowerPercentile(vector<double> vec,double X){
    int n=vec.size();
    int rank=(X/100)*(n+1); //simple rounding , but other more accurate methods are available
    sort(vec.begin(),vec.end());
    return vec[rank];
}

double multivariateGaussian(double x, double y, double meanx, double meany, double stdx, double stdy, double covxy){
    double rho=covxy/(stdx*stdy);
    double firstterm=(x-meanx)*(x-meanx);
    firstterm/=(stdx*stdx);
    double secondterm=(y-meany)*(y-meany);
    secondterm/=(stdy*stdy);
    double thirdterm=(x-meanx)*(y-meany)*rho*2;
    thirdterm/=(stdx*stdy);

    double numerator=firstterm+secondterm-thirdterm;
    double denominator=(1-rho*rho)*2;

    double power=(numerator/denominator);
    return exp(-power);
}

void covariance_observed(double sigmaN){
    cov_observed=new double*[2];
    for(int i=0;i<2;i++){
        cov_observed[i]=new double[2];
        for(int j=0;j<2;j++){
            cov_observed[i][j]=0;
        }
    }
    cov_observed[0][0]=(sigmaN*sigmaN)/2;
    cov_observed[1][1]=(sigmaN*sigmaN)/2;
}
double compute_second_order_derivative(double gradx, double grady, double gradxy,double rad){
    double cosval=cos(rad);
    double sinval=sin(rad);
    double grad_magnitude=(cosval*cosval*gradx+sinval*sinval*grady-2*sinval*cosval*gradxy);
    return (grad_magnitude);
}
double gaussian_second_order(double intensity, double mean, double var,int dir,int dir1=0,int dir2=0){
    double power=((intensity-mean)*(intensity-mean));
    double denominator=1/(2*var*var);
    power*=denominator;
    double gaussval=exp(-power);
    double coeff;
    if(dir2!=0){
        coeff=1/((2*M_PI)*pow(var,6));
    }
    else{
        coeff=1/((2*M_PI)*pow(var,4));
    }
    double second_coeff;
    if(dir2!=0) {second_coeff=dir*dir1;}
    else{
        second_coeff=pow((dir/var),2)-1;
    }
    coeff*=second_coeff;
    return gaussval*coeff;
}

double gauss(double intensity,double mean, double var,double dir){
    double power=((intensity-mean)*(intensity-mean));
    double denominator=1/(2*var*var);
    power*=denominator;
    double gaussval=exp(-power);
    double coeff=1/((2*M_PI)*pow(var,4));
    coeff*=dir;
    return gaussval*coeff;
}


double getEucledianDistance( short x1,  short y1, short z1,  short x2,  short y2,  short z2){
    return sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)+(z2-z1)*(z2-z1));
}

int get345Distance( short x1,  short y1,  short x2,  short y2){
    double dist= sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
    return dist*10;
}

double CartesianDistance2D( short x1,  short y1, short x2,  short y2){
    return sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
}

double CartesianDistance2Double( double x1,  double y1, double x2,  double y2){
    return sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
}

double volumeofaSphere(int r){
    return M_PI*r*r*r*1.33;
}

double areaofacircle(int r){
    return M_PI*r*r;
}

double perimeterofcircle(int r){
    return 2*3.14*r;
}

double fractionperimeterofcircle(int r){
    double fraction=gradientPercentage/100;
    return 2*M_PI*r*fraction;
}

double gammaFunction(double smax,double cur_s,double Gamma){
    double fraction=cur_s/(smax+1);
    double val=pow(fraction,Gamma);
    return 1-val;
}

template<class T>
double squareroot(T val1,T val2){
    return sqrt(val1*val1+val2*val2);
}

double det2D(double** mat){
    double det=(mat[0][0]*mat[1][1])-(mat[0][1]*mat[1][0]);
    return det;
}

double** transpose2D(double** mat){
    double** transpose=new double*[2];
    for(int i=0;i<2;i++){
        transpose[i]=new double[2];
        for(int j=0;j<2;j++){
            transpose[i][j]=0;
        }
    }
    transpose[0][0]=mat[0][0];
    transpose[0][1]=mat[1][0];
    transpose[1][0]=mat[0][1];
    transpose[1][1]=mat[1][1];

    return transpose;
}

double** inverse2D(double** mat){
    double det=det2D(mat);
    if(det==0) det=1;

    invmat[0][0]=mat[1][1];
    invmat[1][1]=mat[0][0];
    invmat[0][1]=-mat[0][1];
    invmat[1][0]=-mat[1][0];

    invmat[0][0]/=det;
    invmat[0][1]/=det;
    invmat[1][0]/=det;
    invmat[1][1]/=det;

    return invmat;
}

double* mat_mult_square_col(double** arr1,double*arr2,bool pf=false){
    double val1=arr1[0][0]*arr2[0];
    double val2=arr1[0][1]*arr2[1];
    double val3=arr1[1][0]*arr2[0];
    double val4=arr1[1][1]*arr2[1];
    colmat[0]=val1+val2;
    colmat[1]=val3+val4;
    return colmat;
}

double mat_mult_row_col(double* arr1, double* arr2){
    return arr1[0]*arr2[0]+arr1[1]*arr2[1];
}

double compute_variance_X_from_inner(int r,int prev=1){
    if(prev==0){
        globalmeanX=TotalweightX/(Totalweight);
        globalmeanY=TotalweightY/(Totalweight);
    }
    double xcomp;
    double varx1=0;
    for (int  i=0; i<totalpoints; i++){
        xcomp=0.5*weight_gradient_matrix[r][i][0][0]+weight_gradient_matrix[r][i][1][0]+0.5*weight_gradient_matrix[r][i][2][0];
        xcomp/=2;
        varx1+=(pow((xcomp-globalmeanX),2));
    }
    globalmeanX=TotalweightX/(Totalweight);
    return varx1/(totalpoints);
}

double compute_variance_Y_from_inner(int r,int prev=1){
    if(prev==0){
        globalmeanX=TotalweightX/(Totalweight);
        globalmeanY=TotalweightY/(Totalweight);
    }
    double ycomp;
    double vary1=0;
    for (int  i=0; i<totalpoints; i++){
        ycomp=0.5*weight_gradient_matrix[r][i][0][1]+weight_gradient_matrix[r][i][1][1]+0.5*weight_gradient_matrix[r][i][2][1];
        ycomp/=2;
        vary1+=(pow((ycomp-globalmeanY),2));
    }
    globalmeanY=TotalweightY/(Totalweight);
    return vary1/(totalpoints);
}

double compute_threshold(double totalpoints){
    double value=54.0/totalpoints;
    return value*noisestdv;
}

double compute_confidence(double confidence){
    return confidence;
}

double radtoDeg(double rad){
    double angle=(rad*180)/M_PI;
    return angle;
}
double degtorad(double degree){
    double rad=(M_PI)/180;
    return rad*degree;
}

#endif // MATHFUNCTIONS_H
