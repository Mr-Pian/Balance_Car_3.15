#include "control.h"
#include "tim.h"

car_typedef the_car=
{
	0.0,
	0.0,
	M_Off,
	M_Off
};

/**
  * @name   Get_Motor_Speed(car_typedef* hcar)
	*
	* @brief  ���ص������ϵ���ʵ���ٶ�
  *           
  * @note   �ú�����Ҫ������һ���̶���ʱ����         
  *     
  * @param  car_typedef�ṹ��ָ��
  * 
	* @retval void
  **/

void Get_Motor_Speed(car_typedef* hcar)
{
	uint16_t counter_a = 0, counter_b = 0;
	float angle_a = 0.0, angle_b = 0.0;
	
	counter_a = __HAL_TIM_GET_COUNTER(&htim3);
	counter_b = __HAL_TIM_GET_COUNTER(&htim4);
	
	//��ֹ����
	if (counter_a > 32768)
	{
		counter_a = 65536 - counter_a;
	}
	if (counter_b > 32768)
	{
		counter_b = 65536 - counter_b;
	}

	angle_a = 2.0f*PI*((float)counter_a/30000.0f);
	angle_b = 2.0f*PI*((float)counter_b/30000.0f);
	
	//�жϷ������Ϸ���
	if (__HAL_TIM_GET_COUNTER(&htim3) > 32768)
	{
		hcar->L_Real_Speed = (angle_a*(WHEEL_SIZE/2.0f))*GET_MOTOR_SPEED_FRE;
	}
	else
	{
		hcar->L_Real_Speed = -(angle_a*(WHEEL_SIZE/2.0f))*GET_MOTOR_SPEED_FRE;
	}
	
	if (__HAL_TIM_GET_COUNTER(&htim4) > 32768)
	{
		hcar->R_Real_Speed = -(angle_b*(WHEEL_SIZE/2.0f))*GET_MOTOR_SPEED_FRE;
	}
	else
	{
		hcar->R_Real_Speed = (angle_b*(WHEEL_SIZE/2.0f))*GET_MOTOR_SPEED_FRE;
	}
	
	//����������
	__HAL_TIM_SET_COUNTER(&htim3, 0);
	__HAL_TIM_SET_COUNTER(&htim4, 0);
}

/**
  * @name   Get_Motor_Speed(car_typedef* hcar)
	*
	* @brief  �����ٶȱջ�ִ�к���
  *           
  * @note   �ú�����Ҫ������һ���̶���ʱ����         
  *     
  * @param  car_typedef�ṹ��ָ��
  * 
	* @retval void
  **/

void Speed_CLoop_PID_Control(car_typedef* hcar)
{
	if (hcar->L_Motor_Status == M_Foward)
	{
		;
	}
	
}