#include "basicWebcamFunctionality.h"
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "core/core_c.h"
#include "core/types_c.h"
#include "highgui/highgui_c.h"

CvCapture* capture;

void webcamInit(){
	capture = cvCaptureFromCAM(CV_CAP_ANY); //Capture using any camera connected to your system
	mkdir("/tmp/pictures", 0777);
	mkdir("/tmp/videos", 0777);
	chmod("/dev/video0", 0777);
}

IplImage* takePicture(){
	//printf("taking picture\n");
	//fflush(stdout);
	return cvQueryFrame(capture);
}

void startRecording(){
	printf("***start recoding***\n");
	fflush(stdout);
	//todo infinite loop which takes photos
}

void stopRecording(){
	printf("***stop recoding***\n");
	fflush(stdout);
	//todo get all created photos from startRecording and convert them to movie
}

void webcamTeardown(){
	cvReleaseCapture(&capture); //Release capture.
}
