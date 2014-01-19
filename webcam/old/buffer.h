
#ifndef _BUFFER_H_
#define _BUFFER_H_

#include "main.h"

typedef struct BitBuffer
{
	uint8_t* _buffer;
	uint8_t* _insert;
	uint32_t _length;

	uint8_t _preBuffer;
	uint8_t _preLength;

	void (*put)(struct BitBuffer* self, int32_t data, uint8_t bitsNum);
	void (*flush)(struct BitBuffer* self);

	void (*rawPut)(struct BitBuffer* self, uint8_t bit);
	void (*rawWrite)(struct BitBuffer* self, void* data, uint32_t size, uint32_t count);

	void (*calcBits)(struct BitBuffer* self, int32_t value, uint16_t bits[2]);
}
BitBuffer;

BitBuffer* newBitBuffer(uint8_t* buffer);
void delBitBuffer(BitBuffer* self);

#endif
