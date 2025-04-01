#ifndef __CONTROL_H__
#define __CONTROL_H__

#include "main.h"
#include "pid.h"
#include "tim.h"

#define WHEEL_SIZE 0.067  //轮子直径，单位是m
#define PI 3.14159 
#define GET_MOTOR_SPEED_FRE 1000  //编码器采样频率，单位是hz
#define MAX_SPEED 50

typedef enum
{
	M_Off = 0,
	M_On
}motor_status_typedef;

typedef struct
{
	float gyro_x;
	float gyro_y;
	float gyro_z;
	float L_Speed;
	float R_Speed;
}sensor_typedef;

//主结构体
typedef struct
{
	sensor_typedef* Sensor_val;
	motor_status_typedef L_Motor_Status;
	motor_status_typedef R_Motor_Status;
	PID_TypeDef* Pid_speed_L;
	PID_TypeDef* Pid_speed_R;
}car_typedef;

void Get_Motor_Speed(car_typedef* hcar);
void Control_Init(car_typedef* car);
void Speed_CLoop_PID_Control(car_typedef* hcar);

extern car_typedef the_car;
extern PID_TypeDef pid_speed_L;
extern PID_TypeDef pid_speed_R;

#endif
