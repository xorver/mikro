
#ifndef _JPEGENCODER_H_
#define _JPEGENCODER_H_

#include "Global.h"

#include "EncoderInput.h"
#include "BitBuffer.h"
#include "HuffmanEncoder.h"

typedef struct JpegEncoder
{
	uint8_t* _outBuffer;
	BitBuffer* _buffer;
	HuffmanEncoder* _huffman;
	EncoderInput* _input;

	uint8_t quality;

	uint8_t YTable[64];
	uint8_t UVTable[64];

	real fdtbl_Y[64];
	real fdtbl_UV[64];

	uint32_t (*encode)(struct JpegEncoder* slef, EncoderInput* input);
}
JpegEncoder;

JpegEncoder* newJpegEncoder(uint8_t* outBuffer);
void delJpegEncoder(JpegEncoder* self);



#endif
