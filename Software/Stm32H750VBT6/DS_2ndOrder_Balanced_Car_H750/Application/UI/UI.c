#include "UI.h"
#include "logo.h"
#include "LCD_ST7789S.h"
#include "EEPROM.h"
#include "delay.h"
#include "UI.h"
#include "key.h"
#include "main.h"
#include "stdlib.h"
#include "math.h"
#include "EEPROM.h"
#include "usart.h"


/*显示开机logo*/
void UI_Show_Logo()
{
//	uint8_t img_data[2];
//	uint16_t i=0;
//	TFTSPI_Set_Pos(0,0,159,37);   //定位图片显示区域
//	TFTSPI_DC_H;       //DC=1发送数据
//	for(i=0;i<12160;i+=2)//ABO_FLY
//	{
//		
//		img_data[0] = E2P_read_byte(0x12BF+i);
//		img_data[1] = E2P_read_byte(0x12BF+i+1);
//		HAL_SPI_Transmit(&hspi1,img_data,2, 0xffff);//发送数据
//	}
//	TFTSPI_Set_Pos(55,45,55+49,45+47);   //定位图片显示区域
//	TFTSPI_DC_H;       //DC=1发送数据	
//	for(i=0;i<4800;i+=2)//四旋翼
//	{
//		img_data[0] = E2P_read_byte(0x0000+i);
//		img_data[1] = E2P_read_byte(0x0000+i+1);
//		HAL_SPI_Transmit(&hspi1,img_data,2, 0xffff);//发送数据
//	}	
	delay_ms(500);
}
/*显示主菜单样式*/
void UI_Show_menu()
{
    static uint32_t i=0;
    static uint8_t move_i_flag = 0;
	TFTSPI_Set_Pos(0,0,240-1,60-1);   //定位图片显示区域
	TFTSPI_DC_H;       //DC=1发送数据
	HAL_SPI_Transmit(&hspi1,maodie_GIF[i],27840, 0xffff);//发送数据
    if(i<49 && move_i_flag == 0)
    {
        i++;
    }
    else if(i>=49 && move_i_flag == 0)
    {
        move_i_flag = 1;
    }
    else if(i>0 && move_i_flag == 1)
    {
        i--;
    }
    else if(i==0 && move_i_flag == 1)
    {
        move_i_flag = 0;    
    }
}
/*显示运行过程中的各项数据*/
void UI_Show_RUN_Data()
{
//		if((my_aircraft.status & 0x01) == 0x00)//飞机未启动
//		{
//			TFTSPI_Draw_Part(0,0,159,14,COLOR_BLACK);
//			TFT_Printf(117,0,COLOR_YELLOW,COLOR_BLACK,fsize_6X8,"rt:%2.1fV",my_remote.Battery_Volt);//TFT屏幕显示遥控电池电压
//			TFT_Printf(0,0,COLOR_YELLOW,COLOR_BLACK,fsize_6X8,"ac:%2.1fV",my_aircraft.Battery_Volt);//TFT屏幕显示无人机电池电压
//		}
//		else //飞机已启动
//		{
//			if((my_aircraft.status & 0x08))//飞行器异常
//			{
//				TFTSPI_Draw_Part(0,0,159,14,COLOR_RED);
//				TFT_Printf(117,0,COLOR_YELLOW,COLOR_RED,fsize_6X8,"rt:%2.1fV",my_remote.Battery_Volt);//TFT屏幕显示遥控电池电压
//				TFT_Printf(0,0,COLOR_YELLOW,COLOR_RED,fsize_6X8,"ac:%2.1fV",my_aircraft.Battery_Volt);//TFT屏幕显示无人机电池电压				
//			}
//			else
//			{
//				if((my_aircraft.status & 0x02) == 0x00)//手动油门模式
//				{
//					TFTSPI_Draw_Part(0,0,79,14,COLOR_GREEN);
//					TFT_Printf(0,0,COLOR_YELLOW,COLOR_GREEN,fsize_6X8,"ac:%2.1fV",my_aircraft.Battery_Volt);//TFT屏幕显示无人机电池电压		
//				}
//				else//自动油门模式
//				{
//					TFTSPI_Draw_Part(0,0,79,14,COLOR_WHITE);
//					TFT_Printf(0,0,COLOR_RED,COLOR_WHITE,fsize_6X8,"ac:%2.1fV",my_aircraft.Battery_Volt);//TFT屏幕显示无人机电池电压		
//				}
//				if((my_aircraft.status & 0x04) == 0x00)//手动方向模式
//				{
//					TFTSPI_Draw_Part(80,0,159,14,COLOR_GREEN);
//					TFT_Printf(117,0,COLOR_YELLOW,COLOR_GREEN,fsize_6X8,"rt:%2.1fV",my_remote.Battery_Volt);//TFT屏幕显示遥控电池电压
//				}
//				else//自动方向模式
//				{
//					TFTSPI_Draw_Part(80,0,159,14,COLOR_WHITE);
//					TFT_Printf(117,0,COLOR_RED,COLOR_WHITE,fsize_6X8,"rt:%2.1fV",my_remote.Battery_Volt);//TFT屏幕显示遥控电池电压	
//				}
//			}
//		}
//		TFT_Printf(10,85,COLOR_RED,COLOR_BLACK,fsize_6X8,"%3.2f %3.2f   %3.2f %3.2f  ",my_remote.YG_LEFT_UD,my_remote.YG_LEFT_LR,my_remote.YG_RIGHT_UD,my_remote.YG_RIGHT_LR);//TFT屏幕显示摇杆数据

//		TFT_Printf(95,95,COLOR_YELLOW,COLOR_BLACK,fsize_8X16,"RSI:%d%% ",my_remote.signal);//TFT屏幕显示遥控器信号强度			
//		TFT_Printf(95,113,COLOR_GREEN,COLOR_BLACK,fsize_8X16,"RER:%d%% ",my_remote.error_rate);//TFT屏幕显示遥控器误码率
//		TFT_Printf(0,95,COLOR_YELLOW,COLOR_BLACK,fsize_8X16,"ASI:%d%% ",my_aircraft.signal);//TFT屏幕显示飞行器信号强度
//		TFT_Printf(0,113,COLOR_GREEN,COLOR_BLACK,fsize_8X16,"AER:%d%% ",my_aircraft.error_rate);//TFT屏幕显示飞行器误码率
//	
//		TFT_Printf(5,15,COLOR_RED,COLOR_BLACK,fsize_6X8,"TH:%4d%%",my_aircraft.Throttle);//TFT屏幕显示飞行器油门
//		TFT_Printf(100,15,COLOR_CYAN,COLOR_BLACK,fsize_6X8,"HE:%3dcm",my_aircraft.Height);//TFT屏幕显示飞行器高度
//		TFT_Printf(5,25,ST7735_COLOR565(200,150,200),COLOR_BLACK,fsize_6X8,"AL:%4dm",my_aircraft.Altitude);//TFT屏幕显示飞行器海拔
//		TFT_Printf(100,25,ST7735_COLOR565(255,128,64),COLOR_BLACK,fsize_6X8,"TE:%3ddeg ",my_aircraft.Temperature);//TFT屏幕显示飞行器温度
//		TFT_Printf(30,35,COLOR_WHITE,COLOR_BLACK,fsize_6X8,"M1:%3d%%  M2:%3d%% ",my_aircraft.Motor_PWM_duty[0],my_aircraft.Motor_PWM_duty[1]);//TFT屏幕显示飞行器电机占空比
//		TFT_Printf(30,45,COLOR_WHITE,COLOR_BLACK,fsize_6X8,"M3:%3d%%  M4:%3d%% ",my_aircraft.Motor_PWM_duty[2],my_aircraft.Motor_PWM_duty[3]);//TFT屏幕显示飞行器电机占空比
//		TFT_Printf(10,57,COLOR_YELLOW,COLOR_BLACK,fsize_6X8,"X:%4dcm/s   Y:%4dcm/s ",my_aircraft.X_speed,my_aircraft.Y_speed);//TFT屏幕显示飞行器水平速度
//		TFT_Printf(5,65,COLOR_LIGHTGRAY,COLOR_BLACK,fsize_8X16,"R:%3d P:%3d Y:%4d ",my_aircraft.ROLL,my_aircraft.PITCH,my_aircraft.YAW);//TFT屏幕显示飞行器姿态角			
}


