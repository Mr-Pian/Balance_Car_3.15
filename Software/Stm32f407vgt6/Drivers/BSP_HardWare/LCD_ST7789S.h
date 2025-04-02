#ifndef __THT18_ST7735S_H__
#define __THT18_ST7735S_H__

#include "main.h"
#include "stdint.h"
#include "spi.h"
#include "stdlib.h"
#include "stdio.h"
#include <stdarg.h>
#include <string.h>
/*******************�ӿڶ���******************************/
/***TFT SPIģ��ܽŶ���***/
#define TFTSPI_CS     LCD_CS_GPIO_Port,LCD_CS_Pin     // CS�ܽ� Ĭ�����ͣ����Բ���
#define TFTSPI_SCK    GPIOC,GPIO_PIN_10     // SPI SCK�ܽ�
#define TFTSPI_SDI    GPIOC,GPIO_PIN_12     // SPI MOSI�ܽ�
#define TFTSPI_DC     LCD_DC_GPIO_Port,LCD_DC_Pin     // D/C�ܽ�
#define TFTSPI_RST    LCD_RST_GPIO_Port,LCD_RST_Pin     // RESET�ܽ�

#define TFTSPI_CS_H     HAL_GPIO_WritePin(TFTSPI_CS,GPIO_PIN_SET);      /* CS�ܽ� */
#define TFTSPI_SCK_H    HAL_GPIO_WritePin(TFTSPI_SCK,GPIO_PIN_SET);     /* SCL�ܽ� */
#define TFTSPI_SDI_H    HAL_GPIO_WritePin(TFTSPI_SDI,GPIO_PIN_SET);     /* SDI�ܽ� */
#define TFTSPI_DC_H     HAL_GPIO_WritePin(TFTSPI_DC,GPIO_PIN_SET);      /* DC�ܽ� */
#define TFTSPI_RST_H    HAL_GPIO_WritePin(TFTSPI_RST,GPIO_PIN_SET);     /* RST�ܽ� */

#define TFTSPI_CS_L     HAL_GPIO_WritePin(TFTSPI_CS,GPIO_PIN_RESET);     /* CS�ܽ�(Ĭ�����ͣ���������) */
#define TFTSPI_SCK_L    HAL_GPIO_WritePin(TFTSPI_SCK,GPIO_PIN_RESET);    /* SCL�ܽ� */
#define TFTSPI_SDI_L    HAL_GPIO_WritePin(TFTSPI_SDI,GPIO_PIN_RESET);    /* SDI�ܽ� */
#define TFTSPI_DC_L     HAL_GPIO_WritePin(TFTSPI_DC,GPIO_PIN_RESET);     /* DC�ܽ� */
#define TFTSPI_RST_L    HAL_GPIO_WritePin(TFTSPI_RST,GPIO_PIN_RESET);    /* RST�ܽ� */

/* HAL�� SPI�ӿڶ���*/
#define SPI_Handler  hspi3

#define TFT18W        240 //TFT��Ļ�����ظ���
#define TFT18H        240 //TFT��Ļ�����ظ���

#define USE_SPI_DMA   0 //ʹ��DMA����
#define USE_EX_GRAM   0  //ʹ���ⲿ�Դ�
#define USE_HORIZONTAL 2//���ú�������������ʾ
												//0:����������
												//1:��������ת��
												//2:����������
												//3:��������ת��
												
