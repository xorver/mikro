#include "basicWebcamFunctionality.h"
#include <stdio.h>

FILE *takePicture(){
	exec("sudo fswebcam -q -r 320x240 --png 8 /tmp/pictures/picture.png");
	return fopen(_PICTURE_PATH, "r");
}

void startRecording(){
	exec("sudo motion start");
}

void stopRecording(){
	exec("sudo motion stop");
}
