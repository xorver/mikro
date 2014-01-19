#include "basicWebcamFunctionality.h"
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "JpegToAvi/jpegtoavi.h"
#include "email.h"
#include "BitmapToJpeg/BitmapToJpeg.h"


CvCapture* capture;
int createdMovies=0;

void webcamInit(){
	capture = cvCaptureFromCAM(0); //Capture using any camera connected to your system
    jpegEncoderInit();
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
	
	char* actualBitmap=(char*) malloc(IMAGE_DATA_SIZE);
	char* previousBitmap=(char*) malloc(IMAGE_DATA_SIZE);
    char* path = (char*) malloc(strlen(PICTURE_PATH)+10);
    char suffix[9];
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
		//prepare path
		strcpy(path,PICTURE_PATH);
        sprintf(suffix,"%d",j);
        strcat(path,suffix);
        
		storeToFile(actualImage,path);
		j++;
	}
	free(actualBitmap);
    free(previousBitmap);
    free(path);

	printf("***stop recording***\n");
	fflush(stdout);
    
    char aviPath[50];
    strcpy(aviPath,VIDEO_PATH);
    strcat(aviPath,"video.avi");
    
    makeMovie(j, PICTURE_PATH, aviPath, VIDEO_FPS);
	printf("***avi file saved***\n");
//	sendVideo(aviPath); 
	printf("***email sent***\n");
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
    jpegEncoderTeardown();
}
