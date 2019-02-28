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

int main(int argc, char* argv[]){
    string basePath=argv[1];
    string targetDirectory=argv[2];

    for(int i=0;i<3;i++){
        string noiselevel=NoiseLevel[i];

        for(int j=13;j<=15;j++){
            stringstream ss;
            ss << j;
            string id = ss.str();

            string sourceScaleimage=basePath+noiselevel+"/"+"EZScale_"+Prefix[i]+"-smooth-Phantom"+id+".jpg";
            string sourceEdgeimage=basePath+noiselevel+"/"+"EZ_"+Prefix[i]+"-smooth-Phantom"+id+".jpg";
            string sourceBlurScaleFile=basePath+noiselevel+"/"+"EZBlur"+Prefixid[i]+"_Phantom"+id+".txt";
            string jpgfile=basePath+noiselevel+"/"+Prefix[i]+"-smooth-Phantom"+id+".jpg";

            string TargetPath=basePath+targetDirectory+"/Phantom"+id+"/"+noiselevel+"/";

            string command="cp "+sourceScaleimage+" "+TargetPath;
            system(command.c_str());
            command="cp "+sourceEdgeimage+" "+TargetPath;
            system(command.c_str());
            command="cp "+sourceBlurScaleFile+" "+TargetPath;
            system(command.c_str());
            command="cp "+jpgfile+" "+TargetPath;
            system(command.c_str());
        }
    }
}
