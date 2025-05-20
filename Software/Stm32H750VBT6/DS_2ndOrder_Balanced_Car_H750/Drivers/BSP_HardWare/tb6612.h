#ifndef __TB6612_H__
#define __TB6612_H__
#include "main.h"
//#include "control.h"
#include "usart.h"
#define MAX_MOTOR_DUTY  (3999)
#define MIN_MOTOR_DUTY  (-3999)
#define R 1
#define L 0
#define Both 2

#define Foward 0
#define Backward 1

int Motor_Start(uint8_t L_R);
int Motor_SetSpeed(int speed, uint8_t L_R);
int Motor_Stop(uint8_t L_R);

#endif