uint8_t getDigits(float num) 
{
   uint8_t count = 0;
		if((int)num==0) count=1;
   while (num >= 1 || num <= -1) {
       num /= 10;
       count++;
   }

   return count;
}

void run_func(void)
{

	TFTSPI_CLS(COLOR_BLACK);
	the_car.PID.pid_car_angle.enable = 1;
	the_car.PID.pid_car_gyro.enable = 1;
	the_car.PID.pid_target_distance.enable = 1;
	the_car.PID.pid_turn_gyro.enable = 1;
	the_car.PID.pid_turn_angle.enable = 1;
	the_car.PID.pid_turn_position.enable = 1;
    the_car.PID.pid_ptm_angle.enable = 1;
	the_car.start_yaw = the_car.IMU.yaw; 
	the_car.run_distance = 0.0f;
	while(1)
	{
        if(ui_key_falsh==0) //UI与KEY互斥量
        {
            /***运行界面***/

            /*****姿态信息*****/
            TFT_Printf(0,0,COLOR_WHITE,COLOR_BLACK,fsize_12X24,"ROLL:%f ",the_car.IMU.roll);
            TFT_Printf(0,24*1,COLOR_WHITE,COLOR_BLACK,fsize_12X24,"YAW:%f ",the_car.IMU.yaw);

            /*****速度信息*****/
            TFT_Printf(0,24*2,COLOR_WHITE,COLOR_BLACK,fsize_12X24,"L:%.2fR:%.2f  ",the_car.MOTOR_L.real_speed,the_car.MOTOR_R.real_speed);
            TFT_Printf(0,24*3,COLOR_WHITE,COLOR_BLACK,fsize_12X24,"SPEED:%.3f ",the_car.run_speed);	

            /*****位置信息*****/
            TFT_Printf(0,24*4+1,COLOR_WHITE,COLOR_BLACK,fsize_12X24,"ERR%4d FPS%3d ",the_car.VS.pos_err,the_car.VS.fps);			
            TFT_Printf(0,24*5+1,COLOR_RED,COLOR_BLACK,fsize_12X24,"DIS:%.3f ",the_car.run_distance);
            
            /***电位器信息***/
            TFT_Printf(0,24*6+1,COLOR_CYAN,COLOR_BLACK,fsize_12X24,"PTM:%f ",the_car.potentiometer_voltage);
            /****电池电压*****/
            TFT_Printf(0,24*9+1,COLOR_YELLOW,COLOR_BLACK,fsize_12X24,"VOLTAGE:%.3f ",the_car.battery_voltage);
            if(the_car.KEY.BACK == 1  || fabs(the_car.IMU.roll)>=50.0f)//按键退出或者蓝牙关闭
            {
                delay_ms(250);
                if(the_car.KEY.BACK == 1  || fabs(the_car.IMU.roll)>=50.0f)
                {

                    the_car.PID.pid_car_angle.enable = 0;
                    the_car.PID.pid_car_gyro.enable = 0;
                    the_car.PID.pid_target_distance.enable = 0;
                    the_car.PID.pid_turn_gyro.enable = 0;
                    the_car.PID.pid_turn_angle.enable = 0;
                    the_car.PID.pid_turn_position.enable = 0;   
                    the_car.PID.pid_ptm_angle.enable = 0;
                    the_car.start_yaw = 0;																																																																										
                    the_car.BL_CMD = 0;
                    return;
                }
            }
            ui_key_falsh = 1;//UI与KEY互斥量，表示UI界面一次运行成功
        }
	}
}

