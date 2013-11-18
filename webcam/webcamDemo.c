#include "basicWebcamFunctionality.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
	takePicture();
	delay(2000);
	startRecording();
	delay(10000);
	stopRecording();
	return TRUE;
}
