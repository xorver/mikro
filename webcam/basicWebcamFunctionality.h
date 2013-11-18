#ifndef _BWCF
#define _BWCF

#define TRUE = 1;
#define FALSE = 0;
#define _PICTURE_PATH = /tmp/pictures/picture.png;

void webcamInit();
FILE* takePicture();
void startRecording();
void stopRecording();

#endif
