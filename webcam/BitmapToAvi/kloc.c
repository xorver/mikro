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
	char tmp[50] = "photo/";
	int i = 0;
	capture = cvCaptureFromCAM( 0 );
	for(i=0; i< 100; i++){
		frame = cvQueryFrame( capture );
		fotka = encode(frame);
		char s[10];
		sprintf(s,"%d",i);
		strcat(tmp, s);
		file = fopen(tmp, "w");
		fwrite(fotka->image, sizeof(char), fotka->size, file);
		tmp[6] = 0;		
		fflush(file);
		usleep(200);
	}
	fclose(file);
	makeMovie(100, "photo/", 200000);
	return 0;
}


