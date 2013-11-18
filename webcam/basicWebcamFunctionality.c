#include "basicWebcamFunctionality.h"
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

void webcamInit(){
	mkdir("/tmp/pictures", 0777);
	mkdir("/tmp/videos", 0777);
	chmod("/dev/video0", 0777);
}

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
