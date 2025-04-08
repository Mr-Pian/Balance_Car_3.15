#ifndef __CONTROL_H__
#define __CONTROL_H__

#include "main.h"
#include "pid.h"
#include "tim.h"
/********************************************************************************/
//�ٶ�ת������k
//���㹫ʽΪ ��ֱ��*PI*����Ƶ��/���תһȦ�������ļ���ֵ
//��ǰϵ���Ĳ�����ӦΪ 0.067m PI 1000hz 30000

#define CM_S 0  //����Ϊ����ÿ��
#if CM_S
#define COUNT_SPEED_K 0.701622f 
#endif

#if !CM_S
#define COUNT_SPEED_K 0.00702f*0.5f //500Hz
#endif
/********************************************************************************/
typedef enum  //������״̬ö��
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
}imu_typedef;

typedef struct
{
	int16_t counter;  //����������ֵ
	float real_speed;  //����������ٶ�
	motor_status_typedef motor_status;  //���״̬������أ�
}motor_typedef;

typedef struct
{
	PID_TypeDef* pid_speed_L;  //�����ٶȱջ�
	PID_TypeDef* pid_speed_R;  //�����ٶȱջ�
	PID_TypeDef* pid_stand_angle;  //ֱ�����Ƕ��⻷
	PID_TypeDef* pid_stand_angle_speed;  //ֱ�������ٶ��ڻ�
	PID_TypeDef* pid_target_speed;  //Ŀ���ٶȻ��������Ŀ��Ƕȣ�
}PID_pack_typedef;

//���ṹ��
typedef struct
{
	imu_typedef* Imu;  //imu����
	motor_typedef* Motor_L;  //����
	motor_typedef* Motor_R;  //�ҵ��
	PID_pack_typedef* the_pid;  //����pid
}car_typedef;

void Get_Motor_Speed(car_typedef* hcar);
void Control_Init(car_typedef* car);
void Speed_CLoop_PID_Control(car_typedef* hcar);
void Stand_Angle_Speed_CLoop_PID_Control(car_typedef* hcar);
void Stand_Angle_CLoop_PID_Control(car_typedef* hcar);
void Target_Speed_CLoop_PID_Control(car_typedef* hcar);

extern car_typedef the_car;
extern PID_pack_typedef the_pid;

#endif
