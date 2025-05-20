#ifndef __EEPROM_H
#define __EEPROM_H
/******************
AT24C256 iic����
******************/
#include "main.h"
/*���IIC IO�ڶ���*/
#define E2P_IIC_SDA_PIN P37
#define E2P_IIC_SCL_PIN P36

#define E2P_IIC_SDA(x)              (E2P_IIC_SDA_PIN = x)
#define E2P_IIC_SCL(x)              (E2P_IIC_SCL_PIN = x)

/*HAL�� IIC �ӿڶ�����*/
#define IIC_Handler hi2c1
 
#define AT24CXX_ADDR_WRITE  0xA0    // д����
#define AT24CXX_ADDR_READ   0xA1    // ������
#define FLOAT_BYTE_NUM 4
/*********************�����궨��*/
//�������洢������
typedef union
{
    float value;
    uint8_t byte[FLOAT_BYTE_NUM];
} storFloatData;

/*��������*********************/
void E2P_write_byte(uint16_t adr,unsigned char Data);
unsigned char E2P_read_byte(uint16_t adr);

void E2P_WriteLenByte(uint16_t WriteAddr, uint32_t DataToWrite, uint8_t Len);
uint32_t E2P_ReadLenByte(uint16_t ReadAddr, uint8_t Len);
uint8_t E2P_Check(void);
void E2P_ReadBuffer(uint16_t ReadAddr, uint8_t *pBuffer, uint16_t NumToRead);
void E2P_WriteBuffer(uint16_t WriteAddr, uint8_t *pBuffer, uint16_t NumToWrite);
void E2P_WriteBuffer(uint16_t WriteAddr, uint8_t *pBuffer, uint16_t NumToWrite);
void E2P_WriteFloatNum( uint16_t addr, storFloatData Data );
storFloatData E2P_ReadFloatNum( uint16_t addr );
void E2P_Read_allParam(void);
void E2P_Write_allParam(void);
/*********************��������*/




#endif
