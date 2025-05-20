#include "control.h"
#include "filter.h"
#include "angle.h"
#include "math.h"
#include "adc.h"

car_typedef the_car;

/*获取电机速度*/
void Get_Motor_Speed(void)
{
 static int16_t counter_a = 0, counter_b = 0;
	/***编码器读取***/
	// counter_b = -(__HAL_TIM_GET_COUNTER(&htim3));
	// counter_a = __HAL_TIM_GET_COUNTER(&htim4);

  counter_a = -moto_chassis[0].speed_rpm;
	counter_b = +moto_chassis[1].speed_rpm;
   
//	//滑动均值滤波
//	movaverage_L.input = counter_a;
//	counter_a = AntiPulse_MovingAverage_Filter(&movaverage_L);
//	movaverage_R.input = counter_b;
//	counter_b = AntiPulse_MovingAverage_Filter(&movaverage_R);

	//把counter分别存入实例中
	the_car.MOTOR_L.counter = counter_b;
	the_car.MOTOR_R.counter = counter_a;

	//计算真实速度

    // /***低通滤波***/
	// the_car.MOTOR_L.real_speed =the_car.MOTOR_L.real_speed*0.14f + 0.86f*counter_a*COUNT_SPEED_K;
	// the_car.MOTOR_R.real_speed =the_car.MOTOR_R.real_speed*0.14f + 0.86f* counter_b*COUNT_SPEED_K;
    the_car.MOTOR_L.real_speed = the_car.MOTOR_L.counter*COUNT_SPEED_K;
    the_car.MOTOR_R.real_speed = the_car.MOTOR_R.counter*COUNT_SPEED_K;
    the_car.run_speed = (the_car.MOTOR_L.real_speed + the_car.MOTOR_R.real_speed)*0.5f; //计算车速
		the_car.run_distance += the_car.run_speed*0.001f;//计算行驶距离
	// //计数器清零
	// __HAL_TIM_SET_COUNTER(&htim3, 0);
	// __HAL_TIM_SET_COUNTER(&htim4, 0);
}

/*电机执行*/
void Set_Motor_Speed(void)
{
	the_car.MOTOR_L.duty = 0;//电机控制量清零
	the_car.MOTOR_R.duty = 0;//电机控制量清零
    
	// /***姿态环控制直接输出***/
	// the_car.MOTOR_L.duty += -(int)(the_car.PID.pid_car_gyro.output);
	// the_car.MOTOR_R.duty += -(int)(the_car.PID.pid_car_gyro.output);
	// /***转向环控制直接输出***/
	// the_car.MOTOR_L.duty -= (int)(the_car.PID.pid_turn_gyro.output);
	// the_car.MOTOR_R.duty += (int)(the_car.PID.pid_turn_gyro.output);	

	// /***速度环控制直接输出***/
	// the_car.MOTOR_L.duty += (int)the_car.PID.pid_target_speed.output;
	// the_car.MOTOR_R.duty += (int)the_car.PID.pid_target_speed.output;	


    /***电机速度闭环输出***/
    the_car.MOTOR_L.duty = the_car.PID.pid_Lspeed.output;
    the_car.MOTOR_R.duty = -the_car.PID.pid_Rspeed.output;
	/***电机执行***/
    set_moto_current(&hfdcan1,the_car.MOTOR_R.duty,the_car.MOTOR_L.duty,0,0);
}

