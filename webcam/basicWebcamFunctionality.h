#ifndef _BWCF
#define _BWCF

#include<stdio.h>
#define _PICTURE_PATH "/tmp/pictures/picture.png"

void webcamInit();
FILE* takePicture();
void startRecording();
void stopRecording();

#endif