/***�����Сѡ��***/
enum Font_size
{
	fsize_6X8 = 0,
	fsize_8X16,
	fsize_12X24,
	fsize_16X32
};
/***ͼƬ��ɫ����ѡ��***/
enum Image_color
{
	img_gray = 0,
	img_rgb565
};
/****************RGB565��ɫ����*****************/
/* 16-bit RGB565��ɫ�궨�壨��ʽ��R:5λ G:6λ B:5λ�� */
#define COLOR_BLACK     0x0000   // ��ɫ
#define COLOR_WHITE     0xFFFF   // ��ɫ
#define COLOR_RED       0xF800   // ��ɫ
#define COLOR_LIME      0x07E0   // �̣�����HTML limeɫ��
#define COLOR_BLUE      0x001F   // ��ɫ
#define COLOR_YELLOW    0xFFE0   // ��ɫ
#define COLOR_CYAN      0x07FF   // ��ɫ
#define COLOR_MAGENTA   0xF81F   // Ʒ��
#define COLOR_SILVER    0xB596   // ����ɫ
#define COLOR_GRAY      0x7BEF   // ��׼��ɫ
#define COLOR_MAROON    0x8000   // ��ɫ�����죩
#define COLOR_OLIVE     0x8400   // ����̣����ƣ�
#define COLOR_GREEN     0x03E0   // ����ɫ��HTML��׼��
#define COLOR_PURPLE    0x8010   // ��ɫ��������ϣ�
#define COLOR_TEAL      0x0438   // ˮѼɫ�����ࣩ
#define COLOR_NAVY      0x0010   // ����ɫ��������
#define COLOR_ORANGE    0xFD20   // ��ɫ
#define COLOR_PINK      0xF8B2   // �ۺ�ɫ
#define COLOR_BROWN     0x8A22   // ��ɫ
#define COLOR_DARKGRAY  0x4208   // ���ɫ
#define COLOR_LIGHTGRAY 0xC618   // ǳ��ɫ
#define ST7735_COLOR565(r, g, b) (((r & 0x00F8) << 8) | ((-g & 0x00FC) << 3) | ((b & 0x00F8) >> 3))

/*********TFT��ĻSPI��������**********/
#if USE_EX_GRAM
extern uint16_t ex_gram[TFT18W*TFT18H];
#endif
void TFTSPI_Init(void);

void TFTSPI_Write_Cmd(uint8_t cmd);

void TFTSPI_Write_Byte(uint8_t dat);

void TFTSPI_Write_Word(unsigned short dat);
void TFT_DMA_sendGRAM(uint16_t* gram);
void TFT_sendGRAM(uint16_t* gram);
/*********TFT��ĻSPI��������*********/

/*********TFT��Ļͼ�λ�������********/
void TFTSPI_Set_Pos(unsigned short xs,unsigned short ys,unsigned short xe,unsigned short ye);

void TFTSPI_Fill_Area(unsigned short xs,unsigned short ys,unsigned short xe,unsigned short ye,unsigned short color);

void TFTSPI_CLS(unsigned short color);

void TFTSPI_Draw_Part(unsigned short xs,unsigned short ys,unsigned short xe,unsigned short ye,unsigned short color_dat);

void TFTSPI_Draw_Rectangle(unsigned short xs,unsigned short ys,unsigned short xe,unsigned short ye,unsigned short color_dat);

void TFTSPI_Draw_Circle(unsigned short x,unsigned short y,unsigned short r,unsigned short color_dat);

void TFTSPI_Draw_Line(unsigned short xs,unsigned short ys,unsigned short xe,unsigned short ye,unsigned short color_dat);

void TFTSPI_Draw_Dot(unsigned short x,unsigned short y,unsigned short color_dat);
/*********TFT��Ļͼ�λ�������********/

/*********TFT��Ļ�ַ���ӡ************/
void TFT_ShowChar(uint8_t X, uint8_t Y, char Char,unsigned short word_color, unsigned short back_color, enum Font_size Size);
void TFT_ShowString(uint8_t X, uint8_t Y, char *String,unsigned short word_color, unsigned short back_color,enum Font_size Size);
void TFT_Printf(uint8_t X, uint8_t Y,unsigned short word_color, unsigned short back_color ,enum Font_size Size, char *format, ...);
/*********TFT��ĻͼƬ��ʾ************/
void TFT_ShowPicture(uint8_t x,uint8_t y,uint8_t length,uint8_t width,const uint8_t pic[]);
/*********TFT��ĻͼƬ��ʾ************/
#endif
