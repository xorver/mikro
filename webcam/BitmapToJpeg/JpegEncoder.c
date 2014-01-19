
#include "JpegEncoder.h"

#include "Tables.h"


uint32_t __encode(JpegEncoder*, EncoderInput*);
void __encodeCore(JpegEncoder* self, uint8_t* rawData, uint16_t height, uint16_t width, uint8_t components);
void __initTables(JpegEncoder* self);
void __writeHeaders(JpegEncoder* self);
void __createHeaders(uint16_t height, uint16_t width, uint8_t header0[25], uint8_t header1[24], uint8_t header2[14]);


JpegEncoder* newJpegEncoder(uint8_t* outBuffer)
{
	JpegEncoder* encoder = (JpegEncoder*) malloc(sizeof(JpegEncoder));

	encoder->_outBuffer = outBuffer;

	encoder->quality = 50;
	encoder->quality = encoder->quality < 1 ? 1 : encoder->quality > 100 ? 100 : encoder->quality;
	encoder->quality = encoder->quality < 50 ? 5000 / encoder->quality : 200 - encoder->quality * 2;


	encoder->encode = __encode;

	__initTables(encoder);

	return encoder;
}

void delJpegEncoder(JpegEncoder* self)
{
	free(self);
}


uint32_t __encode(JpegEncoder* self, EncoderInput* input)
{
	uint32_t buffLen;

	self->_input = input;
	self->_buffer = newBitBuffer(self->_outBuffer);
	self->_huffman = newHuffmanEncoder(self->_buffer);

	__encodeCore(self, input->data, input->rows, input->cols, input->components);

	delHuffmanEncoder(self->_huffman);

	buffLen = self->_buffer->_length;
	delBitBuffer(self->_buffer);
	return buffLen;
}

void __encodeCore(JpegEncoder* self, uint8_t* data, uint16_t height, uint16_t width, uint8_t components)
{
	BitBuffer* buffer = self->_buffer;
	HuffmanEncoder* huffman = self->_huffman;

	real yMCU[64], uMCU[64], vMCU[64];
	int32_t yDC, uDC, vDC;

	int i;
	int row, col, k;
	int x, y, pos, p;
	
	int ofsG, ofsB;
	float r, g, b;


	__writeHeaders(self);


	/* encode MCUs */

	yDC = uDC = vDC = 0;

	ofsG = components > 1 ? 1 : 0;
	ofsB = components > 1 ? 2 : 0;

	for(y = 0; y < height; y += 8)
	{
		for(x = 0; x < width; x += 8)
		{
			/* got MCU */
			for(row = y, pos = 0; row < y+8; ++row)
			{
				for(col = x; col < x+8; ++col, ++pos)
				{
					p = row*width*components + col*components;

					/* size not divisible by 8 */
					if(row >= height) p -= width*components*(row+1 - height);
					if(col >= width)  p -= components*(col+1 - width);

					r = data[p+0];
					g = data[p+ofsG];
					b = data[p+ofsB];

					yMCU[pos] = +0.29900f*r + 0.58700f*g + 0.11400f*b - 128;
					uMCU[pos] = -0.16874f*r - 0.33126f*g + 0.50000f*b;
					vMCU[pos] = +0.50000f*r - 0.41869f*g - 0.08131f*b;
				}
			}

			yDC = huffman->encodeBlock(huffman, yMCU, self->fdtbl_Y,  yDC, YDC_HT,  YAC_HT);
			uDC = huffman->encodeBlock(huffman, uMCU, self->fdtbl_UV, uDC, UVDC_HT, UVAC_HT);
			vDC = huffman->encodeBlock(huffman, vMCU, self->fdtbl_UV, vDC, UVDC_HT, UVAC_HT);
		}
	}

	buffer->put(buffer, 0x7F, 7);

	/* EOI */
	buffer->rawPut(buffer, 0xFF);
	buffer->rawPut(buffer, 0xD9);
}


void __initTables(JpegEncoder* self)
{
	uint32_t i, j;
	uint32_t row, col;
	int32_t tmp;

	for(i = 0; i < 64; ++i)
	{
		tmp = (YQT[i] * self->quality + 50)/100;
		self->YTable[zigzag_order[i]] = tmp < 1 ? 1 : tmp > 255 ? 255 : tmp;
		tmp = (UVQT[i] * self->quality + 50)/100;
		self->UVTable[zigzag_order[i]] = tmp < 1 ? 1 : tmp > 255 ? 255 : tmp;
	}
	
	tmp = 0;
	for(row = 0; row < 8; ++row) {
		for(col = 0; col < 8; ++col) {
			self->fdtbl_Y[tmp]  = 1 / (self->YTable [zigzag_order[tmp]] * aasf[row] * aasf[col]);
			self->fdtbl_UV[tmp] = 1 / (self->UVTable[zigzag_order[tmp]] * aasf[row] * aasf[col]);
			tmp++;
		}
	}
}

