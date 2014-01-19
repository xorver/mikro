
#include "DibHeader.h"

DibHeader* newDibHeader(uint8_t* rawHeader)
{
	DibHeader* header = (DibHeader*) malloc(sizeof(DibHeader));

	memcpy(	&(header->rawDataOffset),	rawHeader + 0x0A, sizeof(uint32_t));
	memcpy(	&(header->imageWidth),		rawHeader + 0x12, sizeof(uint32_t));
	memcpy(	&(header->imageHeight),		rawHeader + 0x16, sizeof(uint32_t));
	memcpy(	&(header->bitsPerPixel),	rawHeader + 0x1C, sizeof(uint16_t));
	memcpy(	&(header->rawDataSize),		rawHeader + 0x22, sizeof(uint32_t));

	header->rawRowSize = ((header->bitsPerPixel*header->imageWidth + 31) / 32) * 4;
	header->rowSize = header->imageWidth * (header->bitsPerPixel / 8);
	header->rowPadding = header->rawRowSize - header->rowSize;

	return header;
}

void delDibHeader(DibHeader* header)
{
	free(header);
}