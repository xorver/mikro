#ifndef _BWCF
#define _BWCF
#include <opencv/cv.h>
#include <opencv/highgui.h>

#define PICTURE_PATH "/tmp/pictures/"
#define VIDEO_PATH "/tmp/videos/"

#define VIDEO_FPS 4

#define RECORDING_INTERVAL 1000
#define PIXEL_VALUE_EPSILON 30 
#define MINIMAL_DIFFERENCE_PERCENTAGE 10
#define EQL_IN_ROW_TO_STOP_RECORDING 5
#define NTH_FRAME_OF_RECORD 2 
//(640*480*3)
#define IMAGE_DATA_SIZE 921600


void webcamInit();
IplImage* takePicture();
int isDifferent(char* first, char* second);
void startRecording();
void stopRecording();
void webcamTeardown();
#endif
