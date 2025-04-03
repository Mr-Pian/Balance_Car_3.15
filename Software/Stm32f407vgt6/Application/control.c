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
  int16_t counter_a = 0, counter_b = 0;
	
	counter_b = __HAL_TIM_GET_COUNTER(&htim3);
	counter_b = -counter_b;
	counter_a = __HAL_TIM_GET_COUNTER(&htim4);
	
	//������ֵ�˲�
	movaverage_L.input = counter_a;
	counter_a = AntiPulse_MovingAverage_Filter(&movaverage_L);
	movaverage_R.input = counter_b;
	counter_b = AntiPulse_MovingAverage_Filter(&movaverage_R);
	
	//��counter�ֱ����ʵ����
	hcar->Motor_L->counter = counter_a;
	hcar->Motor_R->counter = counter_b;

	//������ʵ�ٶ�
	hcar->Motor_L->real_speed = counter_a*COUNT_SPEED_K;
	hcar->Motor_R->real_speed = counter_b*COUNT_SPEED_K;
	
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
	if (hcar->Motor_L->motor_status == M_On)
	{
		hcar->the_pid->pid_speed_L->f_cal_pid(&pid_speed_L, hcar->Motor_L->real_speed); //����  
		Motor_SetSpeed((int)hcar->the_pid->pid_speed_L->output, L);  //ִ��
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
	* @brief  ���Ƴ�ʼ��
  *           
  * @note   ���������� �κο��Ʋ�����ʼ֮ǰ         
  *     
  * @param  car_typedef�ṹ��ָ��
  * 
	* @retval void
  **/

void Control_Init(car_typedef* hcar)
{
	//���ṹ���ʼ��
	hcar->Imu = &the_imu;
	hcar->Motor_L = &motor_L;
	hcar->Motor_R = &motor_R;
	hcar->the_pid = &the_pid;
	
	//pid����ʼ��
	hcar->the_pid->pid_speed_L = &pid_speed_L; 
	hcar->the_pid ->pid_speed_R = &pid_speed_R; 
}