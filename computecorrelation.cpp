#include "CImg.h"
#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
using namespace std;
using namespace cimg_library;

string NoiseLevel[3]={"SNR6","SNR12","SNR18"};
string Prefix[3]={"SNR-6","SNR-12","SNR-18"};
string Prefixid[3]={"6","12","18"};

int main(int argc, char* argv[]){
    string basePath=argv[1];
    string targetDirectory=argv[2];

    for(int i=1;i<=12;i++){ //For each phantoms

        stringstream ss;
        ss << i;
        string id = ss.str();

        string trueBlurPath=basePath+targetDirectory+"/Phantom"+id+"/scaleList.txt";
        string imagename=basePath+targetDirectory+"/Phantom"+id+"/BlurField"+id+".jpg";

        CImg<unsigned short> jpgimage(imagename.c_str());
        int Xdim=jpgimage.width();
        int Ydim=jpgimage.height();

        double** trueblurarr=new double*[Xdim];

        for(int i=0;i<Xdim;i++) {
            trueblurarr[i]=new double[Ydim];
        }

        fstream trueblurreader;
        trueblurreader.open(trueBlurPath.c_str(),ios::in);

        int x,y;
        double v;
        while(trueblurreader>>x>>y>>v){
            //cout<<x<<" , "<<y<<" , "<<v<<"\n";
            if(x>=Xdim || y>=Ydim || x<0 || y<0) break;
            trueblurarr[x][y]=v;
        }

        cout<<"read True Blur values\n";

        for(int j=0;j<3;j++){ //for each noise level
            string basedirectory=basePath+targetDirectory+"/Phantom"+id+"/"+NoiseLevel[j]+"/";

            string ezcomputedblurfile=basedirectory+"EZBlur"+Prefixid[j]+"_Phantom"+id+".txt";
            string ezcorrelationfile=basedirectory+"EZCorr.csv";

            fstream ezhandler;
            ezhandler.open(ezcomputedblurfile.c_str(),ios::in);
            fstream ezwriter;
            ezwriter.open(ezcorrelationfile.c_str(),ios::out);
            ezwriter<<"X,Y,TrueBlur,AppliedBlur\n";
            while(ezhandler>>x>>y>>v){
                if((x>2 && x<Ydim-2) && (y>2 && y<Xdim-2))
                ezwriter<<y<<","<<x<<","<<trueblurarr[y][x]<<","<<v<<"\n"; //x,y,trueblur,computedblur
            }

            ezhandler.close(); ezwriter.close();

            string scalebasedcomputedblurfile=basedirectory+"scale-"+Prefix[j]+"-smooth-Phantom"+id+".tif";
            string scalebasededgefile=basedirectory+"scalebasededge_"+Prefix[j]+"-smooth-Phantom"+id+".tif";
            string scalebasedcorrelationfile=basedirectory+"ScaleCorr.csv";

            CImg<unsigned short> computedscaleimage(scalebasedcomputedblurfile.c_str());
            CImg<unsigned short> computededgeimage(scalebasededgefile.c_str());

            fstream scalewriter;
            scalewriter.open(scalebasedcorrelationfile.c_str(),ios::out);
            scalewriter<<"X,Y,TrueBlur,AppliedBlur\n";
            for(int a=2;a<Xdim-2;a++){
                for(int b=2;b<Ydim-2;b++){
                    if(computededgeimage(a,b)>200){
                        scalewriter<<a<<","<<b<<","<<trueblurarr[a][b]<<","<<((double)computedscaleimage(a,b)/2.0)<<"\n"; //x,y,trueblur,computedblur
                    }
                }
            }

            /*cimg_forXY(computedscaleimage,x,y){
                if(computededgeimage(x,y)>200){
                    scalewriter<<x<<","<<y<<","<<trueblurarr[x][y]<<","<<((double)computedscaleimage(x,y)/2.0)<<"\n"; //x,y,trueblur,computedblur
                }
            }*/

            scalewriter.close();

        }

        for(int i=0;i<Xdim;i++) delete trueblurarr[i];
        delete trueblurarr;
    }
}
