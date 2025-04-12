#ifndef __KEY_H
#define  __KEY_H
 
#include "main.h"
#include "gpio.h"
#include "control.h"
//按键引脚定义
#define KEY_OK_PIN 		HAL_GPIO_ReadPin(KEY_OK_GPIO_Port,KEY_OK_Pin)
#define KEY_BACK_PIN  HAL_GPIO_ReadPin(KEY_BACK_GPIO_Port,KEY_BACK_Pin)
#define KEY_UP_PIN 		HAL_GPIO_ReadPin(KEY_UP_GPIO_Port,KEY_UP_Pin)
#define KEY_DOWN_PIN  HAL_GPIO_ReadPin(KEY_DOWN_GPIO_Port,KEY_DOWN_Pin)
#define KEY_LEFT_PIN  HAL_GPIO_ReadPin(KEY_LEFT_GPIO_Port,KEY_LEFT_Pin)
#define KEY_RIGHT_PIN HAL_GPIO_ReadPin(KEY_RIGHT_GPIO_Port,KEY_RIGHT_Pin)



void key_scan(void);//按键扫描
//void key_init();




#endif

