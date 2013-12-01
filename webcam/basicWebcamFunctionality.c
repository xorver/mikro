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
	system("sudo fswebcam -q -r 320x240 --png 8 /tmp/pictures/picture.png");
	return fopen(_PICTURE_PATH, "r");
}

void startRecording(){
	system("sudo ffmpeg -f video4linux2 -r 25 -s 320x240 -i /dev/video0 /tmp/videos/video.avi &");
}

void stopRecording(){
	system("sudo kill -SIGINT `ps aux | grep ffmpeg | awk '{print $2}'`");
}
