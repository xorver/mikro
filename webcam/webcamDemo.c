#include "basicWebcamFunctionality.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
	webcamInit();
	takePicture();
	sleep(2);
	startRecording();
	sleep(10);
	stopRecording();
	return 0;
}
