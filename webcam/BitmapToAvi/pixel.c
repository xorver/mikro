
#include "pixel.h"

Pixel* newPixel(uint8_t r, uint8_t g, uint8_t b)
{
	Pixel* pixel = (Pixel*) malloc(sizeof(Pixel));
	pixel->r = r;
	pixel->g = g;
	pixel->b = b;
	return pixel;
}

void delPixel(Pixel* pixel)
{
	free(pixel);
}
