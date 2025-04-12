#include "control.h"
#include "tb6612.h"
#include "filter.h"
#include "angle.h"

uint32_t BL_CMD=0;//蓝牙指令

PID_TypeDef pid_speed_L;
PID_TypeDef pid_speed_R;
PID_TypeDef pid_turn_gyro;
PID_TypeDef pid_turn_position;
PID_TypeDef pid_stand_angle;
PID_TypeDef pid_stand_angle_speed;
PID_TypeDef pid_target_speed;

motor_typedef motor_L;
motor_typedef motor_R;
imu_typedef the_imu;
VS_TypeDef the_vs;//视觉
PID_pack_typedef the_pid;
key_typedef the_key;
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
  static int16_t counter_a = 0, counter_b = 0;
	
	counter_b = -(__HAL_TIM_GET_COUNTER(&htim3));
	counter_a = __HAL_TIM_GET_COUNTER(&htim4);
	
//	//滑动均值滤波
//	movaverage_L.input = counter_a;
//	counter_a = AntiPulse_MovingAverage_Filter(&movaverage_L);
//	movaverage_R.input = counter_b;
//	counter_b = AntiPulse_MovingAverage_Filter(&movaverage_R);
	/***低通滤波***/
	
	//把counter分别存入实例中
	hcar->Motor_L->counter = counter_a;
	hcar->Motor_R->counter = counter_b;

	//计算真实速度
	hcar->Motor_L->real_speed =hcar->Motor_L->real_speed*0.14f + 0.86f*counter_a*COUNT_SPEED_K;
	hcar->Motor_R->real_speed =hcar->Motor_R->real_speed*0.14f + 0.86f* counter_b*COUNT_SPEED_K;
	
	//计数器清零
	__HAL_TIM_SET_COUNTER(&htim3, 0);
	__HAL_TIM_SET_COUNTER(&htim4, 0);
}

/*电机执行*/
void Set_Motor_Speed()
{
	the_car.Motor_L->duty = 0;//电机控制量清零
	the_car.Motor_R->duty = 0;//电机控制量清零

	/***姿态环控制输出***/
	the_car.Motor_L->duty += -(int)(the_car.the_pid->pid_stand_angle_speed->output);
	the_car.Motor_R->duty += -(int)(the_car.the_pid->pid_stand_angle_speed->output);
	/***转向环控制输出***/
	the_car.Motor_L->duty -= (int)(the_car.the_pid->pid_turn_gyro->output);
	the_car.Motor_R->duty += (int)(the_car.the_pid->pid_turn_gyro->output);	

	/***速度环控制输出***/
	the_car.Motor_L->duty += -(int)the_car.the_pid->pid_target_speed->output;
	the_car.Motor_R->duty += -(int)the_car.the_pid->pid_target_speed->output;	

	/***电机执行***/
	Motor_SetSpeed(the_car.Motor_L->duty, L);  
	Motor_SetSpeed(the_car.Motor_R->duty, R);  
}

/**
  * @name   Speed_CLoop_PID_Control(car_typedef* hcar)
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
	//positive_control = 1000*(int)(hcar->Motor_L->real_speed+hcar->Motor_R->real_speed)*0.5f;
	if (hcar->Motor_L->motor_status == M_On)
	{
		hcar->the_pid->pid_speed_L->f_cal_pid(&pid_speed_L, hcar->Motor_L->real_speed); //计算  
		//Motor_SetSpeed((int)(hcar->the_pid->pid_speed_L->output), L);  //执行
	}
	if (hcar->Motor_R->motor_status == M_On)
	{
		hcar->the_pid->pid_speed_R->f_cal_pid(&pid_speed_R, hcar->Motor_R->real_speed);
		//Motor_SetSpeed((int)(hcar->the_pid->pid_speed_R->output), R);
	}
}

/**
  * @name   Stand_Angle_CLoop_PID_Control(car_typedef* hcar)
	*
	* @brief  直立环角度环执行函数
  *           
  * @note   该函数需要运行在一个固定的时基下         
  *     
  * @param  car_typedef结构体指针
  * 
	* @retval void
  **/
void Stand_Angle_CLoop_PID_Control(car_typedef* hcar)
{
	
	hcar->the_pid->pid_stand_angle_speed->target = hcar->the_pid->pid_stand_angle->f_cal_pid(&pid_stand_angle, hcar->Imu->roll);
}	

/**
  * @name   Stand_Angle_Speed_CLoop_PID_Control(car_typedef* hcar)
	*
	* @brief  直立环角速度环执行函数
  *           
  * @note   该函数需要运行在一个固定的时基下         
  *     
  * @param  car_typedef结构体指针
  * 
	* @retval void
  **/
void Stand_Angle_Speed_CLoop_PID_Control(car_typedef* hcar)
{
	hcar->the_pid->pid_stand_angle_speed->f_cal_pid(&pid_stand_angle_speed, hcar->Imu->gyro_x*Gyro_Gain);
}

