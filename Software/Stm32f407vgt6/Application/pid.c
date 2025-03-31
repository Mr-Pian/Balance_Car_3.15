/**
  ******************************************************************************
  * @file    pid.c
  * @author  Ginger
  * @version V1.0.0
  * @date    2015/11/14
  * @brief   对每一个pid结构体都要先进行函数的连接，再进行初始化
  ******************************************************************************
  * @attention 应该是用二阶差分(d)云台会更加稳定（24/7 Turtle注：这句话意思不明）
  *
  ******************************************************************************
  * 2024/7 
  *大疆PID库 增加了增量式PID的实现  
  *PID结构体增加了volatile修饰  避免一些问题
  *Modified by
  *UESTC-School Of Automation Engineering-Technology Association-Turtle
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "pid.h"
#include "stm32f407xx.h"



PID_TypeDef pid_speed;  //轮上速度闭环



/*参数初始化--------------------------------------------------------------*/
static void pid_param_init(
	volatile PID_TypeDef * pid, 
	PID_ID   id,
	uint16_t maxout,
	uint16_t intergral_limit,
	float deadband,
	uint16_t period,
	int16_t  max_err,
	int16_t  target,

	float 	kp, 
	float 	ki, 
	float 	kd)
{
	pid->id = id;		
	
	pid->ControlPeriod = period;             //没用到
	pid->DeadBand = deadband;
	pid->IntegralLimit = intergral_limit;
	pid->MaxOutput = maxout;
	pid->Max_Err = max_err;
	pid->target = target;
	
	pid->kp = kp;
	pid->ki = ki;
	pid->kd = kd;
	
	pid->output = 0;
}

/*中途更改参数设定--------------------------------------------------------------*/
static void pid_reset(volatile PID_TypeDef * pid, float kp, float ki, float kd)
{
	pid->kp = kp;
	pid->ki = ki;
	pid->kd = kd;
}

/*pid计算-----------------------------------------------------------------------*/
static float pid_calculate(volatile PID_TypeDef* pid, float measure)//, int16_t target)
{
//	uint32_t time,lasttime;
	
	pid->lasttime = pid->thistime;
	pid->thistime = HAL_GetTick();
     
	pid->dtime = pid->thistime-pid->lasttime;
	pid->measure = measure;
  //	pid->target = target;
	//更新偏差和输出
  pid->far_err = pid->last_err;	
	pid->last_err  = pid->err;
	pid->last_output = pid->output;
	//计算当前偏差
	pid->err = pid->target - pid->measure;
	     
	//是否进入死区
	if((ABS(pid->err) > pid->DeadBand))
	{    
		if(pid->id==PID_Position)  //位置式PID
		{                                  
			 pid->pout = pid->kp * pid->err;
			 pid->iout += (pid->ki * pid->err);
			 pid->dout =  pid->kd * (pid->err - pid->last_err); 
			 
			 //积分是否超出限制
			 if(pid->iout > pid->IntegralLimit)
						pid->iout = pid->IntegralLimit;
			 if(pid->iout < - pid->IntegralLimit)
						pid->iout = - pid->IntegralLimit;
			 //pid输出和
			 pid->output = pid->pout + pid->iout + pid->dout;
		}
		else if(pid->id==PID_Speed)  //增量式PID
		{                      
			 pid->pout = pid->kp * (pid->err - pid->last_err);
			 pid->iout = pid->ki * (pid->err);
			 pid->dout = pid->kd * (pid->err - 2*pid->last_err + pid->far_err); 
			 //积分是否超出限制
			 if(pid->iout > pid->IntegralLimit)
						pid->iout = pid->IntegralLimit;
			 if(pid->iout < - pid->IntegralLimit)
						pid->iout = - pid->IntegralLimit;
			 //pid输出和
			 pid->output += pid->pout + pid->iout + pid->dout;
		}
    
		#if WEIGHTED_FILTERING
		pid->output = pid->output*WEIGHT + pid->last_output*(1.0-WEIGHT);  //权重滤波
		#endif
		
		if(pid->output>pid->MaxOutput)         
		{
			pid->output = pid->MaxOutput;
		}
		if(pid->output < -(pid->MaxOutput))
		{
			pid->output = -(pid->MaxOutput);
		}
	}
  else 
	{
		pid->output=0;
		pid->pout=0;
		pid->iout=0;
		pid->dout=0;
  }
	return pid->output;
}

/*pid结构体初始化，每一个pid参数需要调用一次-----------------------------------------------------*/
void pid_init(volatile PID_TypeDef* pid)
{
	pid->f_param_init = pid_param_init;
	pid->f_pid_reset = pid_reset;
	pid->f_cal_pid = pid_calculate;
}



