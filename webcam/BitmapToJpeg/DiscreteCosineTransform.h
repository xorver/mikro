
#ifndef _DISCRETECOSINETRANSFORM_H_
#define _DISCRETECOSINETRANSFORM_H_

#include "Global.h"

typedef struct DiscreteCosineTransform
{
	void (*transformMCU)(struct DiscreteCosineTransform* self, real* MCU);
}
DiscreteCosineTransform;

DiscreteCosineTransform* newDiscreteCosineTransform();
void delDiscreteCosineTransform(DiscreteCosineTransform*);

#endif
