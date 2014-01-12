
#ifndef _ENCODER_H_
#define _ENCODER_H_

#include "main.h"
#include "map.h"

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "input.h"
#include "buffer.h"
#include "huffman.h"


typedef struct JPEG
{
	uint8_t* image;
	uint32_t size;
}
JPEG;	

JPEG* encode(IplImage* frame);

void initInput(IplImage *frame);
void initTables();
void encodeCore(uint8_t* data, uint16_t height, uint16_t width, uint8_t components);
void writeHeaders();
void createHeaders(uint16_t height, uint16_t width, uint8_t header0[25], uint8_t header1[24], uint8_t header2[14]);



#endif
