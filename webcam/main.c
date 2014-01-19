#include "basicWebcamFunctionality.h"
#include "email.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <signal.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#define DURING_RECORDING_INTERVAL 3000000
#define DURING_WAITING_INTERVAL 200000



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

//starts recording if something changes
void detectingMotionLoop(){
	char* actualImage=(char*)malloc(IMAGE_DATA_SIZE);
	char* previousImage=(char*)malloc(IMAGE_DATA_SIZE);
	char* imageData;
	memcpy(actualImage,takePicture()->imageData,IMAGE_DATA_SIZE);
	while(keepGoing){
		memcpy(previousImage,actualImage,IMAGE_DATA_SIZE);
        imageData = takePicture()->imageData;
        memcpy(actualImage,imageData,IMAGE_DATA_SIZE);
		if(isDifferent(previousImage,actualImage)){
			startRecording();
            imageData = takePicture()->imageData;
            memcpy(actualImage,imageData,IMAGE_DATA_SIZE);
        }
		
		
		usleep(DURING_WAITING_INTERVAL);
	}
	free(actualImage);
    free(previousImage);
}

int main(){
	//init&teardown
	init();
	signal(SIGINT, intHandler);

	//main loop
	detectingMotionLoop();	
	teardown();
	return 0;
}









