#ifndef __CONTROL_H
#define __CONTROL_H
#include "main.h"
#include "pid.h"
#define LED(x)    			HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,!x)
#define LED_TOGGLE   		HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
#define MOTOR_MAX_THROTTLE 7000				//����������
#define MOTOR_MIN_THROTTLE 0 					//�����С����
#define MOTOR_MAX_DUTY  9999          //������ռ�ձ�
#define MOTOR_MIN_DUTY  0							//�����Сռ�ձ�

#define ROLL_TARGET_MAX_ANGLE  15.0f
#define ROLL_TARGET_MIN_ANGLE  -15.0f
#define PITCH_TARGET_MAX_ANGLE  15.0f
#define PITCH_TARGET_MIN_ANGLE -15.0f
#define YAW_TARGET_MAX_ANGLE  360.0f
#define YAW_TARGET_MIN_ANGLE  -360.0f

#define X_TARGET_MAX_SPEED 2.0f
#define Y_TARGET_MAX_SPEED 2.0f
/*���*/
typedef struct {
	int duty1;
	int duty2;
	int duty3;
	int duty4;
	int throttle;//����
	float volt_k1;//��ѹϵ��
	float volt_k2;//��ѹϵ��
	float volt_k3;//��ѹϵ��
	float volt_k4;//��ѹϵ��
}Motor_TypeDef;//����ṹ��
/* ������*/
typedef struct {
    float *barometer_height;//��ѹ�ƺ��θ߶�
    float *tof_height;//TOF���߶�
} HeightSensors_TypeDef;//�߶ȴ�����
typedef struct {
    float *gyro_x;
    float *gyro_y;
		float *gyro_z;
    float *acc_x;
    float *acc_y;
		float *acc_z;
    float *roll;
    float *pitch;
		float *yaw;
} AttitudeSensors_TypeDef;//��̬������
typedef struct {
    float *speed_x;
    float *speed_y;
    float *pos_x;
    float *pos_y;		
} PosSensors_TypeDef;//λ�ô�����
/*������*/
typedef struct {
    float Kp;
    float Ki;
    float Kd;
    float max_integral;
    float max_output;
} PIDParams_TypeDef;// PID�����ṹ��

typedef struct
{
	PID_TypeDef x;
	PID_TypeDef y;
	PID_TypeDef z;
}PID_XYZ_TypeDef;//����PID

// ������״̬
typedef enum {
    ALT_HOLD_DISABLED, //�ֶ�����
    BARO_MODE,				//��ѹ�ƶ��߿���
    TOF_MODE					//TOF���߿���
} HeightMode_TypeDef;

// ���������ṹ��
typedef struct
{
	PID_XYZ_TypeDef internal_pid;//�ڻ�pid
	PID_XYZ_TypeDef external_pid;//�⻷pid
	PIDParams_TypeDef internal_params;
  PIDParams_TypeDef external_params;
	AttitudeSensors_TypeDef sensor;
	float roll_target_angle;//roll��Ŀ��
	float pitch_target_angle;//pitch��Ŀ��
	float yaw_target_angle;//yaw��Ŀ��
	float roll_compensate;//roll�ǲ���
	float pitch_compensate;//pitch�ǲ���
}AttiudeController;	//��̬������
typedef struct {
		PID_TypeDef pid;
    PIDParams_TypeDef baro_params;
    PIDParams_TypeDef tof_params;
		HeightSensors_TypeDef sensor;
    HeightMode_TypeDef mode;
		uint8_t auto_height_control_isEnable;  //���߱�־
		float target_height;
		float target_altitude;
    int base_throttle;  // ������ͣ����
} HeightController;//�߶ȿ�����

typedef struct
{
		PID_TypeDef internal_pid_x;//�ڻ�
		PID_TypeDef internal_pid_y;
		PID_TypeDef external_pid_x;//�⻷
		PID_TypeDef external_pid_y;
		PosSensors_TypeDef sensor;
		uint8_t auto_pos_control_isEnable;  //�����־
	
}PositionController;//λ�ÿ�����

extern AttiudeController AttitudeControl;
extern HeightController HeightControl;
extern PositionController PositionControl;//λ�ÿ���
extern Motor_TypeDef aircraft_motor;//����ṹ��
void pid_control_init(void);
void pid_internal_control(void);
void pid_external_control(void);
void motor_init(void);
void motor_deinit(void);
void motor_speed_set(void);
void motor_throttle_control(void);
void motor_control(void);
void aircraft_flight_direction_control(void);
void aircraft_flight_Height_control(void);
uint8_t fatfs_PID_params_read(void);
uint8_t aircraft_protection(void);

#endif
