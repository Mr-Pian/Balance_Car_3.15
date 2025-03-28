#ifndef __CONTROL_H
#define __CONTROL_H
#include "main.h"
#include "pid.h"
#define LED(x)    			HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,!x)
#define LED_TOGGLE   		HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
#define MOTOR_MAX_THROTTLE 7000				//电机最大油门
#define MOTOR_MIN_THROTTLE 0 					//电机最小油门
#define MOTOR_MAX_DUTY  9999          //电机最大占空比
#define MOTOR_MIN_DUTY  0							//电机最小占空比

#define ROLL_TARGET_MAX_ANGLE  15.0f
#define ROLL_TARGET_MIN_ANGLE  -15.0f
#define PITCH_TARGET_MAX_ANGLE  15.0f
#define PITCH_TARGET_MIN_ANGLE -15.0f
#define YAW_TARGET_MAX_ANGLE  360.0f
#define YAW_TARGET_MIN_ANGLE  -360.0f

#define X_TARGET_MAX_SPEED 2.0f
#define Y_TARGET_MAX_SPEED 2.0f
/*电机*/
typedef struct {
	int duty1;
	int duty2;
	int duty3;
	int duty4;
	int throttle;//油门
	float volt_k1;//电压系数
	float volt_k2;//电压系数
	float volt_k3;//电压系数
	float volt_k4;//电压系数
}Motor_TypeDef;//电机结构体
/* 传感器*/
typedef struct {
    float *barometer_height;//气压计海拔高度
    float *tof_height;//TOF测距高度
} HeightSensors_TypeDef;//高度传感器
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
} AttitudeSensors_TypeDef;//姿态传感器
typedef struct {
    float *speed_x;
    float *speed_y;
    float *pos_x;
    float *pos_y;		
} PosSensors_TypeDef;//位置传感器
/*控制器*/
typedef struct {
    float Kp;
    float Ki;
    float Kd;
    float max_integral;
    float max_output;
} PIDParams_TypeDef;// PID参数结构体

typedef struct
{
	PID_TypeDef x;
	PID_TypeDef y;
	PID_TypeDef z;
}PID_XYZ_TypeDef;//三轴PID

// 控制器状态
typedef enum {
    ALT_HOLD_DISABLED, //手动控制
    BARO_MODE,				//气压计定高控制
    TOF_MODE					//TOF定高控制
} HeightMode_TypeDef;

// 控制器主结构体
typedef struct
{
	PID_XYZ_TypeDef internal_pid;//内环pid
	PID_XYZ_TypeDef external_pid;//外环pid
	PIDParams_TypeDef internal_params;
  PIDParams_TypeDef external_params;
	AttitudeSensors_TypeDef sensor;
	float roll_target_angle;//roll角目标
	float pitch_target_angle;//pitch角目标
	float yaw_target_angle;//yaw角目标
	float roll_compensate;//roll角补偿
	float pitch_compensate;//pitch角补偿
}AttiudeController;	//姿态控制器
typedef struct {
		PID_TypeDef pid;
    PIDParams_TypeDef baro_params;
    PIDParams_TypeDef tof_params;
		HeightSensors_TypeDef sensor;
    HeightMode_TypeDef mode;
		uint8_t auto_height_control_isEnable;  //定高标志
		float target_height;
		float target_altitude;
    int base_throttle;  // 基础悬停油门
} HeightController;//高度控制器

typedef struct
{
		PID_TypeDef internal_pid_x;//内环
		PID_TypeDef internal_pid_y;
		PID_TypeDef external_pid_x;//外环
		PID_TypeDef external_pid_y;
		PosSensors_TypeDef sensor;
		uint8_t auto_pos_control_isEnable;  //定点标志
	
}PositionController;//位置控制器

extern AttiudeController AttitudeControl;
extern HeightController HeightControl;
extern PositionController PositionControl;//位置控制
extern Motor_TypeDef aircraft_motor;//电机结构体
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
