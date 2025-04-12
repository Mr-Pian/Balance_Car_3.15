#include "main.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "EEPROM.h"
#include "delay.h"
#include "i2c.h"
#include "control.h"
#include "UI.h"
/*��������*********************/

/**
  * ��    ����E2PROMдSCL�ߵ͵�ƽ
  * ��    ����Ҫд��SCL�ĵ�ƽֵ����Χ��0/1
  * �� �� ֵ����
  * ˵    �������ϲ㺯����ҪдSCLʱ���˺����ᱻ����
  *           �û���Ҫ���ݲ��������ֵ����SCL��Ϊ�ߵ�ƽ���ߵ͵�ƽ
  *           ����������0ʱ����SCLΪ�͵�ƽ������������1ʱ����SCLΪ�ߵ�ƽ
  */
//void E2P_W_SCL(uint8_t BitValue)
//{
//	/*����BitValue��ֵ����SCL�øߵ�ƽ���ߵ͵�ƽ*/
//	
//	if(BitValue==0x00) E2P_IIC_SCL(0);
//	else  E2P_IIC_SCL(1);
//	
//	/*�����Ƭ���ٶȹ��죬���ڴ����������ʱ���Ա��ⳬ��I2Cͨ�ŵ�����ٶ�*/
//	//...
//}

///**
//  * ��    ����E2PROMдSDA�ߵ͵�ƽ
//  * ��    ����Ҫд��SDA�ĵ�ƽֵ����Χ��0/1
//  * �� �� ֵ����
//  * ˵    �������ϲ㺯����ҪдSDAʱ���˺����ᱻ����
//  *           �û���Ҫ���ݲ��������ֵ����SDA��Ϊ�ߵ�ƽ���ߵ͵�ƽ
//  *           ����������0ʱ����SDAΪ�͵�ƽ������������1ʱ����SDAΪ�ߵ�ƽ
//  */
//void E2P_W_SDA(uint8_t BitValue)
//{
//	/*����BitValue��ֵ����SDA�øߵ�ƽ���ߵ͵�ƽ*/
//	if(BitValue==0x00) E2P_IIC_SDA(0);
//	else E2P_IIC_SDA(1);
//	/*�����Ƭ���ٶȹ��죬���ڴ����������ʱ���Ա��ⳬ��I2Cͨ�ŵ�����ٶ�*/
//	//...
//}

/*********************��������*/


/*ͨ��Э��*********************/

///**
//  * ��    ����I2C��ʼ
//  * ��    ������
//  * �� �� ֵ����
//  */
//void E2P_I2C_Start(void)
//{
//	E2P_W_SDA(1);		//�ͷ�SDA��ȷ��SDAΪ�ߵ�ƽ
//	E2P_W_SCL(1);		//�ͷ�SCL��ȷ��SCLΪ�ߵ�ƽ
//	delay_us(4);
//	E2P_W_SDA(0);		//��SCL�ߵ�ƽ�ڼ䣬����SDA��������ʼ�ź�
//	delay_us(4);
//	E2P_W_SCL(0);		//��ʼ���SCLҲ���ͣ���Ϊ��ռ�����ߣ�ҲΪ�˷�������ʱ���ƴ��
//}
///**
//  * ��    ����I2C��ֹ
//  * ��    ������
//  * �� �� ֵ����
//  */
//void E2P_I2C_Stop(void)
//{
//	E2P_W_SCL(0);		//�ͷ�SCL��ʹSCL���ֵ͵�ƽ
//	E2P_W_SDA(0);		//����SDA��ȷ��SDAΪ�͵�ƽ
//	delay_us(4);
//	E2P_W_SCL(1);		//�ͷ�SCL��ʹSCL���ָߵ�ƽ
//	E2P_W_SDA(1);		//��SCL�ߵ�ƽ�ڼ䣬�ͷ�SDA��������ֹ�ź�
//	delay_us(4);
//}

