#include "control.h"
#include "main.h"
#include "adc.h"
#include "tim.h"
#include "pid.h"
#include "angle.h"
#include "math.h"
#include "stdlib.h"
#include "usart.h"
#include "cJSON_usr.h"
#include "fatfs.h"
AttiudeController AttitudeControl;//姿态控制
HeightController HeightControl;//高度控制
PositionController PositionControl;//位置控制
Motor_TypeDef aircraft_motor = {0,0,0,0,0,1,1,1,1};//电机结构体


/*空心杯电机控制begin*/
//电机转速设置
void motor_speed_set()
{
	/***电机电压补偿处理begin***/
	aircraft_motor.duty1 *=aircraft_motor.volt_k1;
	aircraft_motor.duty2 *=aircraft_motor.volt_k2;
	aircraft_motor.duty3 *=aircraft_motor.volt_k3;
	aircraft_motor.duty4 *=aircraft_motor.volt_k4;
	/***电机电压补偿处理end***/	
	
	/***限幅处理begin***/
	if(aircraft_motor.duty1>MOTOR_MAX_DUTY)		aircraft_motor.duty1 = MOTOR_MAX_DUTY;
	else if(aircraft_motor.duty1<MOTOR_MIN_DUTY)		aircraft_motor.duty1 = MOTOR_MIN_DUTY;
	if(aircraft_motor.duty2>MOTOR_MAX_DUTY)		aircraft_motor.duty2 = MOTOR_MAX_DUTY;
	else if(aircraft_motor.duty2<MOTOR_MIN_DUTY)		aircraft_motor.duty2 = MOTOR_MIN_DUTY;
	if(aircraft_motor.duty3>MOTOR_MAX_DUTY)		aircraft_motor.duty3 = MOTOR_MAX_DUTY;
	else if(aircraft_motor.duty3<MOTOR_MIN_DUTY)		aircraft_motor.duty3 = MOTOR_MIN_DUTY;
	if(aircraft_motor.duty4>MOTOR_MAX_DUTY)		aircraft_motor.duty4 = MOTOR_MAX_DUTY;
	else if(aircraft_motor.duty4<MOTOR_MIN_DUTY)		aircraft_motor.duty4 = MOTOR_MIN_DUTY;	
	/***限幅处理end***/	
	

	/***HAL库占空比设置***/
	if(my_aircraft.status&0x01)
	{
		__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_4,aircraft_motor.duty1);
		__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_1,aircraft_motor.duty2);
		__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_3,aircraft_motor.duty3);
		__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_4,aircraft_motor.duty4);
	}

	else
	{
		__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_4,0);
		__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_1,0);
		__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_3,0);
		__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_4,0);		
	}
}
//电机控制
void motor_control()
{
	/***占空比复位***/
	aircraft_motor.duty1=0;
	aircraft_motor.duty2=0;
	aircraft_motor.duty3=0;
	aircraft_motor.duty4=0;	

	/***姿态控制begin***/	
	aircraft_motor.duty1 +=(int)AttitudeControl.internal_pid.x.output;
	aircraft_motor.duty2 -=(int)AttitudeControl.internal_pid.x.output;
	aircraft_motor.duty3 +=(int)AttitudeControl.internal_pid.x.output;
	aircraft_motor.duty4 -=(int)AttitudeControl.internal_pid.x.output;	
	
	aircraft_motor.duty1 -=(int)AttitudeControl.internal_pid.y.output;
	aircraft_motor.duty2 -=(int)AttitudeControl.internal_pid.y.output;
	aircraft_motor.duty3 +=(int)AttitudeControl.internal_pid.y.output;
	aircraft_motor.duty4 +=(int)AttitudeControl.internal_pid.y.output;
	
	aircraft_motor.duty1 -=(int)AttitudeControl.internal_pid.z.output;
	aircraft_motor.duty2 +=(int)AttitudeControl.internal_pid.z.output;
	aircraft_motor.duty3 +=(int)AttitudeControl.internal_pid.z.output;
	aircraft_motor.duty4 -=(int)AttitudeControl.internal_pid.z.output;	
	
	/***姿态环输出限幅***/
	if(aircraft_motor.duty1<MOTOR_MIN_DUTY) aircraft_motor.duty1 =MOTOR_MIN_DUTY;
	if(aircraft_motor.duty2<MOTOR_MIN_DUTY) aircraft_motor.duty2 =MOTOR_MIN_DUTY;
	if(aircraft_motor.duty3<MOTOR_MIN_DUTY) aircraft_motor.duty3 =MOTOR_MIN_DUTY;
	if(aircraft_motor.duty4<MOTOR_MIN_DUTY) aircraft_motor.duty4 =MOTOR_MIN_DUTY;
	
	if(aircraft_motor.duty1>MOTOR_MAX_DUTY) aircraft_motor.duty1 =MOTOR_MAX_DUTY;
	if(aircraft_motor.duty2>MOTOR_MAX_DUTY) aircraft_motor.duty2 =MOTOR_MAX_DUTY;
  if(aircraft_motor.duty3>MOTOR_MAX_DUTY) aircraft_motor.duty3 =MOTOR_MAX_DUTY;
	if(aircraft_motor.duty4>MOTOR_MAX_DUTY) aircraft_motor.duty4 =MOTOR_MAX_DUTY;	
	/***姿态控制end***/	

	/***油门控制begin***/
	aircraft_motor.duty1 +=(int)aircraft_motor.throttle;
	aircraft_motor.duty2 +=(int)aircraft_motor.throttle;
	aircraft_motor.duty3 +=(int)aircraft_motor.throttle;
	aircraft_motor.duty4 +=(int)aircraft_motor.throttle;		
	/***油门控制end***/
	
	motor_speed_set();//电机驱动
}
//电机初始化
void motor_init()
{
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_4);	
}
//电机解除初始化
void motor_deinit()
{
	HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_3);
	HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_4);
	HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_4);	
}
/*空心杯电机控制end*/

