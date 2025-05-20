#include "main.h"
#include "tim.h"
 
#define WS_H           250-1   // 1 码相对计数值
#define WS_L           80-1   // 0 码相对计数值
#define WS_REST        40   // 复位信号脉冲数量
#define LED_NUM         1   // WS2812灯个数
#define DATA_LEN       24   // WS2812数据长度，单个需要24个字节
#define WS2812_RST_NUM 50   // 官方复位时间为50us（40个周期），保险起见使用50个周期
 
/****HAL硬件接口*****/
#define TIM_Handler htim1
#define CHANNEL TIM_CHANNEL_1

void WS2812_LED_Set(uint16_t num,uint8_t color_R,uint8_t color_G,uint8_t color_B);
void WS2812_Init(void);
