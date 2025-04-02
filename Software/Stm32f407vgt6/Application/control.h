#ifndef __CONTROL_H__
#define __CONTROL_H__

#include "main.h"
#include "pid.h"
#include "tim.h"
/********************************************************************************/
//速度转换常数k
//计算公式为 轮直径*PI*控制频率/电机转一圈编码器的计数值
//当前系数的参数对应为 0.067m PI 1000hz 30000

#define CM_S 0  //开启为厘米每秒
#if CM_S
#define COUNT_SPEED_K 0.701622f 
#endif

#if !CM_S
#define COUNT_SPEED_K 0.00702f
#endif
/********************************************************************************/

typedef union
{
	float Data;
	uint8_t Data_bytes[4];
}float_data_typedef;

typedef enum  //编码器状态枚举
{
	M_Off = 0,
	M_On
}motor_status_typedef;

typedef struct
{
	uint16_t gyro_x;
	uint16_t gyro_y;
	uint16_t gyro_z;
	uint16_t acc_x;
	uint16_t acc_y;
	uint16_t acc_z;
	uint16_t temp;
	float roll;
	float pitch;
	float yaw;
}imu_typedef;

typedef struct
{
	int16_t counter;  //编码器计数值
	float real_speed;  //编码器测得速度
	motor_status_typedef motor_status;  //电机状态（开或关）
}motor_typedef;

typedef struct
{
	PID_TypeDef* pid_speed_L;  //左轮速度闭环
	PID_TypeDef* pid_speed_R;  //右轮速度闭环
}PID_pack_typedef;

//主结构体
typedef struct
{
	imu_typedef* Imu;  //imu数据
	motor_typedef* Motor_L;  //左电机
	motor_typedef* Motor_R;  //右电机
	PID_pack_typedef* the_pid;  //所有pid
}car_typedef;

void Get_Motor_Speed(car_typedef* hcar);
void Control_Init(car_typedef* car);
void Speed_CLoop_PID_Control(car_typedef* hcar);

extern car_typedef the_car;
extern PID_pack_typedef the_pid;

#endif
