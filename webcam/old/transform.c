
#include "transform.h"


void __transformMCU(DiscreteCosineTransform* dct, real* MCU);
void __transformVector8(real* d0, real* d1, real* d2, real* d3, real* d4, real* d5, real* d6, real* d7);


DiscreteCosineTransform* newDiscreteCosineTransform()
{
	DiscreteCosineTransform* dct = (DiscreteCosineTransform*) malloc(sizeof(DiscreteCosineTransform));
	dct->transformMCU = __transformMCU;
	return dct;
}

void delDiscreteCosineTransform(DiscreteCosineTransform* dct)
{
	free(dct);
}

void __transformMCU(DiscreteCosineTransform* dct, real* MCU)
{
	uint32_t off;

	/* rows */
	for(off = 0; off < 64; off += 8)
	{
		__transformVector8(
			&MCU[off+0],
			&MCU[off+1],
			&MCU[off+2],
			&MCU[off+3],
			&MCU[off+4],
			&MCU[off+5],
			&MCU[off+6],
			&MCU[off+7] );
	}

	/* columns */
	for(off = 0; off < 8; ++off)
	{
		__transformVector8(
			&MCU[off+0],
			&MCU[off+8],
			&MCU[off+16],
			&MCU[off+24],
			&MCU[off+32],
			&MCU[off+40],
			&MCU[off+48],
			&MCU[off+56] );
	}
}

void __transformVector8(real* d0, real* d1, real* d2, real* d3, real* d4, real* d5, real* d6, real* d7)
{
	/*
	 * pozyczone stad:
	 * ftp://ftp.yellowdoglinux.com/.2/nslu2/sources/svn/gcc.gnu.org/svn/gcc/branches/gcc-4_5-branch/libjava/classpath/gnu/javax/imageio/jpeg/DCT.java
	 * linia 242: DCT.idj_fast_fdct(float input[][])
	 */

	real tmp0 = *d0 + *d7;
	real tmp7 = *d0 - *d7;
	real tmp1 = *d1 + *d6;
	real tmp6 = *d1 - *d6;
	real tmp2 = *d2 + *d5;
	real tmp5 = *d2 - *d5;
	real tmp3 = *d3 + *d4;
	real tmp4 = *d3 - *d4;

	real tmp10 = tmp0 + tmp3;
	real tmp13 = tmp0 - tmp3;
	real tmp11 = tmp1 + tmp2;
	real tmp12 = tmp1 - tmp2;

	real z1, z2, z3, z4, z5, z11, z13;

	*d0 = tmp10 + tmp11;
	*d4 = tmp10 - tmp11;

	z1 = (tmp12 + tmp13) * 0.707106781f;
	*d2 = tmp13 + z1;
	*d6 = tmp13 - z1;

	tmp10 = tmp4 + tmp5;
	tmp11 = tmp5 + tmp6;
	tmp12 = tmp6 + tmp7;

	z5 = (tmp10 - tmp12) * 0.382683433f;
	z2 = tmp10 * 0.541196100f + z5;
	z4 = tmp12 * 1.306562965f + z5;
	z3 = tmp11 * 0.707106781f;

	z11 = tmp7 + z3;
	z13 = tmp7 - z3;

	*d5 = z13 + z2;
	*d3 = z13 - z2;
	*d1 = z11 + z4;
	*d7 = z11 - z4;
}
