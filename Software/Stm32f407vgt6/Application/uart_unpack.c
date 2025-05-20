#include "uart_unpack.h"
#include "usart.h"

float_data_typedef temp_float_data;
uint16_data_typedef temp_u16_data;
uint8_t USART1_RX_BUF[USART_RX_BUF_NUM];  //imu串口接收缓冲数组
uint8_t USART2_RX_BUF[USART_RX_BUF_NUM];  //imu串口接收缓冲数组
uint8_t USART3_RX_BUF[USART_RX_BUF_NUM];  //imu串口接收缓冲数组

static void Get_BL_Data(uint8_t* Buffer,uint32_t size);
static void Get_Imu_Data(car_typedef* hcar, uint8_t* Buffer,uint32_t size);
static void Get_VS_Data(uint8_t* Buffer,uint32_t size);

//串口接收空闲中断回调函数
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if(huart->Instance == USART1)//蓝牙串口
	{  
		uint8_t cnt = USART_RX_BUF_NUM - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);

		/*解析协议*/
		Get_BL_Data(USART1_RX_BUF,cnt);
		/*缓冲清零*/
		 memset(USART1_RX_BUF, 0, cnt);

	}
	else if (huart->Instance == USART2)  //视觉数据接收串口
	{
		uint8_t cnt = USART_RX_BUF_NUM - __HAL_DMA_GET_COUNTER(&hdma_usart2_rx);

		/*解析协议*/
		Get_VS_Data(USART2_RX_BUF,cnt);
		/*缓冲清零*/
		 memset(USART2_RX_BUF, 0, cnt);
	}
	else if (huart->Instance == USART3)  //imu接收串口
	{
      uint8_t cnt = USART_RX_BUF_NUM - __HAL_DMA_GET_COUNTER(&hdma_usart3_rx);

    /*解析协议*/		
		Get_Imu_Data(&the_car, USART3_RX_BUF,cnt);  

		/*缓冲清零*/
		 memset(USART3_RX_BUF, 0, cnt);
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
static void Get_Imu_Data(car_typedef* hcar, uint8_t* Buffer,uint32_t size)
{
	int Frame_header = 0;  //帧头

	if(size < IMU_DATA_SIZE)//接收数据量不够
		return;

	for (int i=0; i<size - IMU_DATA_SIZE; i++)  //在缓冲区里找帧头
	{
		
		if (Buffer[i] == 0x5A && Buffer[i+1] == 0xA5 && Buffer[i+IMU_DATA_SIZE-1]  == 0x0D && Buffer[i+IMU_DATA_SIZE] == 0x0A)  //找到有效帧
		{
			Frame_header = i;
			the_car.Imu->is_ok = 1;//IMU初始化完成
			break;
		}
	}
	if(Frame_header==0 && !(Buffer[Frame_header] == 0x5A && Buffer[Frame_header+1] == 0xA5 && Buffer[Frame_header+IMU_DATA_SIZE-1]  == 0x0D && Buffer[Frame_header+IMU_DATA_SIZE] == 0x0A))//没找到
		return;
	else
		memcpy(hcar->Imu, &Buffer[Frame_header+2], 4*10);  //赋值
}



/*解析蓝牙指令*/
static void Get_BL_Data(uint8_t* Buffer,uint32_t size)
{
	if(size !=BL_DATA_SIZE)
		return;

	/*解析指令*/
	switch(Buffer[0])
	{
		case 0x30://停车
 				BL_CMD =0;
 				break;
 		case 0x31://发车
				BL_CMD |= 0x8000;
				break;
 		case 0x32://前进
				BL_CMD |= 0x4000;
				break;
 		case 0x33://后退
				BL_CMD |= 0x2000;
				break;
 		case 0x34://向左
				BL_CMD |= 0x1000;
				break;
 		case 0x35://向右
				BL_CMD |= 0x0800;
				break;
 		case 0x36://关闭
				BL_CMD |= 0x0400;
				break;
	}
}

/*解析视觉数据*/
uint32_t vs_data_rx_cnt=0;
static void Get_VS_Data(uint8_t* Buffer,uint32_t size)
{
	if(size <VS_DATA_SIZE)
		return;
	uint32_t i=0;
	char str[5];
	
	for (i=0; i<size - (VS_DATA_SIZE-1); i++)  //在缓冲区里找帧头
	{
		memcpy(str,&Buffer[i],5);
		
		if (strcmp(str,"$LINE") == 0)  //找到有效帧
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
