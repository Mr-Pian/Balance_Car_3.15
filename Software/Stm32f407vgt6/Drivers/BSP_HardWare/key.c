#include "main.h"
#include "control.h"
#include "key.h"
#include "stdio.h"

void key_scan()
{
	static uint8_t  delay_sta=0;
	uint8_t  key_value = 0xFF;
	//按键拉低电平有效
	key_value = ( ((uint8_t)KEY_OK_PIN)<<5|((uint8_t)KEY_BACK_PIN)<<4|((uint8_t)KEY_UP_PIN)<<3|((uint8_t)KEY_DOWN_PIN)<<2|((uint8_t)KEY_LEFT_PIN)<<1|((uint8_t)KEY_RIGHT_PIN) );//采集六个按键的状态
	
	switch(key_value)
	{		
		case 0x1F://确认
			if(delay_sta==0) //延时
			{
				delay_sta =1;
				return;
			}
			delay_sta=0;
			the_car.KEY->OK=1;//确认键按下
		break;
		case 0x2F://返回
			if(delay_sta==0) //延时
			{
				delay_sta =1;
				return;
			}
			delay_sta=0;
			the_car.KEY->BACK=1;//返回键按下
		break;
		case 0x37://上
		if(delay_sta==0) //延时
			{
				delay_sta =1;
				return;
			}
			delay_sta=0;
			the_car.KEY->UP=1;//上键按下
		break;
		case 0x3B://下
		if(delay_sta==0) //延时
			{
				delay_sta =1;
				return;
			}
			delay_sta=0;
			the_car.KEY->DOWN=1;//下键按下
		break;
		case 0x0F://左
		if(delay_sta==0) //延时
			{
				delay_sta =1;
				return;
			}
			delay_sta=0;
			the_car.KEY->LEFT=1;//左键按下
		break;
		case 0x33://右
		if(delay_sta==0) //延时
			{
				delay_sta =1;
				return;
			}
			delay_sta=0;
			the_car.KEY->RIGHT=1;//右键按下
		break;
		default://其它情况
			delay_sta=0;//复位
			the_car.KEY->OK=0;
			the_car.KEY->BACK=0;
			the_car.KEY->UP=0;
			the_car.KEY->DOWN=0;
			the_car.KEY->LEFT=0;
			the_car.KEY->RIGHT=0;
			return;
		
	}
}

//void key_init()
//{
//	key_ok_PIN = 1;
//	KEY_BACK_PIN = 1;
//	KEY_UP_PIN = 1;
//	KEY_DOWN_PIN = 1;
//	KEY_LEFT_PIN = 1;
//	KEY_RIGHT_PIN = 1;
//}

