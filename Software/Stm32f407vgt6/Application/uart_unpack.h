#ifndef __UART_UNPACK_H__
#define __UART_UNPACK_H__
#include "main.h"
#include "control.h"
#include "usart.h"

//float接收共用体
typedef union
{
	float Data;
	uint8_t Data_bytes[4];
}float_data_typedef;

//uint16接收共用体
typedef union
{
	uint16_t Data;
	uint8_t Data_bytes[2];
}uint8_data_typedef;

void Get_Imu_Data(car_typedef* hcar, uint8_t* Buffer);

extern uint8_t Received_Buffer_1[128];  //全局串口缓冲区数组
#endif
