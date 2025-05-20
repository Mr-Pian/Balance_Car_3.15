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
#define COUNT_SPEED_K 0.00702f
#endif
/********************************************************************************/

#define MAX_STAND_GYRO 400.0f //ֱ�������ٶ�
#define MAX_TURN_GYRO 1000.0f     //ת�������ٶ�

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
	float gyro_x;
	float gyro_y;
	float gyro_z;
	float acc_x;
	float acc_y;
	float acc_z;
	float temp;
	uint8_t is_ok;//IMU����������־
}imu_typedef;

typedef struct
{
	int32_t pos_err;
	int32_t pos_err_offset;
	uint8_t state;
	uint32_t fps;
}VS_TypeDef;//�Ӿ�

typedef struct
{
	int16_t counter;  //����������ֵ
	float real_speed;  //����������ٶ�
	motor_status_typedef motor_status;  //���״̬������أ�
	int duty;//PWMռ�ձ�
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
	PID_TypeDef* pid_speed_L;  //�����ٶȱջ�
	PID_TypeDef* pid_speed_R;  //�����ٶȱջ�
	PID_TypeDef* pid_turn_position;  //ת��λ���⻷
	PID_TypeDef* pid_turn_angle;  //ת�򻷽Ƕ��⻷
	PID_TypeDef* pid_turn_gyro;  //ת�򻷽��ٶ��ڻ�
	PID_TypeDef* pid_stand_angle;  //ֱ�����Ƕ��⻷
	PID_TypeDef* pid_stand_angle_speed;  //ֱ�������ٶ��ڻ�
	PID_TypeDef* pid_target_speed;  //Ŀ���ٶȻ��������Ŀ��Ƕȣ�
}PID_pack_typedef;

//���ṹ��
typedef struct
{
	imu_typedef* Imu;  //imu����
	VS_TypeDef * vs;
	motor_typedef* Motor_L;  //����
	motor_typedef* Motor_R;  //�ҵ��
	key_typedef* KEY;	//����
	PID_pack_typedef* the_pid;  //����pid
	float run_speed;//�����ٶ�
	float run_speed_K;//�����ٶȶ�̬����
	float run_distance;//���о���
	float start_yaw;//��ʼƫ����
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
extern uint32_t BL_CMD;//����ָ��
#endif
