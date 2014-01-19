
#include "Bitmap.h"

Bitmap* newBitmap(char* fileName)
{
	FILE* fp = fopen(fileName, "r");

	Bitmap* bmp = (Bitmap*) malloc(sizeof(Bitmap));
	uint8_t* rawHeader;
	uint8_t* rawData;

	uint32_t i, row, col;
	
	fseek(fp, 0, SEEK_SET);
	rawHeader = (uint8_t*) malloc(0x36*sizeof(uint8_t));
	fread(rawHeader, sizeof(uint8_t), 0x36, fp);
	bmp->header = newDibHeader(rawHeader);
	
	fseek(fp, bmp->header->rawDataOffset, SEEK_SET);
	rawData = (uint8_t*) malloc(bmp->header->rawDataSize);
	fread(rawData, sizeof(uint8_t), bmp->header->rawDataSize, fp);
	fclose(fp);

	bmp->data = (Pixel**) malloc(bmp->header->imageHeight*sizeof(Pixel*));
	for(i=0; i < bmp->header->imageHeight; ++i)
		bmp->data[i] = (Pixel*) malloc(bmp->header->imageWidth*sizeof(Pixel));

	i = 0;
	for(row = bmp->header->imageHeight; row > 0; --row)
	{
		for(col = 0; col < bmp->header->imageWidth; ++col)
		{
			bmp->data[row-1][col].b = rawData[i];
			bmp->data[row-1][col].g = rawData[i+1];
			bmp->data[row-1][col].r = rawData[i+2];
			
			i += 3;
		}
		i += bmp->header->rowPadding;
	}

	free(rawHeader);
	free(rawData);

	return bmp;
}

void delBitmap(Bitmap* bmp)
{
	uint32_t row, col;

	for(row = 0; row < bmp->header->imageHeight; ++row)
		free(bmp->data[row]);
	free(bmp->data);

	delDibHeader(bmp->header);
	free(bmp);
}