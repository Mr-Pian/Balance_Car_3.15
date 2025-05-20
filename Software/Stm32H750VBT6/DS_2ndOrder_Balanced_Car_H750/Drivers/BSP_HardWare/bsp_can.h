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

/// 2024_7 
/// Modified by
/// UESTC-School Of Automation Engineering-Technology Association-Turtle
**************/
#ifndef __BSP_CAN
#define __BSP_CAN


#include "stm32h7xx_hal.h"

#include "mytype.h"
#include "fdcan.h"


extern FDCAN_TxHeaderTypeDef TxHeader1;
extern FDCAN_RxHeaderTypeDef RxHeader1;
extern uint8_t CAN_RxData[];
extern uint8_t CAN_TxData[];

/*CAN发送或是接收的ID*/


typedef enum
{

	CAN_2006Moto_ALL_ID = 0x200,
	CAN_2006Moto1_ID = 0x201,
	CAN_2006Moto2_ID = 0x202,
	CAN_2006Moto3_ID = 0x203,
	CAN_2006Moto4_ID = 0x204,
	
}CAN_Message_ID;


#define FILTER_BUF_LEN		5
/*接收到的无刷电机的参数结构体*/
typedef struct{
	int16_t	 	speed_rpm;
    float  	real_current;
    int16_t  	given_current;
    uint8_t  	hall;
	uint16_t 	angle;				//abs angle range:[0,8191]
	uint16_t 	last_angle;	//abs angle range:[0,8191]
	uint16_t	offset_angle;
	int32_t		round_cnt;
	int32_t		total_angle;
	uint8_t			buf_idx;
	uint16_t			angle_buf[FILTER_BUF_LEN];
	uint16_t			fited_angle;
	uint32_t			msg_cnt;
}moto_measure_t;

extern moto_measure_t moto_chassis[];
extern uint8_t CAN_RxData[];

/* Extern  ------------------------------------------------------------------*/
extern moto_measure_t  moto_chassis[];

void FDCAN1_Config(void) ;

void get_moto_measure(moto_measure_t *ptr, FDCAN_HandleTypeDef* hcan);
void can_receive_onetime(FDCAN_HandleTypeDef* _hcan, int time);
void set_moto_current(FDCAN_HandleTypeDef* hcan, int16_t iq1, int16_t iq2, int16_t iq3, int16_t iq4);
#endif
