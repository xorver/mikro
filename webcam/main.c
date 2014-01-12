#include "basicWebcamFunctionality.h"
#include "email.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <signal.h>
#include "core/core_c.h"
#include "core/types_c.h"
#include "highgui/highgui_c.h"

#define DURING_RECORDING_INTERVAL 3000000
#define DURING_WAITING_INTERVAL 200000
#define PIXEL_VALUE_EPSILON 30 
#define MINIMAL_DIFFERENCE_PERCENTAGE 10
//(640*480*3)
#define IMAGE_DATA_SIZE 921600

int i;
int keepGoing=1;

void init(){
	webcamInit();
	emailInit();
}

void teardown(){
	webcamTeardown();
}

void intHandler(int dummy) {
    keepGoing=0;
}

int isDifferent(char* first, char* second){
	int diffrentPixelsNo=0;
	char* p1 = first;
	char* p2 = second;
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

// recording loop, stops recording if nothing changes on view
void recordingLoop(char* previousImage, char* actualImage){
	startRecording();
	while(isDifferent(previousImage,actualImage) && keepGoing){
		memcpy(previousImage,actualImage,IMAGE_DATA_SIZE);
		usleep(DURING_RECORDING_INTERVAL);
		memcpy(actualImage,takePicture()->imageData,IMAGE_DATA_SIZE);
	}
	stopRecording();
}

//starts recording if something changes
void detectingMotionLoop(){
	char* actualImage=(char*)malloc(IMAGE_DATA_SIZE);
	char* previousImage=(char*)malloc(IMAGE_DATA_SIZE);
	memcpy(actualImage,takePicture()->imageData,IMAGE_DATA_SIZE);
	while(keepGoing){
		memcpy(previousImage,actualImage,IMAGE_DATA_SIZE);
		char* imageData = takePicture()->imageData;
		memcpy(actualImage,imageData,IMAGE_DATA_SIZE);
		if(isDifferent(previousImage,actualImage))
			recordingLoop(previousImage,actualImage);
		usleep(DURING_WAITING_INTERVAL);
	}
}

int main(){
	//init&teardown
	init();
	if (atexit(teardown) != 0) {
		fprintf(stderr, "cannot set exit function\n");
		return EXIT_FAILURE;
	}
	signal(SIGINT, intHandler);

	//main loop
	detectingMotionLoop();	
	teardown();
	return 0;
}









