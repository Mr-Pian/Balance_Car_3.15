/**
  ******************************************************************************
  * @file		 pid.h
  * @author  Ginger
  * @version V1.0.0
  * @date    2015/11/14
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  ******************************************************************************
  * 2024/7 
  *大疆PID库 增加了增量式PID的实现  
  *Modified by
  *UESTC-School Of Automation Engineering-Technology Association-Turtle
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#ifndef _PID_H
#define _PID_H

#include "stdint.h"

#define ABS(x) ((x>0)? x: -x) //死区绝对值

#define WEIGHTED_FILTERING 0  //是否打开权重滤波
#define WEIGHT 0.7f

typedef enum
{

	PID_Position,
	PID_Speed,
	
}PID_ID;

typedef struct _PID_TypeDef
{
	PID_ID id;
	
	float target;						//目标值
	float lastNoneZeroTarget;
	float kp;
	float ki;
	float kd;
	
	float   measure;				//测量值
	float   err;						//误差
	float   last_err;      	//上次误差
  float   far_err;        //上上次误差
	
	float pout;
	float iout;
	float dout;
	
	float output;						//本次输出
	float last_output;			//上次输出
	
	float MaxOutput;				//输出限幅
	float IntegralLimit;		//积分限幅
	float DeadBand;			    //死区（绝对值）
	float ControlPeriod;		//控制周期
	float  Max_Err;					//最大误差
	
	uint32_t thistime;
	uint32_t lasttime;
	uint8_t dtime;	
	
	void (*f_param_init)(volatile struct _PID_TypeDef *pid,  //PID参数初始化(pid模式，限幅，参数初值等)
				
	PID_ID id,
	uint16_t maxOutput,
	uint16_t integralLimit,
	float deadband,
  uint16_t controlPeriod,
	int16_t max_err,     
	int16_t  target,
  float kp,
	float ki,
	float kd);
				   
	void (*f_pid_reset)(volatile struct _PID_TypeDef *pid, float kp,float ki, float kd);		//单独修改P I D三个参数
	
	float (*f_cal_pid)(volatile struct _PID_TypeDef *pid, float measure);   //pid计算

}PID_TypeDef;

extern PID_TypeDef pid_speed;

void pid_init(volatile PID_TypeDef* pid);
#endif
