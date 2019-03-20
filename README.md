A new edge detection algorithm has been presented using the local blur information at each pixel on an Image. Although there are several popular edge detection algorithm (Canny, Deriche etc) all of them either fails to detect blurred edges or could not differentiate between true and noisy edges. Here we have presented an algorithm that not only detects the highly blurred edges but also suppresses the noisy ones.

Compile: 

Linux: g++ -o EdgeDetectionContinuous.exe EdgeDetectionContinuous.cpp -O2 -L/usr/X11R6/lib -lm -lpthread -lX11
