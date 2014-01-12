#include "basicWebcamFunctionality.h"
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include "core/core_c.h"
#include "core/types_c.h"
#include "highgui/highgui_c.h"
#include "email.h"

CvCapture* capture;
int createdMovies=0;

void webcamInit(){
	capture = cvCaptureFromCAM(CV_CAP_ANY); //Capture using any camera connected to your system
	mkdir(PICTURE_PATH, 0777);
	mkdir(VIDEO_PATH, 0777);
	chmod("/dev/video0", 0777);
}

IplImage* takePicture(){
	return cvQueryFrame(capture);
}

void startRecording(){
	printf("***start recoding***\n");
	fflush(stdout);
	
	char* actualBitmap=(char*)malloc(IMAGE_DATA_SIZE);
	char* previousBitmap=(char*)malloc(IMAGE_DATA_SIZE);
	IplImage* actualImage;
	int equalInRow=0;
	int j=0;
	while(equalInRow<EQL_IN_ROW_TO_STOP_RECORDING){
		actualImage = takePicture();
		if((j % NTH_FRAME_OF_RECORD)==0){
			memcpy(previousBitmap,actualBitmap,IMAGE_DATA_SIZE);
			memcpy(actualBitmap,actualImage->imageData,IMAGE_DATA_SIZE);
			if(isDifferent(previousBitmap,actualBitmap))
				equalInRow=0;
			else
				equalInRow++;
		}
		//save in dir (actualImage,strcat(PICTURE_PATH,itoa(j)))
		j++;
	}
	char* aviPath = malloc(strlen(VIDEO_PATH)+10);
	sprintf(aviPath,"%s%d%s",VIDEO_PATH,createdMovies,".avi");
	printf("***stop recording***\n");
	fflush(stdout);
	
	//make movie (i,VIDEO_FPS,PICTURE_PATH,aviPath)
	system(strcat("touch ",aviPath)); //todo delete
	
	sendVideo(aviPath);
	createdMovies++;
}

int isDifferent(char* first, char* second){
	int diffrentPixelsNo=0;
	char* p1 = first;
	char* p2 = second;
	int i;
	for(i=0;i<IMAGE_DATA_SIZE;i++){		
		if(abs((int)*p1-(int)*p2)>PIXEL_VALUE_EPSILON)
			diffrentPixelsNo++;
		p1++;
		p2++;
	}
	int differentInPercents = floor(diffrentPixelsNo*100 / IMAGE_DATA_SIZE);
	printf("pictures different in: %d%%, returning %d\n",differentInPercents,differentInPercents>MINIMAL_DIFFERENCE_PERCENTAGE);
	fflush(stdout);
	return differentInPercents>MINIMAL_DIFFERENCE_PERCENTAGE;
}

void stopRecording(){
	printf("***stop recoding***\n");
	fflush(stdout);
	//todo get all created photos from startRecording and convert them to movie
}

void webcamTeardown(){
	cvReleaseCapture(&capture); //Release capture.
}
