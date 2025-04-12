#ifndef __UART_UNPACK_H__
#define __UART_UNPACK_H__
#include "main.h"
#include "control.h"
#include "usart.h"

#define USART_RX_BUF_NUM 128 // ��������С
#define IMU_DATA_SIZE 29 // IMU����֡��
#define BL_DATA_SIZE 1 // ��������֡��
#define VS_DATA_SIZE 10 // �Ӿ�����֡��
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
}uint16_data_typedef;

void Get_Imu_Data(car_typedef* hcar, uint8_t* Buffer,uint32_t size);

extern uint8_t USART1_RX_BUF[USART_RX_BUF_NUM];  //�������ڽ��ջ�������
extern uint8_t USART2_RX_BUF[USART_RX_BUF_NUM];  //�Ӿ����ڽ��ջ�������
extern uint8_t USART3_RX_BUF[USART_RX_BUF_NUM];  //imu���ڽ��ջ�������

#endif
