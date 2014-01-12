
#ifndef _HEAD_H_
#define _HEAD_H_

#include "main.h"

typedef struct structure_DibHeader
{
	uint32_t rawDataOffset;
	uint32_t rawDataSize;

	uint32_t rawRowSize;
	uint32_t rowSize;
	uint32_t rowPadding;

	uint32_t	imageWidth;
	uint32_t	imageHeight;
	uint16_t	bitsPerPixel;
}
DibHeader;

DibHeader*	newDibHeader(uint8_t* rawHeader);
void		delDibHeader(DibHeader* header);

#endif
