#include "uart_unpack.h"
#include "usart.h"

float_data_typedef temp_float_data;
uint16_data_typedef temp_u16_data;
uint8_t USART1_RX_BUF[USART_RX_BUF_NUM];  //IMU���ڽ��ջ�������
uint8_t UART4_RX_BUF[USART_RX_BUF_NUM];  //IMU���ڽ��ջ�������
uint8_t UART8_RX_BUF[USART_RX_BUF_NUM];  //IMU���ڽ��ջ�������

static void Get_BL_Data(uint8_t* Buffer,uint32_t size);
static void Get_IMU_Data(uint8_t* Buffer,uint32_t size);
static void Get_VS_Data(uint8_t* Buffer,uint32_t size);

//���ڽ��տ����жϻص�����
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if(huart->Instance == USART1)//�Ӿ�����
	{  
		/*����Э��*/
		Get_VS_Data(USART1_RX_BUF,Size);
		
		/*��������*/
		 memset(USART1_RX_BUF, 0, Size);

	}
	else if (huart->Instance == UART4)  //IMU����
	{

		/*����Э��*/
		Get_IMU_Data(UART4_RX_BUF,Size);  
		/*��������*/
		 memset(UART4_RX_BUF, 0, Size);
	}
	else if (huart->Instance == UART8)  //��������
	{
    /*����Э��*/		
		
		Get_BL_Data(UART8_RX_BUF,Size);
		/*��������*/
		 memset(UART8_RX_BUF, 0, Size);
	}
}

/**
  * @name   Get_IMU_Data(uint8_t* Buffer)
	*
	* @brief  �������ݽ����������ṹ���и�ֵ
  *           
  * @note   ��         
  *     
  * @param  car_typedef ���͵Ľṹ��ָ��, ���������ͷ��ַ
  * 
	* @retval void
  **/
static void Get_IMU_Data(uint8_t* Buffer,uint32_t size)
{
	int Frame_header = 0;  //֡ͷ

	if(size < IMU_DATA_SIZE)//��������������
		return;

	for (int i=0; i<size - IMU_DATA_SIZE; i++)  //�ڻ���������֡ͷ
	{
		
		if (Buffer[i] == 0x5A && Buffer[i+1] == 0xA5 && Buffer[i+IMU_DATA_SIZE-1]  == 0x0D && Buffer[i+IMU_DATA_SIZE] == 0x0A)  //�ҵ���Ч֡
		{
			Frame_header = i;
			the_car.IMU.is_ok = 1;//IMU��ʼ�����
			break;
		}
	}
	if(Frame_header==0 && !(Buffer[Frame_header] == 0x5A && Buffer[Frame_header+1] == 0xA5 && Buffer[Frame_header+IMU_DATA_SIZE-1]  == 0x0D && Buffer[Frame_header+IMU_DATA_SIZE] == 0x0A))//û�ҵ�
		return;
	else
		memcpy(&the_car.IMU, &Buffer[Frame_header+2], 4*10);  //��ֵ
}



/*��������ָ��*/
static void Get_BL_Data(uint8_t* Buffer,uint32_t size)
{
	if(size !=BL_DATA_SIZE)
		return;

	/*����ָ��*/
	switch(Buffer[0])
	{
		case 0x01://OK
 				the_car.BL_CMD = 1;
 				break;
 		case 0x02://BACK
		 		the_car.BL_CMD = 2;
				break;
 		case 0x03://UP	
		 		the_car.BL_CMD = 3;
				break;
 		case 0x04://DOWN
		 		the_car.BL_CMD = 4;
				break;
 		case 0x05://Left
		 		the_car.BL_CMD = 5;
				break;
 		case 0x06://Right
		 		the_car.BL_CMD = 6;
				break;
	}
}

/*�����Ӿ�����*/
uint32_t VS_data_rx_cnt=0;
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
			the_car.VS.state |= ~(0x80);
			the_car.VS.pos_err = (Buffer[i+5]-'0')*1000 + (Buffer[i+6]-'0')*100 + (Buffer[i+7]-'0')*10 + (Buffer[i+8]-'0') - the_car.VS.pos_err_offset;
			VS_data_rx_cnt++;
			break;
		}
		else if(strcmp(str,"$LOST") == 0)
		{
			the_car.VS.state |= 0x80;
			the_car.VS.pos_err = (Buffer[i+5]-'0')*1000 + (Buffer[i+6]-'0')*100 + (Buffer[i+7]-'0')*10 + (Buffer[i+8]-'0') - the_car.VS.pos_err_offset;
			VS_data_rx_cnt++;
			break;				
		}
	}


}
