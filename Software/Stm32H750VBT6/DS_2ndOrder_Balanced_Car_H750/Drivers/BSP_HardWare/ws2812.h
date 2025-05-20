#include "main.h"
#include "tim.h"
 
#define WS_H           250-1   // 1 ����Լ���ֵ
#define WS_L           80-1   // 0 ����Լ���ֵ
#define WS_REST        40   // ��λ�ź���������
#define LED_NUM         1   // WS2812�Ƹ���
#define DATA_LEN       24   // WS2812���ݳ��ȣ�������Ҫ24���ֽ�
#define WS2812_RST_NUM 50   // �ٷ���λʱ��Ϊ50us��40�����ڣ����������ʹ��50������
 
/****HALӲ���ӿ�*****/
#define TIM_Handler htim1
#define CHANNEL TIM_CHANNEL_1

void WS2812_LED_Set(uint16_t num,uint8_t color_R,uint8_t color_G,uint8_t color_B);
void WS2812_Init(void);