/***************************************************/
/*UI param界面*/
/***************************************************/
void param_func()
{
	//参数格式：{"参数名字:",&(浮点参数变量),&(无符号整型参数),E2P地址}
	struct _parameter_class para_list[] = 
		{
            {"Mtr_L_P:",&the_car.PID.pid_Lspeed.kp,NULL,EEPROM_ADDRESS_BASE+4*21},
            {"Mtr_L_I:",&the_car.PID.pid_Lspeed.ki,NULL,EEPROM_ADDRESS_BASE+4*22},
            {"Mtr_R_P:",&the_car.PID.pid_Rspeed.kp,NULL,EEPROM_ADDRESS_BASE+4*23},
            {"Mtr_R_I:",&the_car.PID.pid_Rspeed.ki,NULL,EEPROM_ADDRESS_BASE+4*24},
			{"Std_A_p:",&the_car.PID.pid_car_angle.kp,NULL,EEPROM_ADDRESS_BASE+4*0},
			//{"Std_A_i:",&the_car.PID.pid_car_angle.ki,NULL,EEPROM_ADDRESS_BASE+4*1},
			{"Std_A_d:",&the_car.PID.pid_car_angle.kd,NULL,EEPROM_ADDRESS_BASE+4*2},
            {"Ptm_A_p:",&the_car.PID.pid_ptm_angle.kp,NULL,EEPROM_ADDRESS_BASE+4*25},
            {"Ptm_A_d:",&the_car.PID.pid_ptm_angle.kd,NULL,EEPROM_ADDRESS_BASE+4*26},
			//{"Std_G_p:",&the_car.PID.pid_car_gyro.kp,NULL,EEPROM_ADDRESS_BASE+4*3},
			//{"Std_G_i:",&the_car.PID.pid_car_gyro.ki,NULL,EEPROM_ADDRESS_BASE+4*4},
			//{"Std_G_d:",&the_car.PID.pid_car_gyro.kd,NULL,EEPROM_ADDRESS_BASE+4*5},
			{"Dis_p:",&the_car.PID.pid_target_distance.kp,NULL,EEPROM_ADDRESS_BASE+4*6},
			//{"Spd_i:",&the_car.PID.pid_target_distance.ki,NULL,EEPROM_ADDRESS_BASE+4*7},
			{"Dis_d:",&the_car.PID.pid_target_distance.kd,NULL,EEPROM_ADDRESS_BASE+4*8},
			{"Turn_P_p:",&the_car.PID.pid_turn_position.kp,NULL,EEPROM_ADDRESS_BASE+4*9},
			//{"Turn_P_i:",&the_car.PID.pid_turn_position.ki,NULL,EEPROM_ADDRESS_BASE+4*10},
			{"Turn_P_d:",&the_car.PID.pid_turn_position.kd,NULL,EEPROM_ADDRESS_BASE+4*11},
//			{"Turn_A_p:",&the_car.PID.pid_turn_angle.kp,NULL,EEPROM_ADDRESS_BASE+4*18},
//			{"Turn_A_i:",&the_car.PID.pid_turn_angle.ki,NULL,EEPROM_ADDRESS_BASE+4*19},
//			{"Turn_A_d:",&the_car.PID.pid_turn_angle.kd,NULL,EEPROM_ADDRESS_BASE+4*20},
			{"Turn_G_p:",&the_car.PID.pid_turn_gyro.kp,NULL,EEPROM_ADDRESS_BASE+4*12},
			//{"Turn_G_i:",&the_car.PID.pid_turn_gyro.ki,NULL,EEPROM_ADDRESS_BASE+4*13},
			{"Turn_G_d:",&the_car.PID.pid_turn_gyro.kd,NULL,EEPROM_ADDRESS_BASE+4*14},	
			{"VS_OFFSET:",NULL,(uint32_t*)&the_car.VS.pos_err_offset,EEPROM_ADDRESS_BASE+4*15},	
			{"SPEED:",&the_car.target_speed,NULL,EEPROM_ADDRESS_BASE+4*16},
			{"SPEED_K:",&the_car.target_speed_K,NULL,EEPROM_ADDRESS_BASE+4*17},
            {"car_ofst:",&the_car.car_angle_offset,NULL,EEPROM_ADDRESS_BASE+4*27},
            {"ptm_ofst:",&the_car.ptm_angle_offset,NULL,EEPROM_ADDRESS_BASE+4*28},
            {"ptm_A_K:",&the_car.ptm_angle_k,NULL,EEPROM_ADDRESS_BASE+4*29},
			{".."}								//结尾标志,方便自动计算数量
		};
		run_parameter(para_list);
}
	

