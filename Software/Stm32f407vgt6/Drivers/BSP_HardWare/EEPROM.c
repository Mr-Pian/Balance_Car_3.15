#include "main.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "EEPROM.h"
#include "delay.h"
#include "i2c.h"
#include "control.h"
#include "UI.h"
/*引脚配置*********************/

/**
  * 函    数：E2PROM写SCL高低电平
  * 参    数：要写入SCL的电平值，范围：0/1
  * 返 回 值：无
  * 说    明：当上层函数需要写SCL时，此函数会被调用
  *           用户需要根据参数传入的值，将SCL置为高电平或者低电平
  *           当参数传入0时，置SCL为低电平，当参数传入1时，置SCL为高电平
  */
//void E2P_W_SCL(uint8_t BitValue)
//{
//	/*根据BitValue的值，将SCL置高电平或者低电平*/
//	
//	if(BitValue==0x00) E2P_IIC_SCL(0);
//	else  E2P_IIC_SCL(1);
//	
//	/*如果单片机速度过快，可在此添加适量延时，以避免超出I2C通信的最大速度*/
//	//...
//}

///**
//  * 函    数：E2PROM写SDA高低电平
//  * 参    数：要写入SDA的电平值，范围：0/1
//  * 返 回 值：无
//  * 说    明：当上层函数需要写SDA时，此函数会被调用
//  *           用户需要根据参数传入的值，将SDA置为高电平或者低电平
//  *           当参数传入0时，置SDA为低电平，当参数传入1时，置SDA为高电平
//  */
//void E2P_W_SDA(uint8_t BitValue)
//{
//	/*根据BitValue的值，将SDA置高电平或者低电平*/
//	if(BitValue==0x00) E2P_IIC_SDA(0);
//	else E2P_IIC_SDA(1);
//	/*如果单片机速度过快，可在此添加适量延时，以避免超出I2C通信的最大速度*/
//	//...
//}

/*********************引脚配置*/


/*通信协议*********************/

///**
//  * 函    数：I2C起始
//  * 参    数：无
//  * 返 回 值：无
//  */
//void E2P_I2C_Start(void)
//{
//	E2P_W_SDA(1);		//释放SDA，确保SDA为高电平
//	E2P_W_SCL(1);		//释放SCL，确保SCL为高电平
//	delay_us(4);
//	E2P_W_SDA(0);		//在SCL高电平期间，拉低SDA，产生起始信号
//	delay_us(4);
//	E2P_W_SCL(0);		//起始后把SCL也拉低，即为了占用总线，也为了方便总线时序的拼接
//}
///**
//  * 函    数：I2C终止
//  * 参    数：无
//  * 返 回 值：无
//  */
//void E2P_I2C_Stop(void)
//{
//	E2P_W_SCL(0);		//释放SCL，使SCL呈现低电平
//	E2P_W_SDA(0);		//拉低SDA，确保SDA为低电平
//	delay_us(4);
//	E2P_W_SCL(1);		//释放SCL，使SCL呈现高电平
//	E2P_W_SDA(1);		//在SCL高电平期间，释放SDA，产生终止信号
//	delay_us(4);
//}

/*************************************************************/
/*                      IIC读取应答位                        */
/*************************************************************/
//void E2P_I2C_readack(void) 
//{
//	uint8_t ack_wait_cnt=0;
//  E2P_W_SDA(1);
//	delay_us(1);
//  E2P_W_SCL(1);
//	delay_us(1);
//  while(E2P_IIC_SDA_PIN) 
//  {
//		//ack_wait_cnt++;
//  }
//  E2P_W_SCL(0);
//}

///*************************************************************/
///*                      IIC发送应答位                        */
///*************************************************************/
//void E2P_I2C_sendack(void) 
//{
//  E2P_W_SCL(0);
//  E2P_W_SDA(0);
//	delay_us(2);
//  E2P_W_SCL(1);
//	delay_us(2);
//  E2P_W_SCL(0);
//}

///*************************************************************/
///*                      IIC不发送应答位                      */
///*************************************************************/
//void E2P_I2C_sendnoack(void) 
//{
//  E2P_W_SCL(0);
//  E2P_W_SDA(1);
//	delay_us(2);
//  E2P_W_SCL(1);
//	delay_us(2);
//  E2P_W_SCL(0);
//}



///**
//  * 函    数：I2C发送一个字节
//  * 参    数：Byte 要发送的一个字节数据，范围：0x00~0xFF
//  * 返 回 值：无
//  */
//void E2P_I2C_SendByte(uint8_t Byte)
//{
//	uint8_t i;
//  E2P_W_SCL(0);//拉低时钟开始数据传输	
//	/*循环8次，主机依次发送数据的每一位*/
//	for (i = 0; i < 8; i++)
//	{
//		if((Byte & 0x80)>>7)//高位在先
//			E2P_W_SDA(1);
//		else 
//			E2P_W_SDA(0);
//		Byte<<=1;//把低位左移到高位待发
//		delay_us(2);
//		E2P_W_SCL(1);	//释放SCL，从机在SCL高电平期间读取SDA
//		delay_us(2);
//		E2P_W_SCL(0);	//拉低SCL，主机开始发送下一位数据
//		delay_us(2);
//	}
//	
//}
///**
//  * 函    数：I2C接收一个字节
//  * 参    数：
//  * 返 回 值：无
//  */
//unsigned char E2P_I2C_RecvByte(uint8_t ack) 
//{
// unsigned char i=0;
// unsigned char Data=0;
// E2P_W_SDA(1);