/*电机油门控制begin*/
void motor_throttle_control()
{
	if(HeightControl.auto_height_control_isEnable)//自动定高油门控制
	{
		aircraft_motor.throttle = HeightControl.base_throttle + HeightControl.pid.output;
	}
	else//默认手动油门控制
	{
		if(remote_data_flash[0]==1)
		{
			if(abs((int)my_remote.YG_LEFT_UD-128)>50)
				aircraft_motor.throttle += ((int)my_remote.YG_LEFT_UD - 128)*0.3;
			else 
				aircraft_motor.throttle+=0;

			remote_data_flash[0] = 0;
		}
	}
	/***油门限幅***/
	if(aircraft_motor.throttle>MOTOR_MAX_THROTTLE)
		aircraft_motor.throttle = MOTOR_MAX_THROTTLE;
	else if(aircraft_motor.throttle<MOTOR_MIN_THROTTLE)
		aircraft_motor.throttle = MOTOR_MIN_THROTTLE;
}
/*电机油门控制end*/

/*飞行器飞行方向控制begin*/
void aircraft_flight_direction_control()
{
	static uint32_t poscontrol_cnt_4 = 0;
	static uint32_t poscontrol_cnt_10 = 0;
	if(PositionControl.auto_pos_control_isEnable)//开启自动定点
	{
		if(poscontrol_cnt_4>=4)//内环50Hz
		{
			
			AttitudeControl.pitch_target_angle = PositionControl.internal_pid_x.f_cal_pid(&PositionControl.internal_pid_x,*PositionControl.sensor.speed_x);
			
			AttitudeControl.roll_target_angle = - PositionControl.internal_pid_y.f_cal_pid(&PositionControl.internal_pid_y,*PositionControl.sensor.speed_y);		
			
			poscontrol_cnt_4 = 0;
		}
		if(poscontrol_cnt_10>=10)//外环20Hz
		{
			PositionControl.internal_pid_x.target = PositionControl.external_pid_x.f_cal_pid(&PositionControl.external_pid_x,*PositionControl.sensor.pos_x);
			
			PositionControl.internal_pid_y.target = PositionControl.external_pid_y.f_cal_pid(&PositionControl.external_pid_y,*PositionControl.sensor.pos_y);				
			poscontrol_cnt_10 = 0;
		}
		poscontrol_cnt_4++;
		poscontrol_cnt_10++;
	}
	else//手动方向控制
	{
		poscontrol_cnt_4 = 0;
		poscontrol_cnt_10 = 0;		
		if(remote_data_flash[1]==1)
		{
			if(abs((int)my_remote.YG_RIGHT_LR-128)>25)
			{
				if(my_remote.YG_RIGHT_LR>128)
				{
					AttitudeControl.roll_target_angle = -(float)(my_remote.YG_RIGHT_LR-128-25)*((float)ROLL_TARGET_MAX_ANGLE/102.0f);
				}
				else
				{
					AttitudeControl.roll_target_angle = -(float)(my_remote.YG_RIGHT_LR-128+25)*((float)ROLL_TARGET_MAX_ANGLE/103.0f);
				}
				
			}
			else
				AttitudeControl.roll_target_angle = 0;
			
			if(abs((int)my_remote.YG_RIGHT_UD-128)>25)	
			{
				if(my_remote.YG_RIGHT_UD>128)
				{
					AttitudeControl.pitch_target_angle = +(float)(my_remote.YG_RIGHT_UD-128-25)*((float)PITCH_TARGET_MAX_ANGLE/102.0f);
				}
				else
				{
					AttitudeControl.pitch_target_angle = +(float)(my_remote.YG_RIGHT_UD-128+25)*((float)PITCH_TARGET_MAX_ANGLE/103.0f);
				}
				
			}
			else
				AttitudeControl.pitch_target_angle = 0;
			
			if(abs((int)my_remote.YG_LEFT_LR-128)>100)
				AttitudeControl.yaw_target_angle += (float)(my_remote.YG_LEFT_LR-128)*0.008f;
			else
				AttitudeControl.yaw_target_angle -=0;
				
			remote_data_flash[1]=0;
		}
	}
		/****姿态外环输入begin****/
		if(AttitudeControl.roll_target_angle>ROLL_TARGET_MAX_ANGLE)
			AttitudeControl.roll_target_angle = ROLL_TARGET_MAX_ANGLE;
		else if(AttitudeControl.roll_target_angle<ROLL_TARGET_MIN_ANGLE)
			AttitudeControl.roll_target_angle = ROLL_TARGET_MIN_ANGLE;
		AttitudeControl.external_pid.x.target = AttitudeControl.roll_target_angle+AttitudeControl.roll_compensate;
		
		if(AttitudeControl.pitch_target_angle>PITCH_TARGET_MAX_ANGLE)
			AttitudeControl.pitch_target_angle = PITCH_TARGET_MAX_ANGLE;
		else if(AttitudeControl.pitch_target_angle<PITCH_TARGET_MIN_ANGLE)
			AttitudeControl.pitch_target_angle = PITCH_TARGET_MIN_ANGLE;	
		AttitudeControl.external_pid.y.target = AttitudeControl.pitch_target_angle+AttitudeControl.pitch_compensate;
		
		if(AttitudeControl.yaw_target_angle>YAW_TARGET_MAX_ANGLE)
			AttitudeControl.yaw_target_angle = YAW_TARGET_MAX_ANGLE;
		else if(AttitudeControl.yaw_target_angle<YAW_TARGET_MIN_ANGLE)
			AttitudeControl.yaw_target_angle = YAW_TARGET_MIN_ANGLE;	
		AttitudeControl.external_pid.z.target = AttitudeControl.yaw_target_angle;
		/****姿态外环输入end****/
}
/*飞行器飞行方向控制end*/