///***************************************************/
///*UI EEPROM界面*/
///***************************************************/
//void eeprom_updata_func()//通过串口上传EEPROM中的数值
//{
//	
//	
//	
//}


/***************************************************/
/*UI MAIN界面*/
/***************************************************/
void main_menu()
{
	
	struct option_class option_list[] = 
		{
			{"RUN",run_func},//运行
			{"PARAM",param_func},//调参
			{".."}								//结尾标志,方便自动计算数量
		};
        
		run_menu(option_list);
}

/**
 * 函    数：菜单运行
 * 参    数：选项列表
 * 返 回 值：无
 * 说    明：把选项列表显示出来,并根据按键事件执行相应操作
 */
void run_menu(struct option_class* option)
{
	uint32_t display_x_start = 10;
	uint32_t display_y_start = 60;
	uint8_t WORD_H = 34;
	int8_t Catch_i = 0;		//选中下标
	int8_t Cursor_i = 0;	//光标下标
	int8_t Show_i = 0; 		//显示起始下标
	int8_t Max = 0;			//选项数量
	int8_t i = 0;
	int8_t Show_d = 0;				//显示动画相关;
	static float Cursor_len_d0 = 0, Cursor_i_d0 = 0; 			//光标位置和长度的起点终点
	while(option[++Max].Name[0] != '.');// {Max++;}	//获取条目数量,如果文件名开头为'.'则为结尾;
	Max--;											//不打印".."
	
	for(i = 0; i <= Max; i++)				//计算选项名宽度;
	{		
		option[i].NameLen = Get_NameLen(option[i].Name);
	}
	TFTSPI_Draw_Part(display_x_start,display_y_start,TFT18W-display_x_start,TFT18H-5,COLOR_DARKGRAY);
	while(1)
	{
        UI_Show_menu();
        if(ui_key_falsh==0) //UI与KEY互斥量
        {
            if(the_car.KEY.UP==1)								//如果Up键按下;
            {
                Cursor_i -= 1;					//更新下标
                Catch_i -= 1;
                TFTSPI_Draw_Part(display_x_start,display_y_start,TFT18W-display_x_start,TFT18H-5,COLOR_DARKGRAY);
								the_car.KEY.UP=0;
            }
            if(the_car.KEY.DOWN==1)
            {
                Cursor_i += 1;					//更新下标
                Catch_i += 1;
                TFTSPI_Draw_Part(display_x_start,display_y_start,TFT18W-display_x_start,TFT18H-5,COLOR_DARKGRAY);
								the_car.KEY.DOWN=0;
            }
            if(Catch_i < 0) {Catch_i = Max;Cursor_i=Max;}			//循环选中下标
            if(Catch_i > Max) {Catch_i = 0;Cursor_i=0;}
            
            if(Cursor_i < 0) {Cursor_i = 0;}		//限制光标位置
            if(Cursor_i > 3) {Cursor_i = 3;}
            if(Cursor_i > Max) {Cursor_i = 0;}
            

        /**********************************************************/
        /*显示相关*/
            Show_i = Catch_i - Cursor_i;				//计算显示起始下标	
            for(i = 0; i < 5; i++)				//遍历显示选项名
            {	
                if(Show_i + i > Max ) {break;}
                TFT_ShowString(display_x_start+5, display_y_start+1+(i* WORD_H)+Show_d*(i+1), option[Show_i + i].Name,COLOR_WHITE,COLOR_DARKGRAY, fsize_16X32);
            }
            
        /**********************************************************/
        /*光标相关*/
            

            Cursor_i_d0 = Cursor_i * WORD_H; Cursor_len_d0 = TFT18W-display_x_start*2;
            
            //显示光标
            TFTSPI_Draw_Rectangle(display_x_start,display_y_start+ Cursor_i_d0, Cursor_len_d0,display_y_start+ Cursor_i_d0+WORD_H-1,COLOR_RED);			//矩形光标

        
            TFT_Printf(TFT18W-display_x_start-23,TFT18H-20,COLOR_GREEN,COLOR_BLACK,fsize_8X16, "%3d",Catch_i);//右下角显示选中下标;
            TFT_Printf(TFT18W-display_x_start-87,TFT18H-20,COLOR_RED,COLOR_WHITE,fsize_8X16, "BAT%.2f",the_car.battery_voltage);//右上角显示电池电压;
            TFT_Printf(TFT18W-display_x_start-151,TFT18H-20,COLOR_YELLOW,COLOR_BLACK,fsize_8X16, "PTM%.3f",the_car.potentiometer_voltage);//右上角显示电位器电压;
            TFT_Printf(TFT18W-display_x_start-220,TFT18H-20,COLOR_BLUE,COLOR_WHITE,fsize_8X16, "R%.3f",the_car.IMU.roll);//右上角显示角度;
            if(the_car.KEY.OK==1)//获取按键
            {
				the_car.KEY.OK=0;
                /*如果功能不为空则执行功能,否则返回*/
                if(option[Catch_i].func) 
                {
                    option[Catch_i].func();
                    TFTSPI_CLS(COLOR_BLACK);
                    UI_Show_menu();
                    TFTSPI_Draw_Part(display_x_start,display_y_start,TFT18W-display_x_start,TFT18H-5,COLOR_DARKGRAY);
					the_car.KEY.BACK=0;//处理返回死区
                }

            }
            if(the_car.KEY.BACK==1){the_car.KEY.BACK=0;return;}	//获取按键
            ui_key_falsh = 1;//UI与KEY互斥量，表示UI界面一次运行成功
        }
	}
}