/*电机速度闭环控制*/
void MotorSpeed_PID_Control(void)
{
     /***直立环控制输出***/
    the_car.PID.pid_Lspeed.target = the_car.PID.pid_car_angle.output; 
    the_car.PID.pid_Rspeed.target = the_car.PID.pid_car_angle.output; 
    the_car.PID.pid_Lspeed.target -= the_car.PID.pid_ptm_angle.output; 
    the_car.PID.pid_Rspeed.target -= the_car.PID.pid_ptm_angle.output; 	
    /***转向环控制输出***/
    the_car.PID.pid_Lspeed.target += the_car.PID.pid_turn_gyro.output;
   	the_car.PID.pid_Rspeed.target -= the_car.PID.pid_turn_gyro.output;
    /***位移环控制输出***/
    the_car.PID.pid_Lspeed.target -= the_car.PID.pid_target_distance.output;
    the_car.PID.pid_Rspeed.target -= the_car.PID.pid_target_distance.output;

	the_car.PID.pid_Lspeed.f_cal_pid(&the_car.PID.pid_Lspeed, the_car.MOTOR_L.real_speed);  
	the_car.PID.pid_Rspeed.f_cal_pid(&the_car.PID.pid_Rspeed, the_car.MOTOR_R.real_speed);

}

/*车身角度控制*/
void CarAngle_PID_Control(void)
{
	the_car.PID.pid_car_gyro.target = the_car.PID.pid_car_angle.f_cal_pid(&the_car.PID.pid_car_angle, the_car.IMU.roll - the_car.car_angle_offset);
}	

/*车身角速度控制*/
void CarGyro_PID_Control(void)
{
	the_car.PID.pid_car_gyro.f_cal_pid(&the_car.PID.pid_car_gyro, the_car.IMU.gyro_x);
}

/*转向角速度控制*/
void TurnGyro_PID_Control(void)
{
//	if(the_car.bl_cmd & 0x1000)
//	{
//		the_car.PID.pid_turn_gyro.target +=10.0f;
//		the_car.bl_cmd &= ~(0x1000);
//	}
//	else if(the_car.bl_cmd & 0x0800)
//	{
//		the_car.PID.pid_turn_gyro.target -=10.0f;
//		the_car.bl_cmd &= ~(0x0800);		
//	}
//	else if(the_car.bl_cmd==0)
//	{
//		the_car.PID.pid_turn_gyro.target =0.0f;
//	}
	the_car.PID.pid_turn_gyro.f_cal_pid(&the_car.PID.pid_turn_gyro, the_car.IMU.gyro_z);
}

/*转向位置控制*/
void TurnPosition_PID_Control(void)
{
	//the_car.PID.pid_turn_angle.target = the_car.start_yaw;
	//the_car.PID.pid_turn_angle.target += the_car.PID.pid_turn_position.f_cal_pid(&pid_turn_position,the_car.VS.pos_err);
	
	the_car.PID.pid_turn_gyro.target = the_car.PID.pid_turn_position.f_cal_pid(&the_car.PID.pid_turn_position,the_car.VS.pos_err);

}

/*目标位移控制*/
void TargetDistance_PID_Control(void)
{
	//the_car.PID.pid_target_distance.target += the_car.target_speed *(1.0f-fabs((float)the_car.VS.pos_err)*the_car.target_speed_K);//设定运行速度

//	if(the_car.PID.pid_target_speed.target < 0.0f)
//		the_car.PID.pid_target_speed.target = 0.0f;//避免倒车      
	// if(the_car.bl_cmd & 0x4000)
	// {
	// 	the_car.PID.pid_target_speed.target +=0.05f;
	// 	the_car.bl_cmd &= ~(0x4000);
	// }
	// else if(the_car.bl_cmd & 0x2000)
	// {
	// 	the_car.PID.pid_target_speed.target -=0.05f;
	// 	the_car.bl_cmd &= ~(0x2000);		
	// }
//	else if(the_car.bl_cmd==0)
//	{
//		the_car.PID.pid_target_speed.target =0.0f;           
//	}
	if(the_car.VS.state & 0x80)
		the_car.PID.pid_target_distance.target = 0.0f;
	
	the_car.PID.pid_target_distance.f_cal_pid(&the_car.PID.pid_target_distance, the_car.run_distance);
}

/*电位器角度控制*/
void potAngle_PID_Control(void)
{
	the_car.PID.pid_ptm_angle.f_cal_pid(&the_car.PID.pid_ptm_angle, (the_car.potentiometer_voltage+the_car.IMU.roll*the_car.ptm_angle_k - the_car.ptm_angle_offset));
}