/*飞行器飞行高度控制begin*/

void aircraft_flight_Height_control()
{
	if(HeightControl.auto_height_control_isEnable)//开启自动定高
	{
		if(TOF.distance_m <3.0f&&TOF.confidence >80)//TOF测距小于3m 并且可信度高于80
		{
			HeightControl.mode = TOF_MODE;//TOF定高
			HeightControl.pid.f_pid_reset(&HeightControl.pid,HeightControl.tof_params.Kp,HeightControl.tof_params.Ki,HeightControl.tof_params.Kd);
		}
		else
		{
			HeightControl.mode = BARO_MODE;//气压计定高
			HeightControl.pid.f_pid_reset(&HeightControl.pid,HeightControl.baro_params.Kp,HeightControl.baro_params.Ki,HeightControl.baro_params.Kd);		
		}
		if(HeightControl.mode == BARO_MODE)//气压计定高
		{
			HeightControl.pid.target = HeightControl.target_altitude;
			HeightControl.pid.MaxOutput = MOTOR_MAX_THROTTLE - HeightControl.base_throttle;
			HeightControl.pid.IntegralLimit = HeightControl.pid.MaxOutput*0.5f;
			HeightControl.pid.f_cal_pid(&HeightControl.pid,*HeightControl.sensor.barometer_height);			
		}
		else if(HeightControl.mode == TOF_MODE)//TOF定高
		{
			HeightControl.pid.target =  HeightControl.target_height;
			HeightControl.pid.MaxOutput = MOTOR_MAX_THROTTLE - HeightControl.base_throttle;
			HeightControl.pid.IntegralLimit = HeightControl.pid.MaxOutput*0.5f;		
			HeightControl.pid.f_cal_pid(&HeightControl.pid,*HeightControl.sensor.tof_height);
		}
		else
			return;
	}
	else   
		return;
}
/*飞行器飞行高度控制end*/


