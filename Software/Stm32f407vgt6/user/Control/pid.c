/**
  ******************************************************************************
  * @file    pid.c
  * @author  aBo
  * @version V1.0.0
  * @date    2025/2/24
  * @brief   对每一个pid结构体都要先进行函数的连接，再进行初始化
  ******************************************************************************
  * @attention 
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "pid.h"
#include "main.h"
/*参数初始化--------------------------------------------------------------*/
static void pid_param_init(
	volatile PID_TypeDef * pid, //pid结构体
	PID_ID   id,								//PID类型
	uint16_t maxout,						//PID输出限幅
	uint16_t intergral_limit,		//积分限幅
	float deadband,							//死区
	uint16_t period,						//PID控制周期
	int16_t  target,						//PID目标

	float 	kp, 							
	float 	ki, 
	float 	kd)
{
	pid->id = id;		
	
	pid->ControlPeriod = period;            
	pid->DeadBand = deadband;
	pid->IntegralLimit = intergral_limit;
	pid->MaxOutput = maxout;
	pid->target = target;
	
	pid->kp = kp;
	pid->ki = ki;
	pid->kd = kd;
	
	pid->output = 0;
}
/*PID开启or关闭*/
void pid_enable(volatile PID_TypeDef * pid,uint8_t enable)
{
	pid->enable =enable;
	if(enable)
	{
		pid->enable =1;

		//pid->iout = now_output//使i项输出为当前输出，更加平滑
		//pid->err = pid->last_err;//使偏差变化率为0，降低d项影响

	}
	else
	{
		pid->enable=0;
		pid->pout = 0;
		pid->iout = 0;
		pid->dout = 0;
		pid->output = 0;
		pid->err = 0;
		pid->last_err = 0;
		pid->far_err = 0;
	}
}


/*中途更改参数设定--------------------------------------------------------------*/
static void pid_reset(volatile PID_TypeDef * pid, float kp, float ki, float kd)
{
	pid->kp = kp;
	pid->ki = ki;
	pid->kd = kd;
	
}

/*pid计算-----------------------------------------------------------------------*/	
static float pid_calculate(volatile PID_TypeDef* pid, float measure)
{
	if(pid->enable)
	{
		//	uint32_t time,lasttime;
			
		//	pid->lasttime = pid->thistime;
		//	pid->thistime = HAL_GetTick();
				 
		//	pid->dtime = pid->thistime-pid->lasttime;
			//更新测量值
			pid->measure = measure;
			//更新偏差和输出
			pid->far_err = pid->last_err;	
			pid->last_err  = pid->err;
			pid->last_output = pid->output;
			//计算当前偏差
			pid->err = pid->target - pid->measure;
			
				 
			//是否进入死区
			if((__fabs(pid->err) > pid->DeadBand))
			{    
				if(pid->id==PID_Position) //位置式PID
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
					 pid->output = pid->pout + pid->iout + pid->dout;//绝对输出
					 //pid->output = pid->output*0.7f + pid->last_output*0.3f;  //权重滤波
			
//					/***输出限幅"反计算抗饱和法"***/
//					if(pid->output>pid->MaxOutput)         
//					{
//						pid->iout -= pid->output - pid->MaxOutput;
//						pid->output = pid->MaxOutput;
//					}
//					if(pid->output < -(pid->MaxOutput))
//					{
//						pid->iout += pid->output - pid->MaxOutput;
//						pid->output = -(pid->MaxOutput);
//					}
				}
				else if(pid->id==PID_Speed)//增量式PID
				{ 
					 pid->pout = pid->kp * (pid->err - pid->last_err);
					 pid->iout = pid->ki * (pid->err);
					 pid->dout = pid->kd * (pid->err - 2*pid->last_err + pid->far_err); 
					 //pid输出和
					 pid->output += pid->pout + pid->iout + pid->dout;//增量输出	
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
	else
		return 0;
}

/*pid结构体初始化，每一个pid参数需要调用一次-----------------------------------------------------*/
void pid_init(volatile PID_TypeDef* pid)
{
	pid->f_param_init = pid_param_init;
	pid->f_pid_reset = pid_reset;
	pid->f_cal_pid = pid_calculate;
}



