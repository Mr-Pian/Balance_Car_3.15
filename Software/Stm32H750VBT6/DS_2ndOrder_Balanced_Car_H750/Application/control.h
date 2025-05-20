#ifndef __CONTROL_H__
#define __CONTROL_H__

#include "main.h"
#include "pid.h"
#include "tim.h"

/********����������*******************/
#define MAX_MOTOR_DUTY 10000.0f //������ռ�ձ�(��������)
#define MIN_MOTOR_DUTY -10000.0f //�����Сռ�ձ�
#define MAX_MOTOR_SPEED 1000.0f //�������ٶ�(cm/s)
#define MIN_MOTOR_SPEED -1000.0f //�����С�ٶ�

/********************************************************************************/
//�ٶ�ת������k
//���㹫ʽΪ ��ֱ��*PI*����Ƶ��/���תһȦ�������ļ���ֵ
//��ǰϵ���Ĳ�����ӦΪ 0.067m PI 1000hz 30000

#define CM_S 1  //����Ϊ����ÿ��
#if CM_S
	#define COUNT_SPEED_K 0.0186168f 
#else
	#define COUNT_SPEED_K 0.0001862f
#endif

/********************************************************************************/

#define MAX_STAND_GYRO 1000.0f //ֱ�������ٶ�
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
	PID_TypeDef pid_Lspeed;//�����ٶȱջ�
	PID_TypeDef pid_Rspeed;//�����ٶȱջ�
	PID_TypeDef pid_turn_gyro;//ת�򻷽��ٶ��ڻ�
	PID_TypeDef pid_turn_angle;//һ���ò���
	PID_TypeDef pid_turn_position;//ת��λ���⻷
	PID_TypeDef pid_car_angle;//С���ǶȻ�
	PID_TypeDef pid_car_gyro;//ֱ�������ٶ��ڻ�
	PID_TypeDef pid_ptm_angle;//��λ���ǶȻ�
	PID_TypeDef pid_target_distance;//�ٶȻ�
}PID_pack_typedef;

//���ṹ��
typedef struct
{
	imu_typedef IMU;  //imu����
	VS_TypeDef  VS;
	motor_typedef MOTOR_L;  //����
	motor_typedef MOTOR_R;  //�ҵ��
	key_typedef KEY;	//����
	uint32_t BL_CMD;//����ָ��
	PID_pack_typedef PID;  //����pid
	float run_speed;//�����ٶ�
	float run_distance;//���о���
	float target_speed;//Ŀ���ٶ�
	float target_speed_K;//�����ٶȶ�̬����
	float start_yaw;//��ʼƫ����
	float battery_voltage;//��ص�ѹ
	float potentiometer_voltage;//��λ����ѹ
	float potentiometer_voltage_rate;//��λ����ѹ�仯��
	float car_angle_offset;//С���Ƕ�ƫ��
	float ptm_angle_offset;//��λ���Ƕ�ƫ��
	float ptm_angle_k;
}car_typedef;

void Get_Motor_Speed(void);
void Set_Motor_Speed(void);
float get_battery_voltage(void);
float get_potentiometer_voltage(void);
void Control_Init(void);
void MotorSpeed_PID_Control(void);
void TargetDistance_PID_Control(void);
void CarAngle_PID_Control(void);
void CarGyro_PID_Control(void);
void Turn_Gyro_PID_Control(void);
void Turn_Position_PID_Control(void);
void potAngle_PID_Control(void);

extern car_typedef the_car;
#endif
