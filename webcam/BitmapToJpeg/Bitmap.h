
#ifndef _BITMAP_H_
#define _BITMAP_H_

#include "Global.h"

#include "Pixel.h"
#include "DibHeader.h"

typedef struct struct_Bitmap
{
	DibHeader* header;
	Pixel** data;
}
Bitmap;

Bitmap* newBitmap(char* fileName);
void	delBitmap(Bitmap* bitmap);

#endif
