#include "encoder.h"
#include "const.h"


uint8_t quality = 100;
uint8_t YTable[64];
uint8_t UVTable[64];
real fdtbl_Y[64];
real fdtbl_UV[64];

uint8_t* outBuffer;
BitBuffer* _buffer;
HuffmanEncoder* _huffman;
EncoderInput* _input;

void encoderTeardown(){
	delBitBuffer(_buffer);
	delHuffmanEncoder(_huffman);
	free(outBuffer);
	free(_input);
}


void encoderInit(){
	outBuffer = (uint8_t*)malloc(30000);	
	//_buffer = newBitBuffer(outBuffer);
	//_huffman = newHuffmanEncoder(_buffer);
	_input = (EncoderInput*) malloc(sizeof(EncoderInput));
	initTables();
}


void encode(IplImage* frame,JPEG* result){
	
	int i;

	
	initInput(frame);
	//initTables();
	_buffer = newBitBuffer(outBuffer);
	_huffman = newHuffmanEncoder(_buffer);
	encodeCore(_input->data, _input->rows, _input->cols, _input->components);

	
	result->size = _buffer->_length;
	for(i=0; i < result->size; i++)
		result->image[i] = outBuffer[i];
	return result;
}

void initTables()
{
	uint32_t i, j;
	uint32_t row, col;
	int32_t tmp;

	for(i = 0; i < 64; ++i)
	{
		tmp = (YQT[i] * quality + 50)/100;
		YTable[zigzag_order[i]] = tmp < 1 ? 1 : tmp > 255 ? 255 : tmp;
		tmp = (UVQT[i] * quality + 50)/100;
		UVTable[zigzag_order[i]] = tmp < 1 ? 1 : tmp > 255 ? 255 : tmp;
	}
	
	tmp = 0;
	for(row = 0; row < 8; ++row) {
		for(col = 0; col < 8; ++col) {
			fdtbl_Y[tmp]  = 1 / (YTable [zigzag_order[tmp]] * aasf[row] * aasf[col]);
			fdtbl_UV[tmp] = 1 / (UVTable[zigzag_order[tmp]] * aasf[row] * aasf[col]);
			tmp++;
		}
	}
}

void initInput(IplImage *frame){
	//_input = (EncoderInput*) malloc(sizeof(EncoderInput));
	int32_t w, h, ch;
	uint8_t* rawData;
	uint32_t x, y, i;
	
	_input->cols			= w		= frame->width;
	_input->rows			= h		= frame->height;
	_input->components	= ch	= frame->nChannels;

	rawData = _input->data = (uint8_t*) malloc(w*h*ch);

	i=0;
	for(y = 0; y < h; ++y)
	{
		for(x = 0; x < w; ++x)
		{
			rawData[i++] = frame->imageData[ch*(y*w+x)+2];
			rawData[i++] = frame->imageData[ch*(y*w+x)+1];
			rawData[i++] = frame->imageData[ch*(y*w+x)+0];
		}
	}
}

void encodeCore(uint8_t* data, uint16_t height, uint16_t width, uint8_t components)
{
	real yMCU[64], uMCU[64], vMCU[64];
	int32_t yDC, uDC, vDC;

	int i;
	int row, col, k;
	int x, y, pos, p;
	
	int ofsG, ofsB;
	float r, g, b;


	writeHeaders();


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

			yDC = _huffman->encodeBlock(_huffman, yMCU, fdtbl_Y,  yDC, YDC_HT,  YAC_HT);
			uDC = _huffman->encodeBlock(_huffman, uMCU, fdtbl_UV, uDC, UVDC_HT, UVAC_HT);
			vDC = _huffman->encodeBlock(_huffman, vMCU, fdtbl_UV, vDC, UVDC_HT, UVAC_HT);
		}
	}

	_buffer->put(_buffer, 0x7F, 7);
	//buffer->flush(buffer);

	/* EOI */
	_buffer->rawPut(_buffer, 0xFF);
	_buffer->rawPut(_buffer, 0xD9);
}

void writeHeaders()
{
	uint8_t header0[25];
	uint8_t header1[24];
	uint8_t header2[14];

	/* fill arrays */
	createHeaders(_input->rows, _input->cols, header0, header1, header2);

	/* store data */

	_buffer->rawWrite(_buffer,	header0,		sizeof(header0[0]),			sizeof(header0)			);
	_buffer->rawWrite(_buffer,	YTable,	sizeof(YTable[0]),	sizeof(YTable)	);

	_buffer->rawPut(_buffer,		0x01);

	_buffer->rawWrite(_buffer,	UVTable,	sizeof(UVTable[0]),	sizeof(UVTable)	);
	_buffer->rawWrite(_buffer,	header1,		sizeof(header1[0]),			sizeof(header1)			);

	_buffer->rawWrite(_buffer,	std_dc_luminance_nrcodes+1,		sizeof(std_dc_luminance_nrcodes[0]),	sizeof(std_dc_luminance_nrcodes)-1	);
	_buffer->rawWrite(_buffer,	std_dc_luminance_values,		sizeof(std_dc_luminance_values[0]),		sizeof(std_dc_luminance_values)		);

	_buffer->rawPut(_buffer,		0x10);

	_buffer->rawWrite(_buffer,	std_ac_luminance_nrcodes+1,		sizeof(std_ac_luminance_nrcodes[0]),	sizeof(std_ac_luminance_nrcodes)-1	);
	_buffer->rawWrite(_buffer,	std_ac_luminance_values,		sizeof(std_ac_luminance_values[0]),		sizeof(std_ac_luminance_values)		);

	_buffer->rawPut(_buffer,		0x01);

	_buffer->rawWrite(_buffer,	std_dc_chrominance_nrcodes+1,	sizeof(std_dc_chrominance_nrcodes[0]),	sizeof(std_dc_chrominance_nrcodes)-1	);
	_buffer->rawWrite(_buffer,	std_dc_chrominance_values,		sizeof(std_dc_chrominance_values[0]),	sizeof(std_dc_chrominance_values)		);

	_buffer->rawPut(_buffer,		0x11);

	_buffer->rawWrite(_buffer,	std_ac_chrominance_nrcodes+1,	sizeof(std_ac_chrominance_nrcodes[0]),	sizeof(std_ac_chrominance_nrcodes)-1	);
	_buffer->rawWrite(_buffer,	std_ac_chrominance_values,		sizeof(std_ac_chrominance_values[0]),	sizeof(std_ac_chrominance_values)		);

	_buffer->rawWrite(_buffer,	header2,		sizeof(header2[0]),		sizeof(header2)		);
}

void createHeaders(uint16_t height, uint16_t width, uint8_t header0[25], uint8_t header1[24], uint8_t header2[14])
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


