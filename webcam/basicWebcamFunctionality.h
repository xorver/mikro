#ifndef _BWCF
#define _BWCF

#include "core/core_c.h"
#include "core/types_c.h"
#include "highgui/highgui_c.h"

void webcamInit();
char *takePicture();
//void startRecording();
//void stopRecording();
void webcamStop();

#endif
