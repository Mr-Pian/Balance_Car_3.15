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
  *��PID�� ����������ʽPID��ʵ��  
  *Modified by
  *UESTC-School Of Automation Engineering-Technology Association-Turtle
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#ifndef _PID_H
#define _PID_H

#include "stdint.h"

#define ABS(x) ((x>0)? x: -x) //��������ֵ

#define WEIGHTED_FILTERING 0  //�Ƿ��Ȩ���˲�
#define WEIGHT 0.7f

typedef enum
{

	PID_Position,
	PID_Speed,
	
}PID_ID;

typedef struct _PID_TypeDef
{
	PID_ID id;
	
	float target;						//Ŀ��ֵ
	float lastNoneZeroTarget;
	float kp;
	float ki;
	float kd;
	
	float   measure;				//����ֵ
	float   err;						//���
	float   last_err;      	//�ϴ����
  float   far_err;        //���ϴ����
	
	float pout;
	float iout;
	float dout;
	
	float output;						//�������
	float last_output;			//�ϴ����
	
	float MaxOutput;				//����޷�
	float IntegralLimit;		//�����޷�
	float DeadBand;			    //����������ֵ��
	float ControlPeriod;		//��������
	float  Max_Err;					//������
	
	uint32_t thistime;
	uint32_t lasttime;
	uint8_t dtime;	
	
	void (*f_param_init)(volatile struct _PID_TypeDef *pid,  //PID������ʼ��(pidģʽ���޷���������ֵ��)
				
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
				   
	void (*f_pid_reset)(volatile struct _PID_TypeDef *pid, float kp,float ki, float kd);		//�����޸�P I D��������
	
	float (*f_cal_pid)(volatile struct _PID_TypeDef *pid, float measure);   //pid����

}PID_TypeDef;

extern PID_TypeDef pid_speed;

void pid_init(volatile PID_TypeDef* pid);
#endif
