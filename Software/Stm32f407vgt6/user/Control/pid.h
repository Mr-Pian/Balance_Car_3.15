/**
  ******************************************************************************
  * @file		 pid.h
  * @author  aBo
  * @version V1.0.0
  * @date    2025/2/24
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#ifndef _PID_H
#define _PID_H

#include "stdint.h"

typedef enum
{

	PID_Position,
	PID_Speed,
	
}PID_ID;

typedef struct _PID_TypeDef
{
	PID_ID id;
	uint8_t enable;						//使能开关
	float target;							//目标值
	float lastNoneZeroTarget;
	float kp;
	float ki;
	float kd;
	
	float   measure;					//测量值
	float   err;							//误差
	float   last_err;      		//上次误差
     float   far_err;              //上上次误差
	
	float pout;
	float iout;
	float dout;
	
	float output;						//本次输出
	float last_output;			//上次输出
	
	float MaxOutput;				//输出限幅
	float IntegralLimit;		//积分限幅
	float DeadBand;			  //死区（绝对值）
	float ControlPeriod;		//控制周期
	
	uint32_t thistime;
	uint32_t lasttime;
	uint32_t dtime;	
	
	void (*f_param_init)(volatile struct _PID_TypeDef *pid,  //PID参数初始化
				PID_ID id,
				uint16_t maxOutput,
				uint16_t integralLimit,
				float deadband,
				uint16_t controlPeriod,    
				int16_t  target,
				float kp,
				float ki,
				float kd);
				   
	void (*f_pid_reset)(volatile struct _PID_TypeDef *pid, float kp,float ki, float kd);		//pid三个参数修改
	float (*f_cal_pid)(volatile struct _PID_TypeDef *pid, float measure);   //pid计算
}PID_TypeDef;

void pid_init(volatile PID_TypeDef* pid);
void pid_enable(volatile PID_TypeDef * pid,uint8_t enable);
#endif
