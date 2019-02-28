#include <iostream>
#include <string>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdlib>
using namespace std;

string NoiseLevel[3]={"SNR6","SNR12","SNR18"};
string Prefix[3]={"SNR-6","SNR-12","SNR-18"};
string Prefixid[3]={"6","12","18"};

int main(int argc,char* argv[]){
    string basePath=argv[1];
    string targetDirectory=argv[2];

    for(int i=1;i<=15;i++){

        stringstream ss;
        ss << i;
        string id = ss.str();

        string directory=basePath+targetDirectory+"/Phantom"+id+"/";

        for(int j=0;j<3;j++){
            string noisedirectory=directory+NoiseLevel[j]+"/";
            string imagename=Prefix[j]+"-smooth-Phantom"+id+".jpg";

            string scaleedgename="scalebasededge_"+Prefix[j]+"-smooth-Phantom"+id+".tif";
            string ezedgename="EZ_"+Prefix[j]+"-smooth-Phantom"+id+".jpg";

string cannyedgename="Canny-"+Prefix[j]+"-smooth-Phantom"+id+".tif";
            string command="./overlapedge.exe "+imagename+" "+scaleedgename+" 255 0 0 "+noisedirectory;
            cout<<"Command: "<<command<<"\n";
            system(command.c_str());

            command="./overlapedge.exe "+imagename+" "+ezedgename+" 255 0 0 "+noisedirectory;
            system(command.c_str());

command="./overlapedge.exe "+imagename+" "+cannyedgename+" 255 0 0 "+noisedirectory;
            system(command.c_str());
        }
    }
}
