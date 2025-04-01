/**
  ******************************************************************************
  * @file    pid.c
  * @author  aBo
  * @version V1.0.0
  * @date    2025/2/24
  * @brief   ��ÿһ��pid�ṹ�嶼Ҫ�Ƚ��к��������ӣ��ٽ��г�ʼ��
  ******************************************************************************
  * @attention 
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "pid.h"
#include "main.h"
/*������ʼ��--------------------------------------------------------------*/
static void pid_param_init(
	volatile PID_TypeDef * pid, //pid�ṹ��
	PID_ID   id,								//PID����
	uint16_t maxout,						//PID����޷�
	uint16_t intergral_limit,		//�����޷�
	float deadband,							//����
	float period,						//PID��������
	float  target,						//PIDĿ��

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
/*PID����or�ر�*/
void pid_enable(volatile PID_TypeDef * pid,uint8_t enable)
{
	pid->enable =enable;
	if(enable)
	{
		pid->enable =1;

		//pid->iout = now_output//ʹi�����Ϊ��ǰ���������ƽ��
		//pid->err = pid->last_err;//ʹƫ��仯��Ϊ0������d��Ӱ��

	}
	else
		pid->enable=0;
}


/*��;���Ĳ����趨--------------------------------------------------------------*/
static void pid_reset(volatile PID_TypeDef * pid, float kp, float ki, float kd)
{
	pid->kp = kp;
	pid->ki = ki;
	pid->kd = kd;
}

/*pid����-----------------------------------------------------------------------*/	
static float pid_calculate(volatile PID_TypeDef* pid, float measure)
{
	if(pid->enable)
	{
		//	uint32_t time,lasttime;
			
		//	pid->lasttime = pid->thistime;
		//	pid->thistime = HAL_GetTick();
				 
		//	pid->dtime = pid->thistime-pid->lasttime;
			//���²���ֵ
			pid->measure = measure;
			//����ƫ������
			pid->far_err = pid->last_err;	
			pid->last_err  = pid->err;
			pid->last_output = pid->output;
			//���㵱ǰƫ��
			pid->err = pid->target - pid->measure;
			
				 
			//�Ƿ��������
			if((__fabs(pid->err) > pid->DeadBand))
			{    
				if(pid->id==PID_Position) //λ��ʽPID
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
					 pid->output = pid->pout + pid->iout + pid->dout;//�������
					 //pid->output = pid->output*0.7f + pid->last_output*0.3f;  //Ȩ���˲�
			
//					/***����޷�"�����㿹���ͷ�"***/
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
				else if(pid->id==PID_Speed)//����ʽPID
				{ 
					pid->pout = pid->kp * (pid->err - pid->last_err);
					pid->iout = pid->ki * (pid->err);
					pid->dout = pid->kd * (pid->err - 2*pid->last_err + pid->far_err); 
					//pid�����
					pid->output += pid->pout + pid->iout + pid->dout;//�������	

					//����޷�(���޷�)
					if(pid->output>pid->MaxOutput)         
					{
						pid->output = pid->MaxOutput;
					}
					if(pid->output < -(pid->MaxOutput))
					{
						pid->output = -(pid->MaxOutput);
					}
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

/*pid�ṹ���ʼ����ÿһ��pid������Ҫ����һ��-----------------------------------------------------*/
void pid_init(volatile PID_TypeDef* pid)
{
	pid->f_param_init = pid_param_init;
	pid->f_pid_reset = pid_reset;
	pid->f_cal_pid = pid_calculate;
}



