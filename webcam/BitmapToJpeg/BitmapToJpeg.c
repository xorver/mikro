#undef UNICODE

#include "Global.h"
#include "Bitmap.h"
#include "EncoderInput.h"
#include "JpegEncoder.h"

#include <stdio.h>
#include <stdlib.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

JpegEncoder*    encoder;
uint8_t*    outBuff;

EncoderInput* loadCapture(IplImage *frame)
{
	EncoderInput* input = (EncoderInput*) malloc(sizeof(EncoderInput));
	int32_t w, h, ch;
	uint8_t* rawData;
	uint32_t x, y, i;
	
	input->cols			= w		= frame->width;
	input->rows			= h		= frame->height;
	input->components	= ch	= frame->nChannels;

	rawData = input->data = (uint8_t*) malloc(w*h*ch);

	i=0;
	for(y = 0; y < h; ++y)
	{
		for(x = 0; x < w; ++x)
		{
			rawData[i++] = frame->imageData[ch*(y*w+x)+0];
			rawData[i++] = frame->imageData[ch*(y*w+x)+1];
			rawData[i++] = frame->imageData[ch*(y*w+x)+2];
		}
	}

	return input;
}

void jpegEncoderInit(){
    uint32_t    maxBuffSize = 500000;
    outBuff     = (uint8_t*) malloc(maxBuffSize);
    encoder = newJpegEncoder(outBuff);
}

void jpegEncoderTeardown(){
        delJpegEncoder(encoder);
}

void storeToFile(IplImage* image,char* path){
    FILE* file = fopen(path,"w");
    EncoderInput* input = loadCapture(image);
    int32_t outSize = encoder->encode(encoder, input);
    
    fwrite((const char*)outBuff,1,outSize,file);
    fclose(file);

    delEncoderInput(input);
}


/*int main()
{
    jpegEncoderInit();
	CvCapture* capture = initCamera();

    int i;
    char path[30];
    char suffix[5];
	for(i=0;i<50000;i++)
	{
        strcpy(path,"images/");
        sprintf(suffix,"%d",i);
        strcat(path,suffix);
        
		storeToFile(cvQueryFrame( capture ),path);
	}


	jpegEncoderTeardown();

	return 0;
}*/





