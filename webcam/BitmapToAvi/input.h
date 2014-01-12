
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"
#include "map.h"

typedef struct EncoderInput
{
	/*
		data = [rows * cols * components]

		component=0 Y
		component=1 Cb (U)
		component=2 Cr (V)
	*/

	uint8_t* data;

	uint32_t rows;
	uint32_t cols;

	uint8_t components;
}
EncoderInput;

EncoderInput* newEncoderInput(Bitmap* source);
void delEncoderInput(EncoderInput*);

#endif