//	for(i=0;i<8;i++ )
//	{
//    E2P_W_SCL(0);  
//		delay_us(2);
//    E2P_W_SCL(1);
//    Data<<=1;
//    if(E2P_IIC_SDA_PIN) Data++;
//	 	delay_us(2);
//	}
//	if(ack==1)
//		E2P_I2C_sendack() ;
//	else
//		E2P_I2C_sendnoack() ;
//	
// return(Data);
//}
/*************************************************************/
/*             向AT24C02写入一个字节的数据                   */
/*          adr为写入的地址，Data为写入的数据                */
/*************************************************************/
void E2P_write_byte(uint16_t adr,unsigned char Data)
{
//  E2P_I2C_Start();
//  E2P_I2C_SendByte(AT24CXX_ADDR_WRITE);
//  E2P_I2C_readack();

//  E2P_I2C_SendByte(adr>>8);//发送高地址
//  E2P_I2C_readack();

//  E2P_I2C_SendByte(adr%0xff);//发送低地址
//  E2P_I2C_readack();	
//	
//  E2P_I2C_SendByte(Data);
//  E2P_I2C_readack();
//  E2P_I2C_Stop();
	uint8_t data_to_send[3] ={0,0,0};
	data_to_send[0] =  adr>>8;
	data_to_send[1] =  adr%0xff;
	data_to_send[2] =  Data;
	HAL_I2C_Master_Transmit(&IIC_Handler,AT24CXX_ADDR_WRITE,data_to_send,3,0x1000);
	delay_ms(6);
}

/*************************************************************/
/*             由AT24C02读取一个字节的数据                   */
/*          adr为读取的地址，返回值为读取的数据              */
/*************************************************************/
uint8_t E2P_read_byte(uint16_t adr)
{
//	uint8_t readdata;
//  E2P_I2C_Start();
//  E2P_I2C_SendByte(AT24CXX_ADDR_WRITE);
//  E2P_I2C_readack();

//  E2P_I2C_SendByte(adr>>8);//发送高地址
//  E2P_I2C_readack();

//  E2P_I2C_SendByte(adr%0xff);//发送低地址
//  E2P_I2C_readack();

//  E2P_I2C_Start();
//  E2P_I2C_SendByte(AT24CXX_ADDR_READ);//准备接收
//  E2P_I2C_readack();

//  readdata = E2P_I2C_RecvByte(0);
//  E2P_I2C_Stop();
	uint8_t data_to_send[2] ={0,0};
	uint8_t data_to_receive=0x00;
	data_to_send[0] =  adr>>8;
	data_to_send[1] =  adr%0xff;
	HAL_I2C_Master_Transmit(&IIC_Handler,AT24CXX_ADDR_WRITE,data_to_send,2,0x1000);
	HAL_I2C_Master_Receive(&IIC_Handler,AT24CXX_ADDR_READ,&data_to_receive,1,0x1000);
  return data_to_receive;

}
/*********************通信协议*/


/*工具函数*********************/

/*********************工具函数*/


/*功能函数*********************/
//在AT24CXX里面的指定地址开始写入长度为Len的数据
//该函数用于写入16bit或者32bit的数据.
//WriteAddr  :开始写入的地址
//DataToWrite:数据数组首地址
//Len        :要写入数据的长度2,4
void E2P_WriteLenByte(uint16_t WriteAddr, uint32_t DataToWrite, uint8_t Len)
{
    uint8_t t;
    for (t = 0; t < Len; t++)
    {
				E2P_write_byte(WriteAddr + t, (DataToWrite >> (8 * t)) & 0xff);
    }
}
 
//在AT24CXX里面的指定地址开始读出长度为Len的数据
//该函数用于读出16bit或者32bit的数据.
//ReadAddr   :开始读出的地址
//返回值     :数据
//Len        :要读出数据的长度2,4
uint32_t E2P_ReadLenByte(uint16_t ReadAddr, uint8_t Len)
{
    uint8_t t;
    uint32_t temp = 0;
    for (t = 0; t < Len; t++)
    {
        temp <<= 8;
        temp += E2P_read_byte(ReadAddr + Len - t - 1);
    }
    return temp;
}

