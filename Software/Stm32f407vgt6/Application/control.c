#include "control.h"
#include "tb6612.h"
#include "filter.h"

PID_TypeDef pid_speed_L;
PID_TypeDef pid_speed_R;
motor_typedef motor_L;
motor_typedef motor_R;
imu_typedef the_imu;
PID_pack_typedef the_pid;
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
  int16_t counter_a = 0, counter_b = 0;
	
	counter_b = __HAL_TIM_GET_COUNTER(&htim3);
	counter_b = -counter_b;
	counter_a = __HAL_TIM_GET_COUNTER(&htim4);
	
	//滑动均值滤波
	movaverage_L.input = counter_a;
	counter_a = AntiPulse_MovingAverage_Filter(&movaverage_L);
	movaverage_R.input = counter_b;
	counter_b = AntiPulse_MovingAverage_Filter(&movaverage_R);
	
	//把counter分别存入实例中
	hcar->Motor_L->counter = counter_a;
	hcar->Motor_R->counter = counter_b;

	//计算真实速度
	hcar->Motor_L->real_speed = counter_a*COUNT_SPEED_K;
	hcar->Motor_R->real_speed = counter_b*COUNT_SPEED_K;
	
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
	if (hcar->Motor_L->motor_status == M_On)
	{
		hcar->the_pid->pid_speed_L->f_cal_pid(&pid_speed_L, hcar->Motor_L->real_speed); //计算  
		Motor_SetSpeed((int)hcar->the_pid->pid_speed_L->output, L);  //执行
	}
	if (hcar->Motor_R->motor_status == M_On)
	{
		hcar->the_pid->pid_speed_R->f_cal_pid(&pid_speed_R, hcar->Motor_R->real_speed);
		Motor_SetSpeed((int)hcar->the_pid->pid_speed_R->output, R);
	}
}

/**
  * @name   Control_Init(car_typedef* hcar)
	*
	* @brief  控制初始化
  *           
  * @note   必须运行在 任何控制操作开始之前         
  *     
  * @param  car_typedef结构体指针
  * 
	* @retval void
  **/

void Control_Init(car_typedef* hcar)
{
	//主结构体初始化
	hcar->Imu = &the_imu;
	hcar->Motor_L = &motor_L;
	hcar->Motor_R = &motor_R;
	hcar->the_pid = &the_pid;
	
	//pid包初始化
	hcar->the_pid->pid_speed_L = &pid_speed_L; 
	hcar->the_pid ->pid_speed_R = &pid_speed_R; 
}