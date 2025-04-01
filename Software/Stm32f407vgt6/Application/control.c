#include "control.h"
#include "tb6612.h"

PID_TypeDef pid_speed_L;
PID_TypeDef pid_speed_R;
sensor_typedef sensor_val;
car_typedef the_car;

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

	angle_a = 2.0*PI*(((float)counter_a)/30000.0);
	angle_b = 2.0*PI*(((float)counter_b)/30000.0);
	
	//�жϷ������Ϸ���
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
	if (hcar->L_Motor_Status == M_On)
	{
		hcar->Pid_speed_L->f_cal_pid(&pid_speed_L, hcar->Sensor_val->L_Speed); //����  
		Motor_SetSpeed((int)hcar->Pid_speed_L->output, L);  //ִ��
	}
	if (hcar->R_Motor_Status == M_On)
	{
		hcar->Pid_speed_R->f_cal_pid(&pid_speed_R, hcar->Sensor_val->R_Speed); //����  
		Motor_SetSpeed((int)hcar->Pid_speed_R->output, R);  //ִ��
	}
}

/**
  * @name   Control_Init(car_typedef* car)
	*
	* @brief  ���Ƴ�ʼ��
  *           
  * @note   ���������� �κο��Ʋ�����ʼ֮ǰ         
  *     
  * @param  car_typedef�ṹ��ָ��
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
	car->Pid_speed_L->enable=1;  //pidʹ��
	car->Pid_speed_R->enable=1;
}