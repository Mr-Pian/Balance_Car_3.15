#include "control.h"
#include "tb6612.h"

PID_TypeDef pid_speed_L;
PID_TypeDef pid_speed_R;
sensor_typedef sensor_val;
car_typedef the_car;

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

	angle_a = 2.0*PI*(((float)counter_a)/30000.0);
	angle_b = 2.0*PI*(((float)counter_b)/30000.0);
	
	//判断方向后加上符号
	if (__HAL_TIM_GET_COUNTER(&htim3) > 32768)
	{
		hcar->Sensor_val->R_Speed = (angle_a*(WHEEL_SIZE/2.0))*GET_MOTOR_SPEED_FRE;
	}
	else
	{
		hcar->Sensor_val->R_Speed = -(angle_a*(WHEEL_SIZE/2.0))*GET_MOTOR_SPEED_FRE;
	}
	
	if (__HAL_TIM_GET_COUNTER(&htim4) > 32768)
	{
		hcar->Sensor_val->L_Speed = -(angle_b*(WHEEL_SIZE/2.0))*GET_MOTOR_SPEED_FRE;
	}
	else
	{
		hcar->Sensor_val->L_Speed = (angle_b*(WHEEL_SIZE/2.0))*GET_MOTOR_SPEED_FRE;
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
	if (hcar->L_Motor_Status == M_On)
	{
		hcar->Pid_speed_L->f_cal_pid(&pid_speed_L, hcar->Sensor_val->L_Speed); //计算  
		Motor_SetSpeed((int)hcar->Pid_speed_L->output, L);  //执行
	}
	if (hcar->R_Motor_Status == M_On)
	{
		hcar->Pid_speed_R->f_cal_pid(&pid_speed_R, hcar->Sensor_val->R_Speed); //计算  
		Motor_SetSpeed((int)hcar->Pid_speed_R->output, R);  //执行
	}
}

/**
  * @name   Control_Init(car_typedef* car)
	*
	* @brief  控制初始化
  *           
  * @note   必须运行在 任何控制操作开始之前         
  *     
  * @param  car_typedef结构体指针
  * 
	* @retval void
  **/

void Control_Init(car_typedef* car)
{
	car->Sensor_val = &sensor_val;
	car->Pid_speed_L = &pid_speed_L;
	car->Pid_speed_R = &pid_speed_R;
	car->L_Motor_Status = M_Off;
	car->R_Motor_Status = M_Off;
	car->Pid_speed_L->enable=1;  //pid使能
	car->Pid_speed_R->enable=1;
}