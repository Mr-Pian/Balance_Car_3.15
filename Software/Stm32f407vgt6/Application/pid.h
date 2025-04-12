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
	uint8_t enable;						//ʹ�ܿ���
	float target;							//Ŀ��ֵ
	float lastNoneZeroTarget;
	float kp;
	float ki;
	float kd;
	
	float   measure;					//����ֵ
	float   err;							//���
	float   last_err;      		//�ϴ����
  float   far_err;          //���ϴ����
	
	float pout;
	float iout;
	float dout;
	
	float output;						  //�������
	float last_output;			  //�ϴ����
	
	float MaxOutput;				  //����޷�
	float IntegralLimit;		  //�����޷�
	float DeadBand;			      //����������ֵ��
	float ControlPeriod;		  //��������
	
	uint32_t thistime;
	uint32_t lasttime;
	uint32_t dtime;	
	
	void (*f_param_init)(volatile struct _PID_TypeDef *pid,  //PID������ʼ��
				PID_ID id,
				float maxOutput,
				float integralLimit,
				float deadband,
				float controlPeriod,    
				float  target,
				float kp,
				float ki,
				float kd);
				   
	void (*f_pid_reset)(volatile struct _PID_TypeDef *pid, float kp,float ki, float kd);		//pid���������޸�
	float (*f_cal_pid)(volatile struct _PID_TypeDef *pid, float measure);   //pid����
}PID_TypeDef;

void pid_init(volatile PID_TypeDef* pid);
void pid_enable(volatile PID_TypeDef * pid,uint8_t enable);
#endif
