#include "main.h"
#include "control.h"
#include "key.h"
#include "stdio.h"

void key_scan()
{
	static uint8_t  delay_sta=0;
	uint8_t  key_value = 0xFF;
	//�������͵�ƽ��Ч
	key_value = ( ((uint8_t)KEY_OK_PIN)<<5|((uint8_t)KEY_BACK_PIN)<<4|((uint8_t)KEY_UP_PIN)<<3|((uint8_t)KEY_DOWN_PIN)<<2|((uint8_t)KEY_LEFT_PIN)<<1|((uint8_t)KEY_RIGHT_PIN) );//�ɼ�����������״̬
	
	switch(key_value)
	{		
		case 0x1F://ȷ��
			if(delay_sta==0) //��ʱ
			{
				delay_sta =1;
				return;
			}
			delay_sta=0;
			the_car.KEY->OK=1;//ȷ�ϼ�����
		break;
		case 0x2F://����
			if(delay_sta==0) //��ʱ
			{
				delay_sta =1;
				return;
			}
			delay_sta=0;
			the_car.KEY->BACK=1;//���ؼ�����
		break;
		case 0x37://��
		if(delay_sta==0) //��ʱ
			{
				delay_sta =1;
				return;
			}
			delay_sta=0;
			the_car.KEY->UP=1;//�ϼ�����
		break;
		case 0x3B://��
		if(delay_sta==0) //��ʱ
			{
				delay_sta =1;
				return;
			}
			delay_sta=0;
			the_car.KEY->DOWN=1;//�¼�����
		break;
		case 0x0F://��
		if(delay_sta==0) //��ʱ
			{
				delay_sta =1;
				return;
			}
			delay_sta=0;
			the_car.KEY->LEFT=1;//�������
		break;
		case 0x33://��
		if(delay_sta==0) //��ʱ
			{
				delay_sta =1;
				return;
			}
			delay_sta=0;
			the_car.KEY->RIGHT=1;//�Ҽ�����
		break;
		default://�������
			delay_sta=0;//��λ
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

