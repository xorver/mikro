
#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "main.h"

typedef struct DiscreteCosineTransform
{
	void (*transformMCU)(struct DiscreteCosineTransform* self, real* MCU);
}
DiscreteCosineTransform;

DiscreteCosineTransform* newDiscreteCosineTransform();
void delDiscreteCosineTransform(DiscreteCosineTransform*);

#endif
