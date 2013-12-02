#include "basicWebcamFunctionality.h"
#include "email.h"
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
	webcamStop();
	
	emailInit();
	sendVideo("email.h");
	return 0;
}
