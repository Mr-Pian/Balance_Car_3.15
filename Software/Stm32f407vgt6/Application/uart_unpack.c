#include "uart_unpack.h"

float_data_typedef temp_float_data;
uint8_data_typedef temp_u16_data;
uint8_t Received_Buffer_1[128];  //imu串口接收缓冲数组

//串口接收空闲中断回调函数
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if (huart->Instance == USART3)  //imu接收串口
	{
		
		Get_Imu_Data(&the_car, Received_Buffer_1);  //解包
		memset(Received_Buffer_1,0x00,128);  //接收前清空缓冲区
		HAL_UARTEx_ReceiveToIdle_DMA(&huart3, Received_Buffer_1, 128); //开启空闲中断
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
	uint8_t unpack_enable = 0;  //解包使能
	uint8_t Frame_header = 0;  //帧头
	for (int i=127; i>=0; i--)  //找接收到的最后一帧帧头
	{
		if (Buffer[i] == 0x5A && Buffer[i+1] == 0xA5)
		{
			unpack_enable = 1;
			Frame_header = i;
			break;
		}
	}
	
	while (unpack_enable)  //验证是否能找到帧头
	{
		if (Buffer[Frame_header+1+12+14+1] == 0x0D && Buffer[Frame_header+1+12+14+1+1] == 0x0A)  //验证帧尾
		{
			for (int j=0; j<3; j++)  //先解包欧拉角
			{
				for (int k=0; k<4; k++)
				{
					temp_float_data.Data_bytes[k] = Buffer[Frame_header+2+(j*4)+k];  //处理完一个float
				}
				
				switch (j)  //赋值
				{
					case 0:
						hcar->Imu->roll = temp_float_data.Data;
						break;
					case 1:
						hcar->Imu->pitch = temp_float_data.Data;
						break;
					case 2:
						hcar->Imu->yaw = temp_float_data.Data;
						break;
				}
				memset(temp_float_data.Data_bytes, 0, 4);  //清空union
			}
			for (int j=0; j<7; j++)  //再解包原始数据
			{
				for (int k=0; k<2; k++)
				{
					temp_u16_data.Data_bytes[k] = Buffer[Frame_header+2+12+1+k];
				}
				
				switch (j)  //赋值
				{
					case 0:
						hcar->Imu->gyro_x = temp_u16_data.Data;
						break;
					case 1:
						hcar->Imu->gyro_y = temp_u16_data.Data;
						break;
					case 2:
						hcar->Imu->gyro_z = temp_u16_data.Data;
						break;
					case 3:
						hcar->Imu->acc_x = temp_u16_data.Data;
						break;
					case 4:
						hcar->Imu->acc_y = temp_u16_data.Data;
						break;
					case 5:
						hcar->Imu->acc_z = temp_u16_data.Data;
						break;
					case 6:
						hcar->Imu->temp = temp_u16_data.Data;
						break;
				}
				memset(temp_u16_data.Data_bytes, 0, 2);
			}
		}
		else
		{
			if (Frame_header <= 0)
			{
				return;
			}
			for (int i=Frame_header; i>=0; i--)  //继续尝试找（可能存在的）前一帧的帧头
			{
				if (Buffer[i] == 0x5A && Buffer[i+1] == 0xA5)
				{
					unpack_enable = 1;
					Frame_header = i;
					break;
				}
			}
		}
	}
	
	return;
}
