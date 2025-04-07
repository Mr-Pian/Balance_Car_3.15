#include "filter.h"

int16_t fifo_L[FIFO_LENGHTH];
int16_t fifo_R[FIFO_LENGHTH]; 

MovAverage movaverage_L=
{
	0,
	0,
	fifo_L,
	FIFO_LENGHTH
};

MovAverage movaverage_R=
{
	0,
	0,
	fifo_R,
	FIFO_LENGHTH
};


/*====================================================================================================*
**函数 : 抗干扰型滑动均值滤波
**功能 : 每次采样到一个新数据放入队列，对N个数据进行算术平均运算
**输入 : 
**輸出 : None
**备注 : None
**====================================================================================================*/
/*====================================================================================================*/
int16_t AntiPulse_MovingAverage_Filter(MovAverage *_MovAverage)
{
		uint8_t i;	
		int32_t sum=0;
		int16_t max=0;
		int16_t min=0xffff;
		_MovAverage->average[_MovAverage->cnt] = _MovAverage->input;	
		_MovAverage->cnt++;			
		if(_MovAverage->cnt==_MovAverage->max_cnt)
		{
			_MovAverage->cnt=0;
		}	
		for(i=0;i<_MovAverage->max_cnt;i++)
		{
				if(_MovAverage->average[i]>max)
						max = _MovAverage->average[i];
				else if(_MovAverage->average[i]<min)
						min = _MovAverage->average[i];
				sum += _MovAverage->average[i];
		}
	return ((sum-max-min)/(_MovAverage->max_cnt-2));                                    
}
