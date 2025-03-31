#ifndef __CONTROL_H__
#define __CONTROL_H__

#define WHEEL_SIZE 0.067  //轮子直径，单位是m
#define PI 3.14159 

#define GET_MOTOR_SPEED_FRE 1000
#define SPEED_CLOOP_CONTROL_FRE 1000

typedef enum
{
	M_Off,
	M_Foward,
	M_Backward
}motor_status_typedef;

typedef struct
{
	float L_Real_Speed;
	float R_Real_Speed;
	float L_Goal_Speed;
	float R_Goal_Speed;
	motor_status_typedef L_Motor_Status;
	motor_status_typedef R_Motor_Status;
}car_typedef;

void Get_Motor_Speed(car_typedef* hcar);

extern car_typedef the_car;
#endif