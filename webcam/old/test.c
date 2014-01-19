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
    fotka =(JPEG*) malloc(sizeof(JPEG));
    fotka->image=(uint8_t*)malloc(30000);
    char s[10];
	for(i=0; i< 50; i++){
		frame = cvQueryFrame( capture );
		encode(frame, fotka);
		//char s[10];
		sprintf(s,"%d",i);
		strcat(tmp, s);
		file = fopen(tmp, "w");
		fwrite(fotka->image, sizeof(char), fotka->size, file);
		tmp[6] = 0;
		s[0] = 0;		
		fflush(file);
		//free(fotka->image);
		fclose(file);
		usleep(100);

	}
	
	//fclose(file);
	makeMovie(50, "photo/", 100000);
	//encoderTeardown();
	free(fotka->image);
	free(fotka);
	return 0;
}


