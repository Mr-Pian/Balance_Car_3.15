#ifndef __UART_UNPACK_H__
#define __UART_UNPACK_H__
#include "main.h"
#include "control.h"
#include "usart.h"

#define USART_RX_BUF_NUM 128 // 缓冲区大小
#define IMU_DATA_SIZE (44-1) // IMU数据帧长
#define BL_DATA_SIZE 1 // 蓝牙数据帧长
#define VS_DATA_SIZE 10 // 视觉数据帧长
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
}uint16_data_typedef;



extern uint8_t USART1_RX_BUF[USART_RX_BUF_NUM];  //蓝牙串口接收缓冲数组
extern uint8_t UART4_RX_BUF[USART_RX_BUF_NUM];  //视觉串口接收缓冲数组
extern uint8_t UART8_RX_BUF[USART_RX_BUF_NUM];  //imu串口接收缓冲数组

extern uint32_t vs_data_rx_cnt;

#endif