/*转向角速度控制*/
void Turn_Gyro_PID_Control()
{
	if(BL_CMD & 0x1000)
	{
		the_car.the_pid->pid_turn_gyro->target +=10.0f;
		BL_CMD &= ~(0x1000);
	}
	else if(BL_CMD & 0x0800)
	{
		the_car.the_pid->pid_turn_gyro->target -=10.0f;
		BL_CMD &= ~(0x0800);		
	}
	else if(BL_CMD==0)
	{
		the_car.the_pid->pid_turn_gyro->target =0.0f;
	}
	the_car.the_pid->pid_turn_gyro->f_cal_pid(&pid_turn_gyro, the_car.Imu->gyro_z*Gyro_Gain);
}
/*转向位置控制*/
void Turn_Position_PID_Control()
{
	the_car.the_pid->pid_turn_gyro->target = the_car.the_pid->pid_turn_position->f_cal_pid(&pid_turn_position,the_car.vs->pos_err);
}

/**
  * @name   Target_Speed_CLoop_PID_Control(car_typedef* hcar)
	*
	* @brief  目标速度外环执行函数
  *           
  * @note   该函数需要运行在一个固定的时基下         
  *     
  * @param  car_typedef结构体指针
  * 
	* @retval void
  **/
void Target_Speed_CLoop_PID_Control(car_typedef* hcar)
{
	float mean_speed  = (hcar->Motor_L->real_speed + hcar->Motor_R->real_speed)*0.5f;
	hcar->the_pid->pid_target_speed->target = hcar->run_speed;//设定运行速度
	if(BL_CMD & 0x4000)
	{
		hcar->the_pid->pid_target_speed->target +=0.1f;
		BL_CMD &= ~(0x4000);
	}
	else if(BL_CMD & 0x2000)
	{
		hcar->the_pid->pid_target_speed->target -=0.1f;
		BL_CMD &= ~(0x2000);		
	}
	else if(BL_CMD==0)
	{
		hcar->the_pid->pid_target_speed->target =0.0f;
	}

	hcar->the_pid->pid_target_speed->f_cal_pid(&pid_target_speed, mean_speed);
//	/***串级输出给角度环控制***/
//	hcar->the_pid->pid_stand_angle->target = hcar->the_pid->pid_target_speed->output;
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
	hcar->vs = &the_vs;
	hcar->Motor_L = &motor_L;
	hcar->Motor_R = &motor_R;
	hcar->the_pid = &the_pid;
	hcar->KEY = &the_key;
	//pid包初始化
	hcar->the_pid->pid_speed_L = &pid_speed_L; 
	hcar->the_pid->pid_speed_R = &pid_speed_R;
	hcar->the_pid->pid_stand_angle = &pid_stand_angle;
	hcar->the_pid->pid_stand_angle_speed = &pid_stand_angle_speed;
	hcar->the_pid->pid_target_speed = &pid_target_speed;
	hcar->the_pid->pid_turn_position = &pid_turn_position;
	hcar->the_pid->pid_turn_gyro = &pid_turn_gyro;
//	pid_init(the_car.the_pid->pid_speed_L);
//	the_car.the_pid->pid_speed_L->f_param_init(the_car.the_pid->pid_speed_L,PID_Speed,MAX_MOTOR_DUTY,MAX_MOTOR_DUTY,0,1,0.0,10000.0f,100.0f,0.0f);
//	the_car.the_pid->pid_speed_L->enable = 1;
//	pid_init(the_car.the_pid->pid_speed_R);
//	the_car.the_pid->pid_speed_R->f_param_init(the_car.the_pid->pid_speed_R,PID_Speed,MAX_MOTOR_DUTY,MAX_MOTOR_DUTY,0,1,0.0,10000.0f,100.0f,0.0f);  
//	the_car.the_pid->pid_speed_R->enable = 1;

	

	/***直立环***/
	pid_init(the_car.the_pid->pid_stand_angle);
	the_car.the_pid->pid_stand_angle->f_param_init(the_car.the_pid->pid_stand_angle,PID_Position,MAX_CONTROL_ANGLE,0,0,0,0.0f, 2.5f,0.2f,0.0f);  
	pid_init(the_car.the_pid->pid_stand_angle_speed);
	the_car.the_pid->pid_stand_angle_speed->f_param_init(the_car.the_pid->pid_stand_angle_speed,PID_Position,MAX_MOTOR_DUTY,0,0,0,0.0,180.0f,0.0f,100.0f);
	
	/***速度环***/
	pid_init(the_car.the_pid->pid_target_speed);
	the_car.the_pid->pid_target_speed->f_param_init(the_car.the_pid->pid_target_speed,PID_Position,MAX_MOTOR_DUTY,MAX_MOTOR_DUTY*0.5f,0,0,0.0f,2300.0f,30.0f,0.0f);

	/***转向环***/
	pid_init(the_car.the_pid->pid_turn_gyro);	
	the_car.the_pid->pid_turn_gyro->f_param_init(the_car.the_pid->pid_turn_gyro,PID_Position,MAX_MOTOR_DUTY,0,0,0,0.0f,0.0f,0.0f,0.0f);
	pid_init(the_car.the_pid->pid_turn_position);	
	the_car.the_pid->pid_turn_position->f_param_init(the_car.the_pid->pid_turn_position,PID_Position,MAX_TURN_GYRO,0,0,0,0.0f,0.0f,0.0f,0.0f);

}
