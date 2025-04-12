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

#define MAX_CONTROL_ANGLE 25.0f //姿态角度最大控制量
#define MAX_TURN_GYRO 50.0f     //转向最大角速度

typedef enum  //编码器状态枚举
{
	M_Off = 0,
	M_On
}motor_status_typedef;

typedef struct
{
	float roll;
	float pitch;
	float yaw;
	int16_t gyro_x;
	int16_t gyro_y;
	int16_t gyro_z;
	int16_t acc_x;
	int16_t acc_y;
	int16_t acc_z;
	int16_t temp;
	uint8_t is_ok;//IMU数据正常标志
}imu_typedef;

typedef struct
{
	int32_t pos_err;
	int32_t pos_err_offset;
	
}VS_TypeDef;//视觉

typedef struct
{
	int16_t counter;  //编码器计数值
	float real_speed;  //编码器测得速度
	motor_status_typedef motor_status;  //电机状态（开或关）
	int duty;//PWM占空比
}motor_typedef;

typedef struct
{
	uint8_t OK;
	uint8_t BACK;
	uint8_t LEFT;
	uint8_t RIGHT;
	uint8_t UP;
	uint8_t DOWN;
}key_typedef;

typedef struct
{
	PID_TypeDef* pid_speed_L;  //左轮速度闭环
	PID_TypeDef* pid_speed_R;  //右轮速度闭环
	PID_TypeDef* pid_turn_position;  //转向环位置外环
	PID_TypeDef* pid_turn_gyro;  //转向环角速度内环
	PID_TypeDef* pid_stand_angle;  //直立环角度外环
	PID_TypeDef* pid_stand_angle_speed;  //直立环角速度内环
	PID_TypeDef* pid_target_speed;  //目标速度环（输出是目标角度）
}PID_pack_typedef;

//主结构体
typedef struct
{
	imu_typedef* Imu;  //imu数据
	VS_TypeDef * vs;
	motor_typedef* Motor_L;  //左电机
	motor_typedef* Motor_R;  //右电机
	key_typedef* KEY;	//按键
	PID_pack_typedef* the_pid;  //所有pid
	float run_speed;//运行速度
}car_typedef;

void Get_Motor_Speed(car_typedef* hcar);
void Set_Motor_Speed(void);
void Control_Init(car_typedef* car);
void Speed_CLoop_PID_Control(car_typedef* hcar);
void Stand_Angle_Speed_CLoop_PID_Control(car_typedef* hcar);
void Stand_Angle_CLoop_PID_Control(car_typedef* hcar);
void Target_Speed_CLoop_PID_Control(car_typedef* hcar);
void Turn_Gyro_PID_Control(void);
void Turn_Position_PID_Control(void);

extern car_typedef the_car;
extern PID_pack_typedef the_pid;
extern uint32_t BL_CMD;//蓝牙指令
#endif
