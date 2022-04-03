//
// Created by iguha on 7/17/19.
//

#include <iostream>
#include <string>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdlib>
using namespace std;

//string Folders[7]={"NewData2","NewData5","NewData9","NewData10","NewData15","NewData16","NewData18"};
//string ImageId[7]={"Data2","Data5","Data9","Data10","Data151","Data161","Data181"};

string Folders[1]={"Phantom1"};
string SNRS[3]={"SNR6","SNR12","SNR18"};
string ImageId[3]={"SNR-6-smooth-Phantom1","SNR-12-smooth-Phantom1","SNR-18-smooth-Phantom1"};

int main(int argc, char* argv[]){
    string basePath=argv[1];

    for(int i=0;i<1;i++) {
        for (int j = 0; j < 3; j++) {
            string noise=SNRS[j];
            string folder = Folders[i];
            string dir = basePath + folder + "/"+noise+"/";
            string image = ImageId[j] + ".jpg";
            string cannyimage = "Canny-" + ImageId[j]+".tif";
            string org_EZ = "EZ_edge.jpg";
            string mat_EZ = "EZ_" + image;
            string scalebased = "scalebasededge_" + ImageId[j]+".tif";

            string command = "./colordilation.exe " + image + " " + cannyimage + " 0 255 0 " + dir;
            system(command.c_str());
            command = "./colordilation.exe " + image + " " + org_EZ + " 0 255 0 " + dir;
            system(command.c_str());
            command = "./colordilation.exe " + image + " " + mat_EZ + " 0 255 0 " + dir;
            system(command.c_str());
            command = "./colordilation.exe " + image + " " + scalebased + " 0 255 0 " + dir;
            system(command.c_str());
        }
    }
}
