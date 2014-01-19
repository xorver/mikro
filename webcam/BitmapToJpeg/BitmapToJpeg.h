#ifndef _BITMAP_TO_JPEG_H_
#define _BITMAP_TO_JPEG_H_

#include <opencv/cv.h>
#include <opencv/highgui.h>

void jpegEncoderInit();
void jpegEncoderTeardown();
void storeToFile(IplImage* image,char* path);

#endif