
#include "input.h"

EncoderInput* newEncoderInput(Bitmap* bmp)
{
	uint32_t i,j;
	uint32_t w, h;
	uint8_t comp;

	EncoderInput* input = (EncoderInput*) malloc(sizeof(EncoderInput));

	input->rows = h = bmp->header->imageHeight;
	input->cols = w = bmp->header->imageWidth;

	input->components = comp = 3;

	input->data = (uint8_t*) malloc(h * w * comp);

	for(i=0; i<h; ++i)
	{
		for(j=0; j<w; ++j)
		{
			input->data[i*(comp*w) + comp*j + 0] = bmp->data[i][j].r;
			input->data[i*(comp*w) + comp*j + 1] = bmp->data[i][j].g;
			input->data[i*(comp*w) + comp*j + 2] = bmp->data[i][j].b;
		}
	}

	return input;
}

void delEncoderInput(EncoderInput* input)
{
	free(input->data);
	free(input);
}
