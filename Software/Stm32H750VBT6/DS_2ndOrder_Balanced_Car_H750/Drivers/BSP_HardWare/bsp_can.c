/******************************************************************************
/// @brief
/// @copyright Copyright (c) 2017 <dji-innovations, Corp. RM Dept.>
/// @license MIT License
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction,including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense,and/or sell
/// copies of the Software, and to permit persons to whom the Software is furnished
/// to do so,subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
/// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
/// THE SOFTWARE.
*******************************************************************************/

/*************
///大疆C610电调 CAN驱动  STM32H750VBT6版
///可利用STM32的CAN外设配合收发器模块（例如TJA1050）控制C610电调，驱动M2006电机（理论上也可驱动M3508，未测试）
///接线方式：
///stm32          tja1050
///GND------------GND
///CANRX----------RX
///CANTX----------TX
///(开发板)5V-----VCC
///注意配置CAN波特率为1M
/// 2024_7 
/// Modified by
/// UESTC-School Of Automation Engineering-Technology Association-Turtle
**************/
#include "fdcan.h"
#include "bsp_can.h"


moto_measure_t moto_chassis[4] = {0};//4 chassis moto
/*发送信息和接收信息的消息头 包含信息标志ID等信息。旧版看库文件中这两个Header包含在hcan实例结构体中，新版需要自己定义*/
FDCAN_TxHeaderTypeDef TxHeader1;
FDCAN_RxHeaderTypeDef RxHeader1;
/*接收消息和发送消息的缓冲区数组*/
uint8_t CAN_RxData[8]={0};
uint8_t CAN_TxData[8]={0};

 uint32_t FlashTimer;


extern FDCAN_TxHeaderTypeDef TxHeader1;
extern FDCAN_RxHeaderTypeDef RxHeader1;
extern uint8_t CAN_RxData[];
extern uint8_t CAN_TxData[];

 uint32_t FlashTimer;

void get_total_angle(moto_measure_t *p);
void get_moto_offset(moto_measure_t *ptr, FDCAN_HandleTypeDef* hcan);

