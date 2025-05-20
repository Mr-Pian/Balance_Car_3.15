#include "ws2812.h"
#include "rng.h"
//使用PWM+DMA驱动ws2812时要注意定时器是多少位的，如果是16位计数器则DMA为半字，如果是32位计数器则DMA为全字 

//显存数组，长度为 灯的数量*24+复位周期
uint32_t WS2812_RGB_Buff[LED_NUM*DATA_LEN+WS2812_RST_NUM] = {0}; 

/**
* 函数：WS2812单灯设置函数
* 参数：num:灯的位置，R、G、B分别为三个颜色通道的亮度，最大值为255
* 作用：单独设置每一个WS2812的颜色
***/
void WS2812_LED_Set(uint16_t num,uint8_t color_R,uint8_t color_G,uint8_t color_B)
{
 uint32_t indexx=(num*(3*8));
 for (uint8_t i = 0;i < 8;i++)
 {
	//填充数组
	WS2812_RGB_Buff[indexx+i]      = (color_G << i) & (0x80)?WS_H:WS_L;
	WS2812_RGB_Buff[indexx+i + 8]  = (color_R << i) & (0x80)?WS_H:WS_L;
	WS2812_RGB_Buff[indexx+i + 16] = (color_B << i) & (0x80)?WS_H:WS_L;
 }
}

//WS2812初始化函数
void WS2812_Init()
{
	uint32_t rnd_r = 0,rnd_g=0,rnd_b=0;

	HAL_RNG_GenerateRandomNumber(&hrng,&rnd_r);
	HAL_RNG_GenerateRandomNumber(&hrng,&rnd_g);
	HAL_RNG_GenerateRandomNumber(&hrng,&rnd_b);

	//设置关闭所有灯
 for(int i=0;i<LED_NUM;i++)
 {
			
			WS2812_LED_Set(i,rnd_r>>28,rnd_g>>28,rnd_b>>28);
 }
 //作用：调用DMA将显存中的内容实时搬运至定时器的比较寄存器
	HAL_TIMEx_PWMN_Start_DMA(&TIM_Handler,CHANNEL,(uint32_t *)WS2812_RGB_Buff,sizeof(WS2812_RGB_Buff)/sizeof(uint16_t));//互补半字DMA
 //HAL_TIM_PWM_Start_DMA(&TIM_Handler,CHANNEL,(uint32_t *)WS2812_RGB_Buff,sizeof(WS2812_RGB_Buff)/sizeof(uint32_t));//正常全字DMA
	
}
