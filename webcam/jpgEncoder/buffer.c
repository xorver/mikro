
#include "buffer.h"


void __put(BitBuffer*, int32_t, uint8_t);
void __flush(BitBuffer*);

void __rawPut(BitBuffer*, uint8_t);
void __rawWrite(BitBuffer*, void*, uint32_t, uint32_t);

void __calcBits(BitBuffer*, int32_t, uint16_t[2]);


BitBuffer* newBitBuffer(uint8_t* buffer)
{
	BitBuffer* buff = (BitBuffer*) malloc(sizeof(BitBuffer));

	buff->_buffer = buffer;
	buff->_insert = buffer;
	buff->_length = 0;

	buff->_preBuffer = 0;
	buff->_preLength = 0;

	buff->put = __put;
	buff->flush = __flush;

	buff->calcBits = __calcBits;

	buff->rawPut = __rawPut;
	buff->rawWrite = __rawWrite;

	return buff;
}

void delBitBuffer(BitBuffer* self)
{
	free(self);
}

void __putByte(BitBuffer* self, uint8_t byte)
{
	*(self->_insert) = byte;
	(self->_insert)++;
	(self->_length)++;

	if(byte == 0xFF)
		__putByte(self, 0);
}

void __putBit(BitBuffer* self, uint8_t bit)
{
	self->_preBuffer <<= 1;
	self->_preBuffer |= bit & 1;
	(self->_preLength)++;

	if(self->_preLength == 8)
	{
		__putByte(self, self->_preBuffer);
		self->_preBuffer = 0;
		self->_preLength = 0;
	}
}

void __put(BitBuffer* self, int32_t data, uint8_t bitsNum)
{
	data <<= (32-bitsNum);

	while(bitsNum-- > 0)
	{
		__putBit(self, (data & (1 << 31)) >> 31);
		data <<= 1;
	}
}

void __flush(BitBuffer* self)
{
	if(self->_preLength > 0)
		self->put(self, 0, 8-self->_preLength);
}

void __calcBits(BitBuffer* self, int32_t value, uint16_t bits[2])
{
	/*
	 * http://en.wikibooks.org/wiki/JPEG_-_Idea_and_Practice/The_Huffman_coding
	 * The integer the binary digit expression of which follows a Huffman code, can be negative,
	 * and (as explained in part one) we do not need an extra bit to indicate this:
	 * the digit expression will always begins with 1 and we can write 0 instead of the 1.
	 * At the decoding of the sequence, the start with 0 will then show that the number is negative,
	 * and 1 followed by the rest of the digits will be the binary expression of the numerical value. [...]
	 */

	int16_t abs = value < 0 ? -value : value;
	value = value < 0 ? value-1 : value;

	bits[1] = 1;
	while(abs >>= 1) ++bits[1];

	bits[0] = value & ((1<<bits[1])-1);
}



/*
 * direct input
 */

void __rawPut(BitBuffer* self, uint8_t byte)
{
	*(self->_insert) = byte;
	(self->_insert)++;
	(self->_length)++;
}

void __rawWrite(BitBuffer* self, void* data, uint32_t size, uint32_t count)
{
	//memcpy(self->_insert, data, size*count);
	//self->_insert += size;
	//self->_length += size;

	uint32_t totalSize = size*count;
	uint8_t* rawData = (uint8_t*) data;

	while(totalSize-- > 0)
		self->rawPut(self, *rawData++);
}