void pid_control_init()
{
	/***姿态控制初始化begin***/
	pid_init(&AttitudeControl.internal_pid.x);
	AttitudeControl.sensor.gyro_x = &IMU_Data.gyro.x;
	pid_init(&AttitudeControl.internal_pid.y);
	AttitudeControl.sensor.gyro_y = &IMU_Data.gyro.y;
	pid_init(&AttitudeControl.internal_pid.z);
	AttitudeControl.sensor.gyro_z = &IMU_Data.gyro.z;
	AttitudeControl.internal_pid.x.f_param_init(&AttitudeControl.internal_pid.x,PID_Position,MOTOR_MAX_DUTY,0,0,0,0,25.0f,0.0f,22.0f);
	pid_enable(&AttitudeControl.internal_pid.x,0);//未开启飞机就不使能pid
	AttitudeControl.internal_pid.y.f_param_init(&AttitudeControl.internal_pid.y,PID_Position,MOTOR_MAX_DUTY,0,0,0,0,25.0f,0.0f,22.0f);
	pid_enable(&AttitudeControl.internal_pid.y,0);//未开启飞机就不使能pid
	AttitudeControl.internal_pid.z.f_param_init(&AttitudeControl.internal_pid.z,PID_Position,MOTOR_MAX_DUTY*0.3,0,0,0,0,60.0f,0.0f,15.0f);
	pid_enable(&AttitudeControl.internal_pid.z,0);//未开启飞机就不使能pid
	
	pid_init(&AttitudeControl.external_pid.x);
	AttitudeControl.sensor.roll = &Angle_Data.roll;
	pid_init(&AttitudeControl.external_pid.y);
	AttitudeControl.sensor.pitch = &Angle_Data.pitch;
	pid_init(&AttitudeControl.external_pid.z);
	AttitudeControl.sensor.yaw = &Angle_Data.yaw;
	AttitudeControl.external_pid.x.f_param_init(&AttitudeControl.external_pid.x,PID_Position,500,100,0,0,0,2.5f,0.025f,20.0f);
	pid_enable(&AttitudeControl.external_pid.x,0);//未开启飞机就不使能pid
	AttitudeControl.external_pid.y.f_param_init(&AttitudeControl.external_pid.y,PID_Position,500,100,0,0,0,2.5f,0.025f,20.0f);
	pid_enable(&AttitudeControl.external_pid.y,0);//未开启飞机就不使能pid
	AttitudeControl.external_pid.z.f_param_init(&AttitudeControl.external_pid.z,PID_Position,100,0,0,0,0,6.0f,0.0f,0.0f);	
	pid_enable(&AttitudeControl.external_pid.z,0);//未开启飞机就不使能pid
	/***姿态控制初始化end***/
	
	/***高度控制初始化begin***/
	pid_init(&HeightControl.pid);
	HeightControl.sensor.tof_height = &TOF.distance_m;
	HeightControl.sensor.barometer_height = &my_aircraft.Altitude;
	HeightControl.pid.f_param_init(&HeightControl.pid,PID_Position,0,0,0,0,0,300.0f,1.5f,2000.0f);
	pid_enable(&HeightControl.pid,0);//未开启定高就不使能pid
	HeightControl.mode = ALT_HOLD_DISABLED;//初始为手动控制

	/***高度控制初始化end***/	
	
	/***位置控制初始化begin***/
	pid_init(&PositionControl.internal_pid_x);
	pid_init(&PositionControl.internal_pid_y);
	PositionControl.sensor.speed_x = &OpticalFlow.flow_x_speed;
	PositionControl.sensor.speed_y = &OpticalFlow.flow_y_speed;
	PositionControl.internal_pid_x.f_param_init(&PositionControl.internal_pid_x,PID_Position,ROLL_TARGET_MAX_ANGLE,0,0,0,0,0.0f,0.0f,0.0f);
	PositionControl.internal_pid_y.f_param_init(&PositionControl.internal_pid_y,PID_Position,PITCH_TARGET_MAX_ANGLE,0,0,0,0,0.0f,0.0f,0.0f);
	pid_enable(&PositionControl.internal_pid_x,0);//未开启定点就不使能pid
	pid_enable(&PositionControl.internal_pid_y,0);//未开启定点就不使能pid

	pid_init(&PositionControl.external_pid_x);
	pid_init(&PositionControl.external_pid_y);
	PositionControl.sensor.pos_x = &OpticalFlow.flow_x_pos;
	PositionControl.sensor.pos_y = &OpticalFlow.flow_y_pos;
	PositionControl.external_pid_x.f_param_init(&PositionControl.external_pid_x,PID_Position,X_TARGET_MAX_SPEED,X_TARGET_MAX_SPEED,0,0,0,0.0f,0.0f,0.0f);
	PositionControl.external_pid_y.f_param_init(&PositionControl.external_pid_y,PID_Position,Y_TARGET_MAX_SPEED,Y_TARGET_MAX_SPEED,0,0,0,0.0f,0.0f,0.0f);
	pid_enable(&PositionControl.external_pid_x,0);//未开启定点就不使能pid
	pid_enable(&PositionControl.external_pid_y,0);//未开启定点就不使能pid
	/***位置控制初始化end***/
	
	fatfs_PID_params_read();//pid参数读取
	motor_init();//电机初始化
}

