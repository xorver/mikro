
#ifndef _ENCODERINPUT_H_
#define _ENCODERINPUT_H_

#include "Global.h"
#include "Bitmap.h"

typedef struct EncoderInput
{
	uint8_t* data;

	uint32_t rows;
	uint32_t cols;

	uint8_t components;
}
EncoderInput;

EncoderInput* newEncoderInput(Bitmap* source);
void delEncoderInput(EncoderInput*);

#endif
