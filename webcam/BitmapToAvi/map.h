
#ifndef _MAP_H_
#define _MAP_H_

#include "main.h"

#include "head.h"
#include "pixel.h"

typedef struct struct_Bitmap
{
	DibHeader* header;
	Pixel** data;
}
Bitmap;

Bitmap* newBitmap(char* fileName);
void	delBitmap(Bitmap* bitmap);

#endif