//检查AT24C256是否正常
//这里用了24256的最后一个地址(0x8000)来存储标志字.
//如果用其他24C系列,这个地址要修改
//返回1:检测失败
//返回0:检测成功
uint8_t E2P_Check(void)
{
    uint8_t temp;
    temp = E2P_read_byte(0x8000); //避免每次开机都写AT24CXX
    if (temp == 0x36)return 0;
    else//排除第一次初始化的情况
    {
        E2P_write_byte(0x8000, 0x36);
        temp = E2P_read_byte(0x8000);
        if (temp == 0x36)return 0;
    }
    return 1;
}
 
//在AT24CXX里面的指定地址开始读出指定个数的数据
//ReadAddr :开始读出的地址 对24c02为0~255
//pBuffer  :数据数组首地址
//NumToRead:要读出数据的个数
void E2P_ReadBuffer(uint16_t ReadAddr, uint8_t *pBuffer, uint16_t NumToRead)
{
    while (NumToRead)
    {
        *pBuffer++ = E2P_read_byte(ReadAddr++);
        NumToRead--;
    }
}
//在AT24CXX里面的指定地址开始写入指定个数的数据
//WriteAddr :开始写入的地址 对24c02为0~255
//pBuffer   :数据数组首地址
//NumToWrite:要写入数据的个数
void E2P_WriteBuffer(uint16_t WriteAddr, uint8_t *pBuffer, uint16_t NumToWrite)
{
    while (NumToWrite--)
    {
        E2P_write_byte(WriteAddr, *pBuffer);
        WriteAddr++;
        pBuffer++;
    }
}
/********************************************************************************************************
函数名称：E2P_WriteFloatNum( uint16_t addr, storFloatData Data )
函数功能：向指定地址写入1个浮点数
详    述：
输    入：
*********************************************************************************************************/

void E2P_WriteFloatNum( uint16_t addr, storFloatData Data )
{
	uint8_t i = 0;

	for( i = 0; i < FLOAT_BYTE_NUM ; i++ )
	{
		E2P_write_byte( addr + i, Data.byte[i] );
	}
	return;
}

/********************************************************************************************************
函数名称：E2P_ReadFloatNum( uint16_t addr )
函数功能：从指定地址读出1个浮点数
详    述：
输    入：
*********************************************************************************************************/
storFloatData E2P_ReadFloatNum( uint16_t addr )
{
    uint8_t i = 0;
    storFloatData read_data;

    for( i = 0; i < FLOAT_BYTE_NUM; i++ )
    {
        read_data.byte[i] = E2P_read_byte( addr + i );
    }

    return read_data;
}
/*********************功能函数*/

//EEPROM读出所有参数
void E2P_Read_allParam(void)
{
		the_car.the_pid->pid_stand_angle->kp = E2P_ReadFloatNum(EEPROM_ADDRESS_BASE+0x04*0).value;
		the_car.the_pid->pid_stand_angle->ki = E2P_ReadFloatNum(EEPROM_ADDRESS_BASE+0x04*1).value;
		the_car.the_pid->pid_stand_angle->kd = E2P_ReadFloatNum(EEPROM_ADDRESS_BASE+0x04*2).value;
		the_car.the_pid->pid_stand_angle_speed->kp = E2P_ReadFloatNum(EEPROM_ADDRESS_BASE+0x04*3).value;
		the_car.the_pid->pid_stand_angle_speed->ki = E2P_ReadFloatNum(EEPROM_ADDRESS_BASE+0x04*4).value;
		the_car.the_pid->pid_stand_angle_speed->kd = E2P_ReadFloatNum(EEPROM_ADDRESS_BASE+0x04*5).value;
		the_car.the_pid->pid_target_speed->kp = E2P_ReadFloatNum(EEPROM_ADDRESS_BASE+0x04*6).value;
		the_car.the_pid->pid_target_speed->ki = E2P_ReadFloatNum(EEPROM_ADDRESS_BASE+0x04*7).value;
		the_car.the_pid->pid_target_speed->kd = E2P_ReadFloatNum(EEPROM_ADDRESS_BASE+0x04*8).value;
		the_car.the_pid->pid_turn_position->kp = E2P_ReadFloatNum(EEPROM_ADDRESS_BASE+0x04*9).value;
		the_car.the_pid->pid_turn_position->ki = E2P_ReadFloatNum(EEPROM_ADDRESS_BASE+0x04*10).value;
		the_car.the_pid->pid_turn_position->kd = E2P_ReadFloatNum(EEPROM_ADDRESS_BASE+0x04*11).value;
		the_car.the_pid->pid_turn_gyro->kp = E2P_ReadFloatNum(EEPROM_ADDRESS_BASE+0x04*12).value;
		the_car.the_pid->pid_turn_gyro->ki = E2P_ReadFloatNum(EEPROM_ADDRESS_BASE+0x04*13).value;
		the_car.the_pid->pid_turn_gyro->kd = E2P_ReadFloatNum(EEPROM_ADDRESS_BASE+0x04*14).value;	
		the_car.vs->pos_err_offset = E2P_ReadLenByte(EEPROM_ADDRESS_BASE+0x04*15, 4);
		the_car.run_speed = E2P_ReadFloatNum(EEPROM_ADDRESS_BASE+0x04*16).value;
		
}
