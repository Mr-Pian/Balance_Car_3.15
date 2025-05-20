#include "uart_unpack.h"
#include "usart.h"

float_data_typedef temp_float_data;
uint16_data_typedef temp_u16_data;
uint8_t USART1_RX_BUF[USART_RX_BUF_NUM];  //imu���ڽ��ջ�������
uint8_t USART2_RX_BUF[USART_RX_BUF_NUM];  //imu���ڽ��ջ�������
uint8_t USART3_RX_BUF[USART_RX_BUF_NUM];  //imu���ڽ��ջ�������

static void Get_BL_Data(uint8_t* Buffer,uint32_t size);
static void Get_Imu_Data(car_typedef* hcar, uint8_t* Buffer,uint32_t size);
static void Get_VS_Data(uint8_t* Buffer,uint32_t size);

//���ڽ��տ����жϻص�����
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if(huart->Instance == USART1)//��������
	{  
		uint8_t cnt = USART_RX_BUF_NUM - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);

		/*����Э��*/
		Get_BL_Data(USART1_RX_BUF,cnt);
		/*��������*/
		 memset(USART1_RX_BUF, 0, cnt);

	}
	else if (huart->Instance == USART2)  //�Ӿ����ݽ��մ���
	{
		uint8_t cnt = USART_RX_BUF_NUM - __HAL_DMA_GET_COUNTER(&hdma_usart2_rx);

		/*����Э��*/
		Get_VS_Data(USART2_RX_BUF,cnt);
		/*��������*/
		 memset(USART2_RX_BUF, 0, cnt);
	}
	else if (huart->Instance == USART3)  //imu���մ���
	{
      uint8_t cnt = USART_RX_BUF_NUM - __HAL_DMA_GET_COUNTER(&hdma_usart3_rx);

    /*����Э��*/		
		Get_Imu_Data(&the_car, USART3_RX_BUF,cnt);  

		/*��������*/
		 memset(USART3_RX_BUF, 0, cnt);
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
static void Get_Imu_Data(car_typedef* hcar, uint8_t* Buffer,uint32_t size)
{
	int Frame_header = 0;  //֡ͷ

	if(size < IMU_DATA_SIZE)//��������������
		return;

	for (int i=0; i<size - IMU_DATA_SIZE; i++)  //�ڻ���������֡ͷ
	{
		
		if (Buffer[i] == 0x5A && Buffer[i+1] == 0xA5 && Buffer[i+IMU_DATA_SIZE-1]  == 0x0D && Buffer[i+IMU_DATA_SIZE] == 0x0A)  //�ҵ���Ч֡
		{
			Frame_header = i;
			the_car.Imu->is_ok = 1;//IMU��ʼ�����
			break;
		}
	}
	if(Frame_header==0 && !(Buffer[Frame_header] == 0x5A && Buffer[Frame_header+1] == 0xA5 && Buffer[Frame_header+IMU_DATA_SIZE-1]  == 0x0D && Buffer[Frame_header+IMU_DATA_SIZE] == 0x0A))//û�ҵ�
		return;
	else
		memcpy(hcar->Imu, &Buffer[Frame_header+2], 4*10);  //��ֵ
}



/*��������ָ��*/
static void Get_BL_Data(uint8_t* Buffer,uint32_t size)
{
	if(size !=BL_DATA_SIZE)
		return;

	/*����ָ��*/
	switch(Buffer[0])
	{
		case 0x30://ͣ��
 				BL_CMD =0;
 				break;
 		case 0x31://����
				BL_CMD |= 0x8000;
				break;
 		case 0x32://ǰ��
				BL_CMD |= 0x4000;
				break;
 		case 0x33://����
				BL_CMD |= 0x2000;
				break;
 		case 0x34://����
				BL_CMD |= 0x1000;
				break;
 		case 0x35://����
				BL_CMD |= 0x0800;
				break;
 		case 0x36://�ر�
				BL_CMD |= 0x0400;
				break;
	}
}

/*�����Ӿ�����*/
uint32_t vs_data_rx_cnt=0;
static void Get_VS_Data(uint8_t* Buffer,uint32_t size)
{
	if(size <VS_DATA_SIZE)
		return;
	uint32_t i=0;
	char str[5];
	
	for (i=0; i<size - (VS_DATA_SIZE-1); i++)  //�ڻ���������֡ͷ
	{
		memcpy(str,&Buffer[i],5);
		
		if (strcmp(str,"$LINE") == 0)  //�ҵ���Ч֡
		{
			the_car.vs->state |= ~(0x80);
			the_car.vs->pos_err = (Buffer[i+5]-'0')*1000 + (Buffer[i+6]-'0')*100 + (Buffer[i+7]-'0')*10 + (Buffer[i+8]-'0') - the_car.vs->pos_err_offset;
			vs_data_rx_cnt++;
			break;
		}
		else if(strcmp(str,"$LOST") == 0)
		{
			the_car.vs->state |= 0x80;
			the_car.vs->pos_err = (Buffer[i+5]-'0')*1000 + (Buffer[i+6]-'0')*100 + (Buffer[i+7]-'0')*10 + (Buffer[i+8]-'0') - the_car.vs->pos_err_offset;
			vs_data_rx_cnt++;
			break;				
		}
	}


}
