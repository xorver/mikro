#include "encoder.h"

int main()
{
	int i;
	//uint32_t	maxBuffSize	= 30000;
	//uint8_t*	outBuff		= (uint8_t*) malloc(maxBuffSize);
	//int32_t	outSize;
	IplImage *frame;

	CvCapture *capture = cvCaptureFromCAM( 0 );
	frame = cvQueryFrame( capture );

	JPEG* fotka = encode(frame);
	
	FILE* file = fopen("focia.jpg", "w");
	fwrite(fotka->image, sizeof(char), fotka->size, file);

	return 0;
}