//初始化FDCAN1，并配置滤波器。
void FDCAN1_Config(void) 
{
	FDCAN_FilterTypeDef sFilterConfig;
 
  /* Configure Rx filter */
  sFilterConfig.IdType = FDCAN_STANDARD_ID;
  sFilterConfig.FilterIndex = 0;
  sFilterConfig.FilterType = FDCAN_FILTER_RANGE;
  sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
  sFilterConfig.FilterID1 = 0x200;
  sFilterConfig.FilterID2 = 0x209;
  if (HAL_FDCAN_ConfigFilter(&hfdcan1, &sFilterConfig) != HAL_OK)
  {
    Error_Handler();
  }
	
 
  /* Configure global filter:
     Filter all remote frames with STD and EXT ID
     Reject non matching frames with STD ID and EXT ID */
  if (HAL_FDCAN_ConfigGlobalFilter(&hfdcan1, FDCAN_REJECT, FDCAN_REJECT, FDCAN_FILTER_REMOTE, FDCAN_FILTER_REMOTE) != HAL_OK)
  {
    Error_Handler();
  }
 
  /* Start the FDCAN module */
  if (HAL_FDCAN_Start(&hfdcan1) != HAL_OK)
  {
    Error_Handler();
  }
 
  if (HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
  {
    Error_Handler();
  }
}

/*******************************************************************************************
  * @Func			void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
  * @Brief    HAL库中标准的CAN接收完成回调函数，需要在此处理通过CAN总线接收到的数据
  *           新版HAL库中FDCAN的库函数结构发生了较大的变化，这里已经修改为新版的中断回调
  * @Param		
  * @Retval		None 
  * @Date     2015/11/24
  * @DateUpdated 2024/7
 *******************************************************************************************/
//记得在cubemx中打开接收中断回调，否则无法接收can总线传回的信息
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{

	if(HAL_GetTick() - FlashTimer>500){
			
		//HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
		FlashTimer = HAL_GetTick();
		
	}
  HAL_FDCAN_GetRxMessage(&hfdcan1,FDCAN_RX_FIFO0,&RxHeader1,CAN_RxData);
	//ignore can1 or can2.
	switch(RxHeader1.Identifier){
		case CAN_2006Moto1_ID:
		case CAN_2006Moto2_ID:
		case CAN_2006Moto3_ID:
		case CAN_2006Moto4_ID:
			{
				static uint8_t i;
				i = RxHeader1.Identifier - CAN_2006Moto1_ID;
				
				get_moto_measure(&moto_chassis[i], hfdcan); //将接收到的信息解析为电机测量数据
			}
			break;
		
		
	}
		
	/*#### add enable can it again to solve can receive only one ID problem!!!####**/
	//__HAL_CAN_ENABLE_IT(&hfdcan1, CAN_IT_FMP0);


}


/*******************************************************************************************
  * @Func			void get_moto_measure(moto_measure_t *ptr, CAN_HandleTypeDef* hcan)
  * @Brief    接收3508电机通过CAN发过来的信息
  * @Param		
  * @Retval		None
  * @Date     2015/11/24
 *******************************************************************************************/
void get_moto_measure(moto_measure_t *ptr, FDCAN_HandleTypeDef* hcan)
{

	ptr->last_angle = ptr->angle;
	ptr->angle = (uint16_t)(CAN_RxData[0]<<8 | CAN_RxData[1]) ;
	ptr->speed_rpm  = (int16_t)(CAN_RxData[2]<<8 | CAN_RxData[3]);
	ptr->real_current = (CAN_RxData[4]<<8 | CAN_RxData[5])*5.f/16384.f;

	ptr->hall = CAN_RxData[6];
	
	
	if(ptr->angle - ptr->last_angle > 4096)
		ptr->round_cnt --;
	else if (ptr->angle - ptr->last_angle < -4096)
		ptr->round_cnt ++;
	ptr->total_angle = ptr->round_cnt * 8192 + ptr->angle - ptr->offset_angle;
}

/*this function should be called after system+can init */
void get_moto_offset(moto_measure_t *ptr, FDCAN_HandleTypeDef* hcan)
{
	ptr->angle = (uint16_t)(CAN_RxData[0]<<8 | CAN_RxData[1]) ;
	ptr->offset_angle = ptr->angle;
}

#define ABS(x)	( (x>0) ? (x) : (-x) )
/**
*@bref 电机上电角度=0， 之后用这个函数更新3510电机的相对开机后（为0）的相对角度。
	*/
void get_total_angle(moto_measure_t *p)
{
	
	int res1, res2, delta;
	if(p->angle < p->last_angle){			//可能的情况
		res1 = p->angle + 8192 - p->last_angle;	//正转，delta=+
		res2 = p->angle - p->last_angle;				//反转	delta=-
	}else{	//angle > last
		res1 = p->angle - 8192 - p->last_angle ;//反转	delta -
		res2 = p->angle - p->last_angle;				//正转	delta +
	}
	//不管正反转，肯定是转的角度小的那个是真的
	if(ABS(res1)<ABS(res2))
		delta = res1;
	else
		delta = res2;

	p->total_angle += delta;
	p->last_angle = p->angle;
}

//FDCAN发送函数
//设置四个电机的电流
void set_moto_current(FDCAN_HandleTypeDef* hcan, int16_t iq1, int16_t iq2, int16_t iq3, int16_t iq4)
{
  TxHeader1.Identifier = 0x200;
  TxHeader1.IdType = FDCAN_STANDARD_ID;
  TxHeader1.TxFrameType = FDCAN_DATA_FRAME;
  TxHeader1.DataLength = FDCAN_DLC_BYTES_8;	
  TxHeader1.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
  TxHeader1.BitRateSwitch = FDCAN_BRS_OFF;
  TxHeader1.FDFormat = FDCAN_CLASSIC_CAN;
  TxHeader1.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
  TxHeader1.MessageMarker = 0;
  
  CAN_TxData[0] = (iq1 >> 8);
	CAN_TxData[1] = iq1;
	CAN_TxData[2] = (iq2 >> 8);
	CAN_TxData[3] = iq2;
	CAN_TxData[4] = iq3 >> 8;
	CAN_TxData[5] = iq3;
	CAN_TxData[6] = iq4 >> 8;
	CAN_TxData[7] = iq4;
  
  HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader1, CAN_TxData);

}


