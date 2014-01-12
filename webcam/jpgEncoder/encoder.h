
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


#endif