void __writeHeaders(JpegEncoder* self)
{
	BitBuffer* buffer = self->_buffer;

	uint8_t header0[25];
	uint8_t header1[24];
	uint8_t header2[14];

	/* fill arrays */
	__createHeaders(self->_input->rows, self->_input->cols, header0, header1, header2);

	/* store data */

	buffer->rawWrite(buffer,	header0,		sizeof(header0[0]),			sizeof(header0)			);
	buffer->rawWrite(buffer,	self->YTable,	sizeof(self->YTable[0]),	sizeof(self->YTable)	);

	buffer->rawPut(buffer,		0x01);

	buffer->rawWrite(buffer,	self->UVTable,	sizeof(self->UVTable[0]),	sizeof(self->UVTable)	);
	buffer->rawWrite(buffer,	header1,		sizeof(header1[0]),			sizeof(header1)			);

	buffer->rawWrite(buffer,	std_dc_luminance_nrcodes+1,		sizeof(std_dc_luminance_nrcodes[0]),	sizeof(std_dc_luminance_nrcodes)-1	);
	buffer->rawWrite(buffer,	std_dc_luminance_values,		sizeof(std_dc_luminance_values[0]),		sizeof(std_dc_luminance_values)		);

	buffer->rawPut(buffer,		0x10);

	buffer->rawWrite(buffer,	std_ac_luminance_nrcodes+1,		sizeof(std_ac_luminance_nrcodes[0]),	sizeof(std_ac_luminance_nrcodes)-1	);
	buffer->rawWrite(buffer,	std_ac_luminance_values,		sizeof(std_ac_luminance_values[0]),		sizeof(std_ac_luminance_values)		);

	buffer->rawPut(buffer,		0x01);

	buffer->rawWrite(buffer,	std_dc_chrominance_nrcodes+1,	sizeof(std_dc_chrominance_nrcodes[0]),	sizeof(std_dc_chrominance_nrcodes)-1	);
	buffer->rawWrite(buffer,	std_dc_chrominance_values,		sizeof(std_dc_chrominance_values[0]),	sizeof(std_dc_chrominance_values)		);

	buffer->rawPut(buffer,		0x11);

	buffer->rawWrite(buffer,	std_ac_chrominance_nrcodes+1,	sizeof(std_ac_chrominance_nrcodes[0]),	sizeof(std_ac_chrominance_nrcodes)-1	);
	buffer->rawWrite(buffer,	std_ac_chrominance_values,		sizeof(std_ac_chrominance_values[0]),	sizeof(std_ac_chrominance_values)		);

	buffer->rawWrite(buffer,	header2,		sizeof(header2[0]),		sizeof(header2)		);
}

void __createHeaders(uint16_t height, uint16_t width, uint8_t header0[25], uint8_t header1[24], uint8_t header2[14])
{
	header0[0] = 0xFF;
	header0[1] = 0xD8;
	header0[2] = 0xFF;
	header0[3] = 0xE0;
	header0[4] = 0x00;
	header0[5] = 0x10;
	header0[6] = 'J';
	header0[7] = 'F';
	header0[8] = 'I';
	header0[9] = 'F';
	header0[10] = 0x00;
	header0[11] = 0x01;
	header0[12] = 0x01;
	header0[13] = 0x00;
	header0[14] = 0x00;
	header0[15] = 0x01;
	header0[16] = 0x00;
	header0[17] = 0x01;
	header0[18] = 0x00;
	header0[19] = 0x00;
	header0[20] = 0xFF;
	header0[21] = 0xDB;
	header0[22] = 0x00;
	header0[23] = 0x84;
	header0[24] = 0x00;

	header1[0] = 0xFF;
	header1[1] = 0xC0;
	header1[2] = 0x00;
	header1[3] = 0x11;
	header1[4] = 0x08;
	header1[5] = height >> 8;
	header1[6] = height &  0xFF;
	header1[7] =  width >> 8;
	header1[8] =  width &  0xFF;
	header1[9] = 0x03;
	header1[10] = 0x01;
	header1[11] = 0x11;
	header1[12] = 0x00;
	header1[13] = 0x02;
	header1[14] = 0x11;
	header1[15] = 0x01;
	header1[16] = 0x03;
	header1[17] = 0x11;
	header1[18] = 0x01;
	header1[19] = 0xFF;
	header1[20] = 0xC4;
	header1[21] = 0x01;
	header1[22] = 0xA2;
	header1[23] = 0x00;

	header2[0] = 0xFF;
	header2[1] = 0xDA;
	header2[2] = 0x00;
	header2[3] = 0x0C;
	header2[4] = 0x03;
	header2[5] = 0x01;
	header2[6] = 0x00;
	header2[7] = 0x02;
	header2[8] = 0x11;
	header2[9] = 0x03;
	header2[10] = 0x11;
	header2[11] = 0x00;
	header2[12] = 0x3F;
	header2[13] = 0x00;
}
