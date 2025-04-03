#include "uart_unpack.h"

float_data_typedef temp_float_data;
uint8_data_typedef temp_u16_data;
uint8_t Received_Buffer_1[128];  //imu���ڽ��ջ�������

//���ڽ��տ����жϻص�����
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if (huart->Instance == USART3)  //imu���մ���
	{
		
		Get_Imu_Data(&the_car, Received_Buffer_1);  //���
		memset(Received_Buffer_1,0x00,128);  //����ǰ��ջ�����
		HAL_UARTEx_ReceiveToIdle_DMA(&huart3, Received_Buffer_1, 128); //���������ж�
	}
}

/**
  * @name   Get_Imu_Data(car_typedef* hcar, uint8_t* Buffer)
	*
	* @brief  �������ݽ����������ṹ���и�ֵ
  *           
  * @note   ��         
  *     
  * @param  car_typedef ���͵Ľṹ��ָ��, ���������ͷ��ַ
  * 
	* @retval void
  **/
void Get_Imu_Data(car_typedef* hcar, uint8_t* Buffer)
{
	uint8_t unpack_enable = 0;  //���ʹ��
	uint8_t Frame_header = 0;  //֡ͷ
	for (int i=127; i>=0; i--)  //�ҽ��յ������һ֡֡ͷ
	{
		if (Buffer[i] == 0x5A && Buffer[i+1] == 0xA5)
		{
			unpack_enable = 1;
			Frame_header = i;
			break;
		}
	}
	
	while (unpack_enable)  //��֤�Ƿ����ҵ�֡ͷ
	{
		if (Buffer[Frame_header+1+12+14+1] == 0x0D && Buffer[Frame_header+1+12+14+1+1] == 0x0A)  //��֤֡β
		{
			for (int j=0; j<3; j++)  //�Ƚ��ŷ����
			{
				for (int k=0; k<4; k++)
				{
					temp_float_data.Data_bytes[k] = Buffer[Frame_header+2+(j*4)+k];  //������һ��float
				}
				
				switch (j)  //��ֵ
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
				memset(temp_float_data.Data_bytes, 0, 4);  //���union
			}
			for (int j=0; j<7; j++)  //�ٽ��ԭʼ����
			{
				for (int k=0; k<2; k++)
				{
					temp_u16_data.Data_bytes[k] = Buffer[Frame_header+2+12+1+k];
				}
				
				switch (j)  //��ֵ
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
			for (int i=Frame_header; i>=0; i--)  //���������ң����ܴ��ڵģ�ǰһ֡��֡ͷ
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
