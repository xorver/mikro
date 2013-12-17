#include "basicWebcamFunctionality.h"
#include <sys/stat.h>
#include <sys/types.h>
#include "core/core_c.h"
#include "core/types_c.h"
#include "highgui/highgui_c.h"

CvCapture* capture;

void webcamInit(){
	chmod("/dev/video0", 0777);
	capture = cvCaptureFromCAM(CV_CAP_ANY);
}

char* takePicture(){
	return cvQueryFrame(capture)->imageData;
}

/*void startRecording(){
	stopPhotoDeamon();
	system("sudo ffmpeg -f video4linux2 -r 25 -s 320x240 -i /dev/video0 /tmp/videos/video.avi &");
}

void stopRecording(){
	system("sudo kill -SIGINT `ps aux | grep ffmpeg | awk '{print $2}'`");
	startPhotoDeamon();
}*/

void webcamStop(){
	cvReleaseCapture(&capture);
}
