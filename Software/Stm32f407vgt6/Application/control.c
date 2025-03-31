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
	* @brief  返回单边轮上的真实线速度
  *           
  * @note   该函数需要运行在一个固定的时基下         
  *     
  * @param  car_typedef结构体指针
  * 
	* @retval void
  **/

void Get_Motor_Speed(car_typedef* hcar)
{
	uint16_t counter_a = 0, counter_b = 0;
	float angle_a = 0.0, angle_b = 0.0;
	
	counter_a = __HAL_TIM_GET_COUNTER(&htim3);
	counter_b = __HAL_TIM_GET_COUNTER(&htim4);
	
	//防止跳变
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
	
	//判断方向后加上符号
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
	
	//计数器清零
	__HAL_TIM_SET_COUNTER(&htim3, 0);
	__HAL_TIM_SET_COUNTER(&htim4, 0);
}

/**
  * @name   Get_Motor_Speed(car_typedef* hcar)
	*
	* @brief  轮上速度闭环执行函数
  *           
  * @note   该函数需要运行在一个固定的时基下         
  *     
  * @param  car_typedef结构体指针
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