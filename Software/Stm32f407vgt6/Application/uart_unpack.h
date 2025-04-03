#ifndef __UART_UNPACK_H__
#define __UART_UNPACK_H__
#include "main.h"
#include "control.h"
#include "usart.h"

//float���չ�����
typedef union
{
	float Data;
	uint8_t Data_bytes[4];
}float_data_typedef;

//uint16���չ�����
typedef union
{
	uint16_t Data;
	uint8_t Data_bytes[2];
}uint8_data_typedef;

void Get_Imu_Data(car_typedef* hcar, uint8_t* Buffer);

extern uint8_t Received_Buffer_1[128];  //ȫ�ִ��ڻ���������
#endif
