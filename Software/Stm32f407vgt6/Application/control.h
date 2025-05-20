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

#define MAX_STAND_GYRO 400.0f //直立最大角速度
#define MAX_TURN_GYRO 1000.0f     //转向最大角速度

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
	float gyro_x;
	float gyro_y;
	float gyro_z;
	float acc_x;
	float acc_y;
	float acc_z;
	float temp;
	uint8_t is_ok;//IMU数据正常标志
}imu_typedef;

typedef struct
{
	int32_t pos_err;
	int32_t pos_err_offset;
	uint8_t state;
	uint32_t fps;
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
	PID_TypeDef* pid_turn_angle;  //转向环角度外环
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
	float run_speed_K;//运行速度动态调整
	float run_distance;//运行距离
	float start_yaw;//初始偏航角
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