void pid_internal_control()
{ 

	/****pid计算begin****/
	AttitudeControl.internal_pid.x.f_cal_pid(&AttitudeControl.internal_pid.x,*AttitudeControl.sensor.gyro_x);//X轴

	AttitudeControl.internal_pid.y.f_cal_pid(&AttitudeControl.internal_pid.y,*AttitudeControl.sensor.gyro_y);//Y轴

	AttitudeControl.internal_pid.z.f_cal_pid(&AttitudeControl.internal_pid.z,*AttitudeControl.sensor.gyro_z);//Z轴

	/****pid计算end****/
	
}
void pid_external_control()
{    
	/****pid计算begin****/
	AttitudeControl.external_pid.x.f_cal_pid(&AttitudeControl.external_pid.x,*AttitudeControl.sensor.roll);//ROLL

	AttitudeControl.external_pid.y.f_cal_pid(&AttitudeControl.external_pid.y,*AttitudeControl.sensor.pitch);//PITCH

	AttitudeControl.external_pid.z.f_cal_pid(&AttitudeControl.external_pid.z,*AttitudeControl.sensor.yaw);//YAW

	/****pid计算end****/
	
	/****外环输出begin****/
	AttitudeControl.internal_pid.x.target = AttitudeControl.external_pid.x.output;
	
	AttitudeControl.internal_pid.y.target = AttitudeControl.external_pid.y.output;
	
	AttitudeControl.internal_pid.z.target = AttitudeControl.external_pid.z.output;
	/****外环输出end****/
}

/*飞行器保护措施begin*/
uint8_t aircraft_protection()
{
	static uint32_t Battery_Low_Power_cnt =0;
	static uint32_t Aircraft_IMU_Error_cnt =0;
	static uint32_t Motor_Duty_Error_cnt =0;
	
	if(my_aircraft.Battery_Volt <=32 )//电池低电量保护
	{
		Battery_Low_Power_cnt ++;
		if(Battery_Low_Power_cnt >= 50)//5s
		{
			motor_deinit();
			return 1;
		}
	}
	else 
	{
		Battery_Low_Power_cnt =0;
	}
	
	if(fabs(Angle_Data.roll)>60||fabs(Angle_Data.pitch)>60)//飞行器姿态错误保护
	{
		Aircraft_IMU_Error_cnt++;
		if(Aircraft_IMU_Error_cnt >= 2)//0.2s
		{
			motor_deinit();
			return 2;
		}		
	}
	else
	{
		Aircraft_IMU_Error_cnt=0;
	}
	
	if(aircraft_motor.duty1>=MOTOR_MAX_DUTY*0.95f||aircraft_motor.duty2>=MOTOR_MAX_DUTY*0.95f||aircraft_motor.duty3>=MOTOR_MAX_DUTY*0.95f||aircraft_motor.duty4>=MOTOR_MAX_DUTY*0.95f)//电机高占空比保护
	{
		Motor_Duty_Error_cnt++;
		if(Motor_Duty_Error_cnt >= 15)//1.5s
		{
			motor_deinit();
			return 3;
		}		
	}
	else
	{
		Motor_Duty_Error_cnt=0;
	}	
	return 0;
	
}
/*飞行器保护措施end*/

