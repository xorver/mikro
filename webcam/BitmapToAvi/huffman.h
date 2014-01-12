
#ifndef _HUFFMAN_H_
#define _HUFFMAN_H_

#include "main.h"
#include "buffer.h"
#include "transform.h"

typedef struct HuffmanEncoder
{
	BitBuffer* _buffer;
	DiscreteCosineTransform* _dct;

	int32_t MCU[64];

	int32_t (*encodeBlock)(struct HuffmanEncoder* self, real* MCU, real* fdtbl, int32_t lastDC, uint16_t HTDC[256][2], uint16_t HTAC[256][2]);
	void (*quantizeZigzag)(struct HuffmanEncoder* self, real* MCU, real* fdtbl);
}
HuffmanEncoder;

HuffmanEncoder* newHuffmanEncoder(BitBuffer* outBuffer);
void delHuffmanEncoder(HuffmanEncoder* self);

#endif
