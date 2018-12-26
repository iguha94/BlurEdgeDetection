#ifndef GLOBAL_H
#define GLOBAL_H

#include <iostream>
#include <map>
#include <list>
#include <fstream>
#include <vector>
#include <algorithm>

#define ushrt unsigned short
using namespace std;

class Point{
public:
    int x;
    int y;
    double value;
    Point(int x, int y, double value){
        this->x=x;
        this->y=y;
        this->value=value;
    }
};

string edgepath;
string textpath;
double lowerpercent;
double upperpercent;
unsigned short ushrtmax=32768;
unsigned short** image;
double** gradImage;
int Xdim;
int Ydim;
int Xc;
int Yc;
int intermediatescale;
double totalgradient=0;
double totalgradient1=0;
double totalsgradient=0;
double gradientvar=0;
double gradientsvar=0;
double snr=1;
double memconf;
string imagename;
string imagefilename;
string imageId;
string gradientimagename;
string scaleimagename;
string textfile;

string sordergradientimagename;
string sorderscaleimagename;

string gaussgradientimagename;
string gradientcolorimagename;
const char* buffer;
void* hdr;
unsigned short  maxIntensity=3000;
unsigned short minIntensity=0;
map<long long int, bool> scaleMap;
list<Point> weightvector;

int maxkernelsize;
double*** PrecomputedKernelsX;
double*** PrecomputedKernelsY;
double maxvar=10;
double minval=1;
double** kernelX;
double** kernelY;
double** cov_observed;
double** cov_inner;
double** cov_final;
double** cov_sample;
double** kernelSX;
double** kernelSY;
double** kernelSXY;
double** Dividorarr;
double** Minarr;
double** MaximizedScale;

double** GradientX;
double** GradientY;
double** ScaleGradientX;
double** ScaleGradientY;
double** LocalMaxima;
double** LocalMinima;
double** Hysteresis;
unsigned short** thresholdedimg;
int** countarr;
double** secondderivativeimg;

double** Gradientangle;
double** SGradientangle;
double* colmat;
double** ReliableScale;
unsigned short** DialatedScale;
double** gradientImage;
unsigned short** gradientSImage;
unsigned short** dtImage;
unsigned short** skelImage;
unsigned short** prunnedskelImage;
bool** isComputed;
double varH=130;
double varL=60;
double mean=0;
double meanL=0;
double meanH=0;
int* pointsarr;
double gaussvar=0.5;
double gaussgradvar=1;
double curgradX;
double curgradY;
double curgrad;
double gradLX;
double gradLY;
double curvar;
double gradThresh=0.63;
double gradientPercentage=50;
double gradientTotalX=0;
double gradientTotalY=0;

double Totalweight=0;
double TotalweightX=0;
double TotalweightY=0;
double PrevweightX=0;
double PrevweightY=0;
double curgradSX=0;
double curgradSY=0;
double curgradSXY=0;
double noisestdv;
double TotalweightS=0;
double TotalweightSX=0;
double TotalweightSY=0;
double TotalweightSXY=0;

double confidence;
double variance=1.0;
double globalmeanX=1.0;
double globalmeanY=1.0;

double globalmeanSX=1.0;
double globalmeanSY=1.0;
double globalmeanSXY=1.0;
double gradienthresh;
double colorgaussvar;
double pointsunderarea;
double prevweight;
string gausscolorimagename;
string localmaximaimagename;
string Hysteresisimagename;
string DTimagename;
string Prunnedimagename;
string localizedimagename;
string dialatedimagename;
string dividorimagename;
string secondderivativeimagename;
int maxscale;
double ncount;
vector<double> percentile;
vector<double> spercentile;
double butterworth_cutoff;
double**** weight_gradient_matrix;
int no_of_interpolating_points;
int totalpoints;
double minimumGradient;
string basepath;//="/nfs/s-l011/local/vol01/i/iguha/Documents/Phd-iguha/Imagesfortesting/Comparison/Data12/";
double interpolatingpoint_weight;
int sampling_rate;
ofstream sout;
double* observed_mat;
double* expected_mat;
double** invmat;
double sample_mean_X;
double sample_mean_Y;
double divisor;
double squaredsumx=0;
double squaredsumy=0;
double weightsum=0;
ofstream logger;

const int ISOLATEDPOINT=0;
const int EDGEPOINT=1;
const int SIMPLEPOINT=2;
const int JUNCTIONPOINT=3;

class Pixel{
public:
    int x,y;
    int dtVal;
    Pixel(int x, int y, int dtVal){
        this->x=x;this->y=y;
        this->dtVal=dtVal;
    }
};

#endif //
GLOBAL_H
