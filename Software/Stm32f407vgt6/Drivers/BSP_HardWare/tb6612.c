#include "tb6612.h"
#include "tim.h"
#include "stdio.h"
/************************************************************************************************************
** int Motor_SetSpeed(uint8_t Mode,float speed, uint8_t L_R)                  				                     **                                                              
** ��������������һ�����ٶ�                                                                    		         **
** ����˵����Foward or Backward�� �ٶȣ� ���ҵ��                          					                         **   
** �������أ�int                                                                                            **
************************************************************************************************************/

//speed �ķ�Χ��0-100

int Motor_SetSpeed(int speed, uint8_t L_R)
{ 
	if (L_R == 1)  //�ұ�ת
	{
		if (speed >= 0)  //��ת
		{
			HAL_GPIO_WritePin(AIN1_GPIO_Port,AIN1_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(AIN2_GPIO_Port,AIN2_Pin,GPIO_PIN_RESET);
			__HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_1,speed);
		}
		else
		{
			HAL_GPIO_WritePin(AIN1_GPIO_Port,AIN1_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(AIN2_GPIO_Port,AIN2_Pin,GPIO_PIN_SET);
			__HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_1,-speed);
		}
		
	}
	
	if (L_R == 0)  //���ת
	{
		if (speed >= 0)
		{
			HAL_GPIO_WritePin(BIN1_GPIO_Port,BIN1_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(BIN2_GPIO_Port,BIN2_Pin,GPIO_PIN_SET);
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,speed);
		}
		else
		{
			HAL_GPIO_WritePin(BIN1_GPIO_Port,BIN1_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(BIN2_GPIO_Port,BIN2_Pin,GPIO_PIN_RESET);	
		}
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,-speed);
	}
	
	return 0;
}

/************************************************************************************************************
** int Motor_Start(uint8_t L_R)                  				                                                   **                                                              
** ��������������һ����                                                                       		         **
** ����˵�������ҵ��                          					                                                   **   
** �������أ�uint8_t                                                                                        **
************************************************************************************************************/

int Motor_Start(uint8_t L_R)
{
	if (L_R == 0)  //���
	{
		HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
		return 0;
	}
	if (L_R == 1)  //�ұ�
	{
		HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_1);
		return 0;
	}
	if (L_R == 2)  //ͬʱ
	{
		HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_1);
		return 0;
	}
	else
	{
		return 1;
	}
}

/************************************************************************************************************
** int Motor_Off(uint8_t L_R)                  				                                                     **                                                              
** ����������ֹͣһ����                                                                        		         **
** ����˵�������ҵ��                          					                                                   **   
** �������أ�uint8_t                                                                                        **
************************************************************************************************************/

int Motor_Off(uint8_t L_R)
{
	if (L_R == 0)  //���
	{
		HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);
		return 0;
	}
	if (L_R == 1)  //�ұ�
	{
		HAL_TIM_PWM_Stop(&htim5,TIM_CHANNEL_1);
		return 0;
	}
	if (L_R == 2)  //ͬʱ
	{
		HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&htim5,TIM_CHANNEL_1);
		return 0;
	}
	else
	{
		return 1;
	}
}

