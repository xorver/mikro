#include "encoder.h"
#include <string.h>
#include <unistd.h>
#include "jpegtoavi.h"

CvCapture *capture;
JPEG* fotka;
FILE* file;
IplImage *frame;

int main(){
	encoderInit();
	char tmp[50] = "photo/";
	int i = 0;
	capture = cvCaptureFromCAM( 0 );
	for(i=0; i< 200; i++){
		frame = cvQueryFrame( capture );
		fotka = encode(frame);
		char s[10];
		sprintf(s,"%d",i);
		strcat(tmp, s);
		file = fopen(tmp, "w");
		fwrite(fotka->image, sizeof(char), fotka->size, file);
		tmp[6] = 0;		
		fflush(file);
		free(fotka->image);
		fclose(file);
		usleep(50);

	}
	//fclose(file);
	makeMovie(200, "photo/", 50000);
	encoderTeardown();
	//free(fotka->image);
	//free(fotka);
	return 0;
}


