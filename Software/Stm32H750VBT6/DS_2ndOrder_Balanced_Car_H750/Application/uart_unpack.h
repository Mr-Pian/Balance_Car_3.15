#ifndef __UART_UNPACK_H__
#define __UART_UNPACK_H__
#include "main.h"
#include "control.h"
#include "usart.h"

#define USART_RX_BUF_NUM 128 // ��������С
#define IMU_DATA_SIZE (44-1) // IMU����֡��
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



extern uint8_t USART1_RX_BUF[USART_RX_BUF_NUM];  //�������ڽ��ջ�������
extern uint8_t UART4_RX_BUF[USART_RX_BUF_NUM];  //�Ӿ����ڽ��ջ�������
extern uint8_t UART8_RX_BUF[USART_RX_BUF_NUM];  //imu���ڽ��ջ�������

extern uint32_t vs_data_rx_cnt;

#endif
