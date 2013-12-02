#include "basicWebcamFunctionality.h"
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

void startPhotoDeamon(){
	system("sudo fswebcam -q -l 1 -r 320x240 --png 8 /tmp/pictures/picture.png &");
}

void stopPhotoDeamon(){
	system("sudo kill -SIGINT `ps aux | grep fswebcam | awk '{print $2}'`"); 
}

void webcamInit(){
	mkdir("/tmp/pictures", 0777);
	mkdir("/tmp/videos", 0777);
	chmod("/dev/video0", 0777);
	startPhotoDeamon();
}

FILE *takePicture(){
	system("sudo mv /tmp/pictures/picture.png /tmp/pictures/actual.png");
	return fopen("/tmp/pictures/actual.png", "r");
}

void startRecording(){
	stopPhotoDeamon();
	system("sudo ffmpeg -f video4linux2 -r 25 -s 320x240 -i /dev/video0 /tmp/videos/video.avi &");
}

void stopRecording(){
	system("sudo kill -SIGINT `ps aux | grep ffmpeg | awk '{print $2}'`");
	startPhotoDeamon();
}

void webcamStop(){
	stopPhotoDeamon();
}