/**
 * 函    数：调参运行
 * 参    数：选项列表
 * 返 回 值：无
 * 说    明：把选项列表显示出来,并根据按键事件执行相应操作
 */
void run_parameter(struct _parameter_class* option)
{
	uint8_t word_H = 24;//字高
	uint8_t word_W = 12;//字宽
	uint8_t max_line_num = 9;//一页最多显示的条目 
	uint16_t BackGround_Color = COLOR_BLACK;//背景色
	uint16_t Font_Color = COLOR_WHITE;//字体色
	int8_t Catch_i = 0;		//选中下标
	int8_t Cursor_i = 0;	//光标下标
	int8_t Show_i = 0; 		//显示起始下标
	int8_t Max = 0;			//选项数量
	int8_t i = 0;
	int8_t digit=0;//变量的位
	int8_t Show_d = 1;				//显示动画相关;
	storFloatData write_data;//浮点数存储
	static uint8_t Cursor_len_d0 = 0, Cursor_i_d0 = 0; 			//光标位置和长度的起点终点
	
		
	while(option[++Max].Name[0] != '.');// {Max++;}	//获取条目数量,如果文件名开头为'.'则为结尾;
	Max--;											//不打印".."
	
	for(i = 0; i <= Max; i++)				//计算选项名宽度;
	{		
		option[i].NameLen = Get_NameLen(option[i].Name);
	}
	TFTSPI_CLS(BackGround_Color);
	
	while(1)
	{
        if(ui_key_falsh==0) //UI与KEY互斥量
        {
            if(the_car.KEY.UP==1)								//如果Up键按下;
            {
                Cursor_i -= 1;					//更新下标
                Catch_i -= 1;
                TFTSPI_CLS(BackGround_Color);
								the_car.KEY.UP=0;
            }
            if(the_car.KEY.DOWN==1)
            {
                Cursor_i += 1;					//更新下标
                Catch_i += 1;
                TFTSPI_CLS(BackGround_Color);
								the_car.KEY.DOWN=0;
            }
            if(Catch_i < 0) {Catch_i = Max;Cursor_i=Max;}			//循环选中下标
            if(Catch_i > Max) {Catch_i = 0;Cursor_i=0;}
            
            if(Cursor_i < 0) {Cursor_i = 0;}		//限制光标位置
            if(Cursor_i > max_line_num-1) {Cursor_i = max_line_num-1;}
            if(Cursor_i > Max) {Cursor_i = 0;}
        /**********************************************************/
        /*显示相关*/
            Show_i = Catch_i - Cursor_i;				//计算显示起始下标	
            for(i = 0; i < max_line_num; i++)				//遍历显示选项名
            {	
                if(Show_i + i > Max ) {break;}
                if(option[Show_i+i].fpara !=NULL)//浮点参数
                    TFT_Printf(2,  (i* word_H)+Show_d*(i+1),Font_Color,BackGround_Color,fsize_12X24,"%s%.6f",option[Show_i+i].Name,*(option[Show_i+i].fpara));
                else if(option[Show_i+i].uipara !=NULL)//整形参数
                    TFT_Printf(2,  (i* word_H)+Show_d*(i+1),Font_Color,BackGround_Color,fsize_12X24,"%s%d",option[Show_i+i].Name,*(option[Show_i+i].uipara));
                
            }
            
        /**********************************************************/
        /*光标相关*/
            Cursor_i_d0 = Cursor_i * (word_H+1); Cursor_len_d0 = TFT18W-1;	
            //显示光标
            TFTSPI_Draw_Rectangle(0,Cursor_i_d0, Cursor_len_d0,Cursor_i_d0+(word_H+1),COLOR_RED);			//矩形光标

        
            TFT_Printf(TFT18W-23,TFT18H-15,COLOR_GREEN,COLOR_BLACK,fsize_8X16, "%3d",Catch_i);//右下角显示选中下标;

            if(the_car.KEY.OK==1)			//获取按键
            {
								the_car.KEY.OK=0;
                //进入调参
                while(1)
                {
                    if(ui_key_falsh==0) //UI与KEY互斥量
                    {
                        if(option[Catch_i].fpara !=NULL)//浮点参数
                            TFT_Printf(2,  (Cursor_i* word_H)+Show_d*(Cursor_i+1),COLOR_CYAN,BackGround_Color,fsize_12X24,"%s%.6f",option[Catch_i].Name,*(option[Catch_i].fpara));
                        else if(option[Catch_i].uipara !=NULL)//整形参数
                            TFT_Printf(2,  (Cursor_i* word_H)+Show_d*(Cursor_i+1),COLOR_CYAN,BackGround_Color,fsize_12X24,"%s%d",option[Catch_i].Name,*(option[Catch_i].uipara));
                        if(the_car.KEY.LEFT==1)//光标左移
                        {
                            digit -=1;	
														the_car.KEY.LEFT=0;
                        }				
                        else if(the_car.KEY.RIGHT==1)//光标右移
                        {
                            digit +=1;	
														the_car.KEY.RIGHT=0;
                        }
                        else if(the_car.KEY.UP==1)
                        {
                            if(option[Catch_i].fpara !=NULL)
                            {					
                                if(digit<getDigits(*(option[Catch_i].fpara)))
                                    *(option[Catch_i].fpara) +=(float)pow(10,getDigits(*(option[Catch_i].fpara))-1-digit);
                                else if(digit==getDigits(*(option[Catch_i].fpara)));
                                else *(option[Catch_i].fpara) +=(float)pow(10,getDigits(*(option[Catch_i].fpara))-digit);
                            }
                            else if(option[Catch_i].uipara !=NULL)
                            {
                                *(option[Catch_i].uipara) += pow(10,getDigits(*(option[Catch_i].uipara))-1-digit);					
                            }
														the_car.KEY.UP=0;
                        }
                        else if(the_car.KEY.DOWN==1)
                        {
                            if(option[Catch_i].fpara !=NULL)
                            {
                                if(digit<getDigits(*(option[Catch_i].fpara)))
                                    *(option[Catch_i].fpara) -=(float)pow(10,getDigits(*(option[Catch_i].fpara))-1-digit);
                                else if(digit==getDigits(*(option[Catch_i].fpara)));
                                else *(option[Catch_i].fpara) -=(float)pow(10,getDigits(*(option[Catch_i].fpara))-digit);
                            }
														else if(option[Catch_i].uipara !=NULL)
                            {
                                if(*(option[Catch_i].uipara)>0)
                                *(option[Catch_i].uipara) -= pow(10,getDigits(*(option[Catch_i].uipara))-1-digit);							
                            }
														the_car.KEY.DOWN=0;
                        }					
                        if(digit<0) digit	=0;
                        if(option[Catch_i].fpara !=NULL)
                        {
                            if(digit>getDigits(*(option[Catch_i].fpara))+6)  digit	=getDigits(*(option[Catch_i].fpara))+6;
                        }
                        else if(option[Catch_i].uipara !=NULL)
                        {
                            if(digit>getDigits(*(option[Catch_i].uipara)))  digit	=getDigits(*(option[Catch_i].fpara));		
                        }
                        //显示小光标
                        if(option[Catch_i].fpara !=NULL)
                        {
                            if(*(option[Catch_i].fpara)>=0)
                                TFTSPI_Draw_Rectangle(2+(digit+option[Catch_i].NameLen)*word_W,Cursor_i_d0+1, 2+(digit+option[Catch_i].NameLen)*word_W+word_W-1,Cursor_i_d0+word_H,COLOR_WHITE);			//矩形光标
                            else
                                TFTSPI_Draw_Rectangle(2+(digit+1+option[Catch_i].NameLen)*word_W,Cursor_i_d0+1, 2+(digit+1+option[Catch_i].NameLen)*word_W+word_W-1,Cursor_i_d0+word_H,COLOR_WHITE);			//矩形光标					
                        }
                        else if(option[Catch_i].uipara !=NULL)
                        {
                            TFTSPI_Draw_Rectangle(2+(digit+option[Catch_i].NameLen)*word_W,Cursor_i_d0+1, 2+(digit+option[Catch_i].NameLen)*word_W+word_W-1,Cursor_i_d0+word_H,COLOR_WHITE);			//矩形光标
                        }
                            
                        if(the_car.KEY.BACK==1)
                        {
                            /***参数保存***/
                            if(option[Catch_i].fpara !=NULL)
                            {
                                write_data.value = *(option[Catch_i].fpara);
                                E2P_WriteFloatNum(option[Catch_i].E2P_addr,write_data);
                            }
                            else if(option[Catch_i].uipara !=NULL)
                            {
                                E2P_WriteLenByte(option[Catch_i].E2P_addr, *option[Catch_i].uipara,4);
                            }
                            the_car.KEY.BACK=0;
                            break;
                        }	//按键返回	
                        ui_key_falsh = 1;//UI与KEY互斥量，表示UI界面一次运行成功
                    }			
                }
            }
            if(the_car.KEY.BACK==1){the_car.KEY.BACK=0;return;}	//获取按键
            ui_key_falsh = 1;//UI与KEY互斥量，表示UI界面一次运行成功
        }
	}
}

//计算选项名宽度;
uint8_t Get_NameLen(char* String)
{
	uint8_t i = 0, len = 0;
	while(String[i] != '\0')			//遍历字符串的每个字符
	{
		len += 1; i += 1;					//属于英文字符长度加1
	}
	return len;
}
