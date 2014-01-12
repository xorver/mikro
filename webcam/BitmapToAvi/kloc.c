#include "encoder.h"
#include <string.h>
#include <unistd.h>
#include "jpegtoavi.h"

CvCapture *capture;
JPEG* fotka;
FILE* file;
IplImage *frame;

int main()
{
	int i = 0;
	capture = cvCaptureFromCAM( 0 );
	for(i=0; i< 5; i++){
		frame = cvQueryFrame( capture );
		fotka = encode(frame);
		char s[10];
		sprintf(s,"%d",i);
		file = fopen(s, "w");
		fwrite(fotka->image, sizeof(char), fotka->size, file);
		fflush(file);
		usleep(100);
	}
	fclose(file);
	printf("KLOC\n");
	char** focie =  malloc(sizeof(char*)*9);
	int t;	
	for(t=0; t<9; t++)
		focie[t] = malloc(50);	
	focie[0] = "makemovie";
	focie[1] = "500000";
	focie[2] = "640";
	focie[3] = "480";
	focie[4] = "0";
	focie[5] = "1";
	focie[6] = "2";
	focie[7] = "3";
	focie[8] = "4";

	printf("KLOC2\n");
	fflush(stdout);
	makeMovie(9, focie);
	return 0;
}