/*控制器初始化*/
void Control_Init(void)
{

	/***电机速度环***/
 	pid_init(&the_car.PID.pid_Lspeed);
	the_car.PID.pid_Lspeed.f_param_init(&the_car.PID.pid_Lspeed,PID_Speed,MAX_MOTOR_DUTY,MAX_MOTOR_DUTY,0,0,0.0f,0.0f,0.0f,0.0f);
	the_car.PID.pid_Lspeed.enable = 1;//电机初始就启动速度闭环
	pid_init(&the_car.PID.pid_Rspeed);
	the_car.PID.pid_Rspeed.f_param_init(&the_car.PID.pid_Rspeed,PID_Speed,MAX_MOTOR_DUTY,MAX_MOTOR_DUTY,0,0,0.0f,0.0f,0.0f,0.0f);  
	the_car.PID.pid_Rspeed.enable = 1;//电机初始就启动速度闭环

	/***直立环***/
	pid_init(&the_car.PID.pid_car_angle);
	the_car.PID.pid_car_angle.f_param_init(&the_car.PID.pid_car_angle,PID_Position,MAX_MOTOR_SPEED,0,0,0,0,0,0,0);  
	pid_init(&the_car.PID.pid_car_gyro);
	the_car.PID.pid_car_gyro.f_param_init(&the_car.PID.pid_car_gyro,PID_Position,MAX_MOTOR_SPEED,0,0,0,0.0,0.0f,0.0f,0.0f);
	pid_init(&the_car.PID.pid_ptm_angle);
	the_car.PID.pid_ptm_angle.f_param_init(&the_car.PID.pid_ptm_angle,PID_Position,MAX_MOTOR_SPEED,0,0,0,0.0f,0.0f,0.0f,0.0f);
	/***位移环***/
	pid_init(&the_car.PID.pid_target_distance);
	the_car.PID.pid_target_distance.f_param_init(&the_car.PID.pid_target_distance,PID_Position,MAX_MOTOR_SPEED,0,0,0,0.0f,0.0f,0.0f,0.0f);

	/***转向环***/
	pid_init(&the_car.PID.pid_turn_gyro);	
	the_car.PID.pid_turn_gyro.f_param_init(&the_car.PID.pid_turn_gyro,PID_Position,MAX_MOTOR_SPEED,0,0,0,0.0f,0.0f,0.0f,0.0f);
	pid_init(&the_car.PID.pid_turn_angle);	
	the_car.PID.pid_turn_angle.f_param_init(&the_car.PID.pid_turn_angle,PID_Position,MAX_TURN_GYRO,0,0,0,0.0f,0.0f,0.0f,0.0f);
	pid_init(&the_car.PID.pid_turn_position);	
	the_car.PID.pid_turn_position.f_param_init(&the_car.PID.pid_turn_position,PID_Position,9999,0,0,0,0.0f,0.0f,0.0f,0.0f);
}

/*获取电池电压*/
float get_battery_voltage(void)
{
    float voltage = 0.0f;
    uint32_t ADC_value = 0;
    ADC_value = HAL_ADC_GetValue(&hadc3);
    voltage = ADC_value * BAT_VOLT_K; // ADC值转换为真实电池电压
    the_car.battery_voltage = voltage;
    return voltage;
}

/*获取电位器电压*/
float get_potentiometer_voltage(void)
{
    float voltage = 0.0f;
    uint32_t ADC_value = 0;
    ADC_value = HAL_ADC_GetValue(&hadc1);
    voltage = ADC_value * POTENTIOMETER_VOLT_K; // ADC值转换为真实电位器电压
    the_car.potentiometer_voltage_rate = voltage - the_car.potentiometer_voltage; // 计算电位器电压变化率
    the_car.potentiometer_voltage = voltage;
    return voltage;
}



