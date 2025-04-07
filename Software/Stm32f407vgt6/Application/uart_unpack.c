#include "uart_unpack.h"

float_data_typedef temp_float_data;
uint16_data_typedef temp_u16_data;
uint8_t Received_Buffer_1[128];  //imu���ڽ��ջ�������

//���ڽ��տ����жϻص�����
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if (huart->Instance == USART3)  //imu���մ���
	{
		
		Get_Imu_Data(&the_car, Received_Buffer_1);  //���
		memset(Received_Buffer_1,0x00,128);  //����ǰ��ջ�����
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
	int Frame_header = 0;  //֡ͷ
	for (int i=0; i<128; i++)  //�ڻ���������֡ͷ
	{
		if (Buffer[i] == 0x5A && Buffer[i+1] == 0xA5 && Buffer[i+27]  == 0x0D && Buffer[i+28] == 0x0A)  //�ҵ���Ч֡
		{
			Frame_header = i;
			break;
		}
	}
	
//	for(int i=0;i<26;i++)
//	{
//		((uint8_t *)(hcar->Imu))[i] = Buffer[Frame_header+2+i];
//	}
	memcpy(hcar->Imu, &Buffer[Frame_header+2], 12+14);  //��ֵ
}

