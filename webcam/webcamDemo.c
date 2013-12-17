#include "basicWebcamFunctionality.h"
//#include "email.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int i,j;
char *im;

int main(){
	webcamInit();
	for(i = 0; i < 10; i++){
		im = takePicture();
		for(j = 0; j < 10; j++)
			printf("%d ",im[j]);
		printf("\n");
	}
	webcamStop();
	//emailInit();
	//sendVideo("email.h");
	return 0;
}