/*读取FLASH中保存的参数*/
#define PARAMS_NAME "Param_s.txt"
#define PARAMS_MAX_READ_SIZE  4096
uint8_t fatfs_PID_params_read()
{
	cJSON *json,*json_get;
	char read_buf[PARAMS_MAX_READ_SIZE];
	if(fatfs_read_file(PARAMS_NAME,read_buf,PARAMS_MAX_READ_SIZE))
	{
		return 1;
	}
	

	json = cJSON_Parse((const char *)read_buf); //将得到的字符串解析成json形式
    // 检查解析是否成功
	if (json == NULL) 
		{
			const char *error_ptr = cJSON_GetErrorPtr();
			if (error_ptr != NULL) {
					printf("JSON Parse error，pos: %s\n", error_ptr);
			}
			return 1; // 直接返回，避免操作空指针
		}
	/****************************/
	/*	  测试将JSON打印出来	*/
	/***************************/
//	char *out_data = cJSON_Print(json);   //将json形式打印成正常字符串形式
//	printf("%s",out_data);
	
	json_get = cJSON_GetObjectItem( json ,"AttitudeControl.internal_pid.x.kp" );
	if(json_get->type == cJSON_Number)  //从json获取键值内容
	{
		AttitudeControl.internal_pid.x.kp = json_get->valuedouble;
	}
	json_get = cJSON_GetObjectItem( json ,"AttitudeControl.internal_pid.x.ki" );
	if(json_get->type == cJSON_Number)  //从json获取键值内容
	{
		AttitudeControl.internal_pid.x.ki = json_get->valuedouble;
 	
	}
	json_get = cJSON_GetObjectItem( json ,"AttitudeControl.internal_pid.x.kd" );
	if(json_get->type == cJSON_Number)  //从json获取键值内容
	{
		AttitudeControl.internal_pid.x.kd = json_get->valuedouble;
 	
	}
	json_get = cJSON_GetObjectItem( json ,"AttitudeControl.internal_pid.y.kp" );
	if(json_get->type == cJSON_Number)  //从json获取键值内容
	{
		AttitudeControl.internal_pid.y.kp = json_get->valuedouble;
 	
	}
	json_get = cJSON_GetObjectItem( json ,"AttitudeControl.internal_pid.y.ki" );
	if(json_get->type == cJSON_Number)  //从json获取键值内容
	{
		AttitudeControl.internal_pid.y.ki = json_get->valuedouble;
 	
	}
	json_get = cJSON_GetObjectItem( json ,"AttitudeControl.internal_pid.y.kd" );
	if(json_get->type == cJSON_Number)  //从json获取键值内容
	{
		AttitudeControl.internal_pid.y.kd = json_get->valuedouble;
 	
	}
	json_get = cJSON_GetObjectItem( json ,"AttitudeControl.internal_pid.z.kp" );
	if(json_get->type == cJSON_Number)  //从json获取键值内容
	{
		AttitudeControl.internal_pid.z.kp = json_get->valuedouble;
 	
	}
	json_get = cJSON_GetObjectItem( json ,"AttitudeControl.internal_pid.z.ki" );
	if(json_get->type == cJSON_Number)  //从json获取键值内容
	{
		AttitudeControl.internal_pid.z.ki = json_get->valuedouble;
 	
	}
	json_get = cJSON_GetObjectItem( json ,"AttitudeControl.internal_pid.z.kd" );
	if(json_get->type == cJSON_Number)  //从json获取键值内容
	{
		AttitudeControl.internal_pid.z.kd = json_get->valuedouble;
 	
	}	
	json_get = cJSON_GetObjectItem( json ,"AttitudeControl.external_pid.x.kp" );
	if(json_get->type == cJSON_Number)  //从json获取键值内容
	{
		AttitudeControl.external_pid.x.kp = json_get->valuedouble;
 	
	}
	json_get = cJSON_GetObjectItem( json ,"AttitudeControl.external_pid.x.ki" );
	if(json_get->type == cJSON_Number)  //从json获取键值内容
	{
		AttitudeControl.external_pid.x.ki = json_get->valuedouble;
 	
	}
	json_get = cJSON_GetObjectItem( json ,"AttitudeControl.external_pid.x.kd" );
	if(json_get->type == cJSON_Number)  //从json获取键值内容
	{
		AttitudeControl.external_pid.x.kd = json_get->valuedouble;
 	
	}
	json_get = cJSON_GetObjectItem( json ,"AttitudeControl.external_pid.y.kp" );
	if(json_get->type == cJSON_Number)  //从json获取键值内容
	{
		AttitudeControl.external_pid.y.kp = json_get->valuedouble;
 	
	}
	json_get = cJSON_GetObjectItem( json ,"AttitudeControl.external_pid.y.ki" );
	if(json_get->type == cJSON_Number)  //从json获取键值内容
	{
		AttitudeControl.external_pid.y.ki = json_get->valuedouble;
 	
	}
	json_get = cJSON_GetObjectItem( json ,"AttitudeControl.external_pid.y.kd" );
	if(json_get->type == cJSON_Number)  //从json获取键值内容
	{
		AttitudeControl.external_pid.y.kd = json_get->valuedouble;
 	
	}
	json_get = cJSON_GetObjectItem( json ,"AttitudeControl.external_pid.z.kp" );
	if(json_get->type == cJSON_Number)  //从json获取键值内容
	{
		AttitudeControl.external_pid.z.kp = json_get->valuedouble;
 	
	}
	json_get = cJSON_GetObjectItem( json ,"AttitudeControl.external_pid.z.ki" );
	if(json_get->type == cJSON_Number)  //从json获取键值内容
	{
		AttitudeControl.external_pid.z.ki = json_get->valuedouble;
 	
	}
	json_get = cJSON_GetObjectItem( json ,"AttitudeControl.external_pid.z.kd" );
	if(json_get->type == cJSON_Number)  //从json获取键值内容
	{
		AttitudeControl.external_pid.z.kd = json_get->valuedouble;
 	
	}	
	json_get = cJSON_GetObjectItem( json ,"HeightControl.tof_params.Kp" );
	if(json_get->type == cJSON_Number)  //从json获取键值内容
	{
		HeightControl.tof_params.Kp = json_get->valuedouble;
 	
	}	
	json_get = cJSON_GetObjectItem( json ,"HeightControl.tof_params.Ki" );
	if(json_get->type == cJSON_Number)  //从json获取键值内容
	{
		HeightControl.tof_params.Ki = json_get->valuedouble;
 	
	}	
	json_get = cJSON_GetObjectItem( json ,"HeightControl.tof_params.Kd");
	if(json_get->type == cJSON_Number)  //从json获取键值内容
	{
		HeightControl.tof_params.Kd = json_get->valuedouble;
	}		
//	json_get = cJSON_GetObjectItem( json ,"HeightControl.tof_params.max_output");
//	if(json_get->type == cJSON_Number)  //从json获取键值内容
//	{
//		HeightControl.tof_params.max_output = json_get->valuedouble;
//	}	
//	json_get = cJSON_GetObjectItem( json ,"HeightControl.tof_params.max_integral");
//	if(json_get->type == cJSON_Number)  //从json获取键值内容
//	{
//		HeightControl.tof_params.max_integral = json_get->valuedouble;
//	}		
	json_get = cJSON_GetObjectItem( json ,"HeightControl.baro_params.Kp" );
	if(json_get->type == cJSON_Number)  //从json获取键值内容
	{
		HeightControl.baro_params.Kp = json_get->valuedouble;
 	
	}	
	json_get = cJSON_GetObjectItem( json ,"HeightControl.baro_params.Ki" );
	if(json_get->type == cJSON_Number)  //从json获取键值内容
	{
		HeightControl.baro_params.Ki = json_get->valuedouble;
 	
	}	
	json_get = cJSON_GetObjectItem( json ,"HeightControl.baro_params.Kd");
	if(json_get->type == cJSON_Number)  //从json获取键值内容
	{
		HeightControl.baro_params.Kd = json_get->valuedouble;
	}		
//	json_get = cJSON_GetObjectItem( json ,"HeightControl.baro_params.max_output");
//	if(json_get->type == cJSON_Number)  //从json获取键值内容
//	{
//		HeightControl.baro_params.max_output = json_get->valuedouble;
//	}	
//	json_get = cJSON_GetObjectItem( json ,"HeightControl.baro_params.max_integral");
//	if(json_get->type == cJSON_Number)  //从json获取键值内容
//	{
//		HeightControl.baro_params.max_integral = json_get->valuedouble;
//	}		
//	json_get = cJSON_GetObjectItem( json ,"HeightControl.base_throttle");
//	if(json_get->type == cJSON_Number)  //从json获取键值内容
//	{
//		HeightControl.base_throttle = (int)json_get->valuedouble;
//	}	
	
	json_get = cJSON_GetObjectItem( json ,"PositionControl.internal_pid_x.kp");
	if(json_get->type == cJSON_Number)  //从json获取键值内容
	{
		PositionControl.internal_pid_x.kp = json_get->valuedouble;
	}		
	json_get = cJSON_GetObjectItem( json ,"PositionControl.internal_pid_x.ki");
	if(json_get->type == cJSON_Number)  //从json获取键值内容
	{
		PositionControl.internal_pid_x.ki = json_get->valuedouble;
	}	
	json_get = cJSON_GetObjectItem( json ,"PositionControl.internal_pid_x.kd");
	if(json_get->type == cJSON_Number)  //从json获取键值内容
	{
		PositionControl.internal_pid_x.kd = json_get->valuedouble;
	}	
	json_get = cJSON_GetObjectItem( json ,"PositionControl.internal_pid_y.kp");
	if(json_get->type == cJSON_Number)  //从json获取键值内容
	{
		PositionControl.internal_pid_y.kp = json_get->valuedouble;
	}		
	json_get = cJSON_GetObjectItem( json ,"PositionControl.internal_pid_y.ki");
	if(json_get->type == cJSON_Number)  //从json获取键值内容
	{
		PositionControl.internal_pid_y.ki = json_get->valuedouble;
	}	
	json_get = cJSON_GetObjectItem( json ,"PositionControl.internal_pid_y.kd");
	if(json_get->type == cJSON_Number)  //从json获取键值内容
	{
		PositionControl.internal_pid_y.kd = json_get->valuedouble;
	}
	json_get = cJSON_GetObjectItem( json ,"PositionControl.external_pid_x.kp");
	if(json_get->type == cJSON_Number)  //从json获取键值内容
	{
		PositionControl.external_pid_x.kp = json_get->valuedouble;
	}		
	json_get = cJSON_GetObjectItem( json ,"PositionControl.external_pid_x.ki");
	if(json_get->type == cJSON_Number)  //从json获取键值内容
	{
		PositionControl.external_pid_x.ki = json_get->valuedouble;
	}	
	json_get = cJSON_GetObjectItem( json ,"PositionControl.external_pid_x.kd");
	if(json_get->type == cJSON_Number)  //从json获取键值内容
	{
		PositionControl.external_pid_x.kd = json_get->valuedouble;
	}	
	json_get = cJSON_GetObjectItem( json ,"PositionControl.external_pid_y.kp");
	if(json_get->type == cJSON_Number)  //从json获取键值内容
	{
		PositionControl.external_pid_y.kp = json_get->valuedouble;
	}		
	json_get = cJSON_GetObjectItem( json ,"PositionControl.external_pid_y.ki");
	if(json_get->type == cJSON_Number)  //从json获取键值内容
	{
		PositionControl.external_pid_y.ki = json_get->valuedouble;
	}	
	json_get = cJSON_GetObjectItem( json ,"PositionControl.external_pid_y.kd");
	if(json_get->type == cJSON_Number)  //从json获取键值内容
	{
		PositionControl.external_pid_y.kd = json_get->valuedouble;
	}
	json_get = cJSON_GetObjectItem( json ,"aircraft_motor.volt_k1");
	if(json_get->type == cJSON_Number)  //从json获取键值内容
	{
		aircraft_motor.volt_k1 = json_get->valuedouble;
	}	
	json_get = cJSON_GetObjectItem( json ,"aircraft_motor.volt_k2");
	if(json_get->type == cJSON_Number)  //从json获取键值内容
	{
		aircraft_motor.volt_k2 = json_get->valuedouble;
	}
	json_get = cJSON_GetObjectItem( json ,"aircraft_motor.volt_k3");
	if(json_get->type == cJSON_Number)  //从json获取键值内容
	{
		aircraft_motor.volt_k3 = json_get->valuedouble;
	}
	json_get = cJSON_GetObjectItem( json ,"aircraft_motor.volt_k4");
	if(json_get->type == cJSON_Number)  //从json获取键值内容
	{
		aircraft_motor.volt_k4 = json_get->valuedouble;
	}	
	
	printf("param_read_succese\n");
	cJSON_Delete(json);  //释放内存 
	cJSON_Delete(json_get);  //释放内存 		
	return 0;
}