/*************************************************************/
/*                      IIC��ȡӦ��λ                        */
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
///*                      IIC����Ӧ��λ                        */
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
///*                      IIC������Ӧ��λ                      */
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
//  * ��    ����I2C����һ���ֽ�
//  * ��    ����Byte Ҫ���͵�һ���ֽ����ݣ���Χ��0x00~0xFF
//  * �� �� ֵ����
//  */
//void E2P_I2C_SendByte(uint8_t Byte)
//{
//	uint8_t i;
//  E2P_W_SCL(0);//����ʱ�ӿ�ʼ���ݴ���	
//	/*ѭ��8�Σ��������η������ݵ�ÿһλ*/
//	for (i = 0; i < 8; i++)
//	{
//		if((Byte & 0x80)>>7)//��λ����
//			E2P_W_SDA(1);
//		else 
//			E2P_W_SDA(0);
//		Byte<<=1;//�ѵ�λ���Ƶ���λ����
//		delay_us(2);
//		E2P_W_SCL(1);	//�ͷ�SCL���ӻ���SCL�ߵ�ƽ�ڼ��ȡSDA
//		delay_us(2);
//		E2P_W_SCL(0);	//����SCL��������ʼ������һλ����
//		delay_us(2);
//	}
//	
//}
///**
//  * ��    ����I2C����һ���ֽ�
//  * ��    ����
//  * �� �� ֵ����
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
/*             ��AT24C02д��һ���ֽڵ�����                   */
/*          adrΪд��ĵ�ַ��DataΪд�������                */
/*************************************************************/
void E2P_write_byte(uint16_t adr,unsigned char Data)
{
//  E2P_I2C_Start();
//  E2P_I2C_SendByte(AT24CXX_ADDR_WRITE);
//  E2P_I2C_readack();

//  E2P_I2C_SendByte(adr>>8);//���͸ߵ�ַ
//  E2P_I2C_readack();

//  E2P_I2C_SendByte(adr%0xff);//���͵͵�ַ
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
/*             ��AT24C02��ȡһ���ֽڵ�����                   */
/*          adrΪ��ȡ�ĵ�ַ������ֵΪ��ȡ������              */
/*************************************************************/
uint8_t E2P_read_byte(uint16_t adr)
{
//	uint8_t readdata;
//  E2P_I2C_Start();
//  E2P_I2C_SendByte(AT24CXX_ADDR_WRITE);
//  E2P_I2C_readack();

//  E2P_I2C_SendByte(adr>>8);//���͸ߵ�ַ
//  E2P_I2C_readack();

//  E2P_I2C_SendByte(adr%0xff);//���͵͵�ַ
//  E2P_I2C_readack();

//  E2P_I2C_Start();
//  E2P_I2C_SendByte(AT24CXX_ADDR_READ);//׼������
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
/*********************ͨ��Э��*/


/*���ߺ���*********************/

/*********************���ߺ���*/


/*���ܺ���*********************/
//��AT24CXX�����ָ����ַ��ʼд�볤��ΪLen������
//�ú�������д��16bit����32bit������.
//WriteAddr  :��ʼд��ĵ�ַ
//DataToWrite:���������׵�ַ
//Len        :Ҫд�����ݵĳ���2,4
void E2P_WriteLenByte(uint16_t WriteAddr, uint32_t DataToWrite, uint8_t Len)
{
    uint8_t t;
    for (t = 0; t < Len; t++)
    {
				E2P_write_byte(WriteAddr + t, (DataToWrite >> (8 * t)) & 0xff);
    }
}
 
//��AT24CXX�����ָ����ַ��ʼ��������ΪLen������
//�ú������ڶ���16bit����32bit������.
//ReadAddr   :��ʼ�����ĵ�ַ
//����ֵ     :����
//Len        :Ҫ�������ݵĳ���2,4
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

//���AT24C256�Ƿ�����
//��������24256�����һ����ַ(0x8000)���洢��־��.
//���������24Cϵ��,�����ַҪ�޸�
//����1:���ʧ��
//����0:���ɹ�
uint8_t E2P_Check(void)
{
    uint8_t temp;
    temp = E2P_read_byte(0x8000); //����ÿ�ο�����дAT24CXX
    if (temp == 0x36)return 0;
    else//�ų���һ�γ�ʼ�������
    {
        E2P_write_byte(0x8000, 0x36);
        temp = E2P_read_byte(0x8000);
        if (temp == 0x36)return 0;
    }
    return 1;
}
 
//��AT24CXX�����ָ����ַ��ʼ����ָ������������
//ReadAddr :��ʼ�����ĵ�ַ ��24c02Ϊ0~255
//pBuffer  :���������׵�ַ
//NumToRead:Ҫ�������ݵĸ���
void E2P_ReadBuffer(uint16_t ReadAddr, uint8_t *pBuffer, uint16_t NumToRead)
{
    while (NumToRead)
    {
        *pBuffer++ = E2P_read_byte(ReadAddr++);
        NumToRead--;
    }
}
//��AT24CXX�����ָ����ַ��ʼд��ָ������������
//WriteAddr :��ʼд��ĵ�ַ ��24c02Ϊ0~255
//pBuffer   :���������׵�ַ
//NumToWrite:Ҫд�����ݵĸ���
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
�������ƣ�E2P_WriteFloatNum( uint16_t addr, storFloatData Data )
�������ܣ���ָ����ַд��1��������
��    ����
��    �룺
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
�������ƣ�E2P_ReadFloatNum( uint16_t addr )
�������ܣ���ָ����ַ����1��������
��    ����
��    �룺
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
/*********************���ܺ���*/

//EEPROM�������в���
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
