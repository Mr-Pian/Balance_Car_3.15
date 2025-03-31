/**
  ******************************************************************************
  * @file    pid.c
  * @author  Ginger
  * @version V1.0.0
  * @date    2015/11/14
  * @brief   ��ÿһ��pid�ṹ�嶼Ҫ�Ƚ��к��������ӣ��ٽ��г�ʼ��
  ******************************************************************************
  * @attention Ӧ�����ö��ײ��(d)��̨������ȶ���24/7 Turtleע����仰��˼������
  *
  ******************************************************************************
  * 2024/7 
  *��PID�� ����������ʽPID��ʵ��  
  *PID�ṹ��������volatile����  ����һЩ����
  *Modified by
  *UESTC-School Of Automation Engineering-Technology Association-Turtle
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "pid.h"
#include "stm32f407xx.h"



PID_TypeDef pid_speed;  //�����ٶȱջ�



/*������ʼ��--------------------------------------------------------------*/
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
	
	pid->ControlPeriod = period;             //û�õ�
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

/*��;���Ĳ����趨--------------------------------------------------------------*/
static void pid_reset(volatile PID_TypeDef * pid, float kp, float ki, float kd)
{
	pid->kp = kp;
	pid->ki = ki;
	pid->kd = kd;
}

/*pid����-----------------------------------------------------------------------*/
static float pid_calculate(volatile PID_TypeDef* pid, float measure)//, int16_t target)
{
//	uint32_t time,lasttime;
	
	pid->lasttime = pid->thistime;
	pid->thistime = HAL_GetTick();
     
	pid->dtime = pid->thistime-pid->lasttime;
	pid->measure = measure;
  //	pid->target = target;
	//����ƫ������
  pid->far_err = pid->last_err;	
	pid->last_err  = pid->err;
	pid->last_output = pid->output;
	//���㵱ǰƫ��
	pid->err = pid->target - pid->measure;
	     
	//�Ƿ��������
	if((ABS(pid->err) > pid->DeadBand))
	{    
		if(pid->id==PID_Position)  //λ��ʽPID
		{                                  
			 pid->pout = pid->kp * pid->err;
			 pid->iout += (pid->ki * pid->err);
			 pid->dout =  pid->kd * (pid->err - pid->last_err); 
			 
			 //�����Ƿ񳬳�����
			 if(pid->iout > pid->IntegralLimit)
						pid->iout = pid->IntegralLimit;
			 if(pid->iout < - pid->IntegralLimit)
						pid->iout = - pid->IntegralLimit;
			 //pid�����
			 pid->output = pid->pout + pid->iout + pid->dout;
		}
		else if(pid->id==PID_Speed)  //����ʽPID
		{                      
			 pid->pout = pid->kp * (pid->err - pid->last_err);
			 pid->iout = pid->ki * (pid->err);
			 pid->dout = pid->kd * (pid->err - 2*pid->last_err + pid->far_err); 
			 //�����Ƿ񳬳�����
			 if(pid->iout > pid->IntegralLimit)
						pid->iout = pid->IntegralLimit;
			 if(pid->iout < - pid->IntegralLimit)
						pid->iout = - pid->IntegralLimit;
			 //pid�����
			 pid->output += pid->pout + pid->iout + pid->dout;
		}
    
		#if WEIGHTED_FILTERING
		pid->output = pid->output*WEIGHT + pid->last_output*(1.0-WEIGHT);  //Ȩ���˲�
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

/*pid�ṹ���ʼ����ÿһ��pid������Ҫ����һ��-----------------------------------------------------*/
void pid_init(volatile PID_TypeDef* pid)
{
	pid->f_param_init = pid_param_init;
	pid->f_pid_reset = pid_reset;
	pid->f_cal_pid = pid_calculate;
}



