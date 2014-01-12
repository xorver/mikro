
#ifndef _PIXEL_H_
#define _PIXEL_H_

#include "main.h"

typedef struct struct_Pixel
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
}
Pixel;

Pixel*	newPixel(uint8_t r, uint8_t g, uint8_t b);
void	delPixel(Pixel* pixel);

#endif
