
#include "huffman.h"

#include "const.h"
#include "transform.h"


int32_t __encodeBlock(HuffmanEncoder* self, real* MCU, real* fdtbl, int32_t lastDC, uint16_t HTDC[256][2], uint16_t HTAC[256][2]);
void __quantizeZigzag(HuffmanEncoder* self, real* MCU, real* fdtbl);


HuffmanEncoder* newHuffmanEncoder(BitBuffer* outBuffer)
{
	HuffmanEncoder* encoder = (HuffmanEncoder*) malloc(sizeof(HuffmanEncoder));
	encoder->_buffer = outBuffer;
	encoder->_dct = newDiscreteCosineTransform();

	encoder->encodeBlock = __encodeBlock;
	encoder->quantizeZigzag = __quantizeZigzag;

	return encoder;
}

void delHuffmanEncoder(HuffmanEncoder* encoder)
{
	delDiscreteCosineTransform(encoder->_dct);
	free(encoder);
}

int32_t __encodeBlock(HuffmanEncoder* self, real* MCU, real* fdtbl, int32_t lastDC, uint16_t HTDC[256][2], uint16_t HTAC[256][2])
{
	BitBuffer* buffer = self->_buffer;

	int32_t diffDC;
	uint16_t bits[2]; /* BitBuffer->calcBits output */
	uint32_t lastNonZero;
	uint8_t zeroCount;
	uint8_t zero16run; /* zeroCount/16, each 16 zeroes coded separately */
	uint32_t i, j;

	int dataOff, lng, nrmarker;
	float v;


	self->_dct->transformMCU(self->_dct, MCU);

	self->quantizeZigzag(self, MCU, fdtbl);


	/* DC encoding */
	diffDC = self->MCU[0] - lastDC;
	if(diffDC == 0)
	{
		buffer->put(buffer, HTDC[0][0], HTDC[0][1]);
	}
	else
	{
		buffer->calcBits(buffer, diffDC, bits);
		buffer->put(buffer, HTDC[bits[1]][0], HTDC[bits[1]][1]);
		buffer->put(buffer, bits[0], bits[1]);
	}


	/* AC encoding */
	lastNonZero = 63;
	while(lastNonZero > 0 && self->MCU[lastNonZero] == 0) lastNonZero--;

	if(lastNonZero == 0)
	{
		/* no AC data */
		buffer->put(buffer, HTAC[0x00][0], HTAC[0x00][1]);
		return self->MCU[0];
	}

	/* start after DC part */
	for(i = 1; i <= lastNonZero; ++i)
	{
		zeroCount = 0;
		while(i <= lastNonZero && self->MCU[i] == 0) {
			zeroCount++;
			i++;
		}

		/*
		 * http://en.wikibooks.org/wiki/JPEG_-_Idea_and_Practice/The_Huffman_coding
		 * If there are m zeros before the non-zero AC number n and if the size of n is k,
		 * we combine these two numbers (being half bytes) to the byte val = m*16 + k, and it is this byte that is Huffman coded.
		 * This presupposes, however, that m and k really are half bytes (that is, <= 15).
		 * k is always <= 11, but there can be more than 15 zeros in a row, therefore,
		 * when a row of zeros has reached 15 and is followed by another zero, we must code these 16 zeros separately:
		 * the byte to be coded is val = 15*16 + 0 = 240 (called ZRL).
		 * [...]
		 * After the Huffman code has been written, the k binary digits of the non-zero AC number are written
		 * in the same way as for the DC (or rather the DIFF) numbers. 
		 */

		if(zeroCount >= 16)
		{
			zero16run = zeroCount>>4;

			for (j=0; j < zero16run; ++j)
				buffer->put(buffer, HTAC[0xF0][0], HTAC[0xF0][1]);

			zeroCount &= 15;
		}

		buffer->calcBits(buffer, self->MCU[i], bits);
		buffer->put(buffer, HTAC[(zeroCount<<4)+bits[1]][0], HTAC[(zeroCount<<4)+bits[1]][1]);
		buffer->put(buffer, bits[0], bits[1]);
	}

	if(lastNonZero != 63)
		buffer->put(buffer, HTAC[0x00][0], HTAC[0x00][1]);

	return self->MCU[0];
}

void __quantizeZigzag(HuffmanEncoder* self, real* MCU, real* fdtbl)
{
	uint32_t i;
	real tmp;

	for(i = 0; i < 64; ++i)
	{
		tmp = MCU[i]*fdtbl[i];
		self->MCU[zigzag_order[i]] = (int32_t)(tmp < 0 ? ceilf(tmp - 0.5f) : floorf(tmp + 0.5f));
	}
}
