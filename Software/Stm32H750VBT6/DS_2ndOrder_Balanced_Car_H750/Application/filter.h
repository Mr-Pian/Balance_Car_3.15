#ifndef __FILTER_H__
#define __FILTER_H__
#include "main.h"

#define FIFO_LENGHTH 5  //fifoµÄ³¤¶È

typedef struct {
	
	uint16_t cnt;
	int16_t input;
	int16_t *average;

	uint8_t max_cnt;
}MovAverage;

int16_t AntiPulse_MovingAverage_Filter(MovAverage *_MovAverage);

extern MovAverage movaverage_L;
extern MovAverage movaverage_R;

#endif
