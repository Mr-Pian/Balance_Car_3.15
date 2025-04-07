#include "uart_unpack.h"

float_data_typedef temp_float_data;
uint16_data_typedef temp_u16_data;
uint8_t Received_Buffer_1[128];  //imu串口接收缓冲数组

//串口接收空闲中断回调函数
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if (huart->Instance == USART3)  //imu接收串口
	{
		
		Get_Imu_Data(&the_car, Received_Buffer_1);  //解包
		memset(Received_Buffer_1,0x00,128);  //接收前清空缓冲区
	}
}

/**
  * @name   Get_Imu_Data(car_typedef* hcar, uint8_t* Buffer)
	*
	* @brief  串口数据解包函数并向结构体中赋值
  *           
  * @note   无         
  *     
  * @param  car_typedef 类型的结构体指针, 缓冲数组的头地址
  * 
	* @retval void
  **/
void Get_Imu_Data(car_typedef* hcar, uint8_t* Buffer)
{
	int Frame_header = 0;  //帧头
	for (int i=0; i<128; i++)  //在缓冲区里找帧头
	{
		if (Buffer[i] == 0x5A && Buffer[i+1] == 0xA5 && Buffer[i+27]  == 0x0D && Buffer[i+28] == 0x0A)  //找到有效帧
		{
			Frame_header = i;
			break;
		}
	}
	
//	for(int i=0;i<26;i++)
//	{
//		((uint8_t *)(hcar->Imu))[i] = Buffer[Frame_header+2+i];
//	}
	memcpy(hcar->Imu, &Buffer[Frame_header+2], 12+14);  //赋值
}

