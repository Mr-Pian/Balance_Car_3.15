/**modify--by JLB****/

#include "LCD_ST7789S.h"
#include "delay.h"
#include "Font.h"


#if USE_EX_GRAM  //ʹ���ⲿ�Դ�
	
uint16_t ex_gram[TFT18W*TFT18H] = {0};
	
#else

#endif

#define USE_QSPI 1  //ʹ��Ӳ��SPI

void TFTSPI_Inversion(uint8_t enable)
{
	if(enable)
	{
		TFTSPI_Write_Cmd(0x21);     //������ʾ��ת 
	}
	else
	{
		TFTSPI_Write_Cmd(0x20);     //�ر���ʾ��ת 
	}
}
/*!
 * @brief    TFT18��ʼ��
 *
 * @param    type �� 0:����  1������
 *
 * @return   ��
 *
 * @note     ����޸Ĺܽ� ��Ҫ�޸ĳ�ʼ���Ĺܽ�
 *
 * @see      TFTSPI_Init(1);
 *
 * @date     2019/6/13 ������
 */
void TFTSPI_Init(void) {
    //-----�˿ڳ�ʼ��----//
		delay_ms(100);
//    TFTSPI_RST_L
    
    delay_ms(20);
//    TFTSPI_RST_H
    
    delay_ms(10);
    TFTSPI_Write_Cmd(0x11);                   //�ر�˯��
    delay_ms(10);
    TFTSPI_Write_Cmd(0x3a);     //�������ظ�ʽ  
    TFTSPI_Write_Byte(0x05);		//ÿ�δ���16λ����(VIPF3-0=0101)��ÿ������16λ(IFPF2-01)
		TFTSPI_Write_Cmd(0xB2);			
		TFTSPI_Write_Byte(0x0C);
		TFTSPI_Write_Byte(0x0C); 
		TFTSPI_Write_Byte(0x00); 
		TFTSPI_Write_Byte(0x33); 
		TFTSPI_Write_Byte(0x33); 			

		TFTSPI_Write_Cmd(0xB7);			
		TFTSPI_Write_Byte(0x35);

		TFTSPI_Write_Cmd(0xBB);			
		TFTSPI_Write_Byte(0x32); //Vcom=1.35V
						
		TFTSPI_Write_Cmd(0xC2);
		TFTSPI_Write_Byte(0x01);

		TFTSPI_Write_Cmd(0xC3);			
		TFTSPI_Write_Byte(0x15); //GVDD=4.8V  ��ɫ���
					
		TFTSPI_Write_Cmd(0xC4);			
		TFTSPI_Write_Byte(0x20); //VDV, 0x20:0v

		TFTSPI_Write_Cmd(0xC6);			
		TFTSPI_Write_Byte(0x0F); //0x0F:60Hz        	

		TFTSPI_Write_Cmd(0xD0);			
		TFTSPI_Write_Byte(0xA4);
		TFTSPI_Write_Byte(0xA1); 

		TFTSPI_Write_Cmd(0xE0);
		TFTSPI_Write_Byte(0xD0);   
		TFTSPI_Write_Byte(0x08);   
		TFTSPI_Write_Byte(0x0E);   
		TFTSPI_Write_Byte(0x09);   
		TFTSPI_Write_Byte(0x09);   
		TFTSPI_Write_Byte(0x05);   
		TFTSPI_Write_Byte(0x31);   
		TFTSPI_Write_Byte(0x33);   
		TFTSPI_Write_Byte(0x48);   
		TFTSPI_Write_Byte(0x17);   
		TFTSPI_Write_Byte(0x14);   
		TFTSPI_Write_Byte(0x15);   
		TFTSPI_Write_Byte(0x31);   
		TFTSPI_Write_Byte(0x34);   

		TFTSPI_Write_Cmd(0xE1);     
		TFTSPI_Write_Byte(0xD0);   
		TFTSPI_Write_Byte(0x08);   
		TFTSPI_Write_Byte(0x0E);   
		TFTSPI_Write_Byte(0x09);   
		TFTSPI_Write_Byte(0x09);   
		TFTSPI_Write_Byte(0x15);   
		TFTSPI_Write_Byte(0x31);   
		TFTSPI_Write_Byte(0x33);   
		TFTSPI_Write_Byte(0x48);   
		TFTSPI_Write_Byte(0x17);   
		TFTSPI_Write_Byte(0x14);   
		TFTSPI_Write_Byte(0x15);   
		TFTSPI_Write_Byte(0x31);   
		TFTSPI_Write_Byte(0x34);

    TFTSPI_Write_Cmd(0x36);       //������ʾ������ɫ��ʽ��RGB/BRG��
    
    switch(USE_HORIZONTAL)
    {
        case 0:TFTSPI_Write_Byte(0xC0);
        break;
        case 1:TFTSPI_Write_Byte(0x00);
        break;
        case 2:TFTSPI_Write_Byte(0x70);
        break;
        case 3:TFTSPI_Write_Byte(0xA0);
        break;
        
    }
		
	//TFTSPI_Inversion(1);
	TFTSPI_Write_Cmd(0x21);              //��ɫ��ת
	TFTSPI_Write_Cmd(0x29);              //������Ļ��ʾ

	TFTSPI_CLS(COLOR_RED);//��ʼΪȫ��
}

/*!
 * @brief    ����ʱ����
 *
 * @param    Del ����ʱʱ��
 *
 * @return   ��
 *
 * @note     �ڲ�����
 *
 * @see      tft18delay_1us(1);
 *
 * @date     2019/6/13 ������
 */
void tft18delay_1us(unsigned int Del)       //
{
	delay_us(Del);
}

#if USE_QSPI
/*!
 * @brief    д����
 *
 * @param    cmd ������
 *
 * @return   ��
 *
 * @note     �ڲ�����
 *
 * @see      TFTSPI_Write_Cmd(0xb7); //LCD Driveing control
 *
 * @date     2019/6/13 ������
 */
void TFTSPI_Write_Cmd(uint8_t cmd)
{
  TFTSPI_DC_L;       //DC=0��������
	TFTSPI_CS_L;//Ƭѡ
#if USE_SPI_DMA
		HAL_SPI_Transmit_DMA(&SPI_Handler,&cmd, 1);
#else		
		HAL_SPI_Transmit(&SPI_Handler,&cmd, 1, 0xffff);//��������
#endif
	
	

}

/*!
 * @brief    д�ֽ�
 *
 * @param    dat ������
 *
 * @return   ��
 *
 * @note     �ڲ�����
 *
 * @see      TFTSPI_Write_Byte(0x00);   
 *
 * @date     2019/6/13 ������
 */
void TFTSPI_Write_Byte(uint8_t dat)
{

  TFTSPI_DC_H;       //DC=1��������
	TFTSPI_CS_L;//Ƭѡ
#if USE_SPI_DMA
			HAL_SPI_Transmit_DMA(&SPI_Handler,&dat, 1);
#else		
		HAL_SPI_Transmit(&SPI_Handler,&dat, 1, 0xffff);//��������
#endif
	
}

/*!
 * @brief    д����
 *
 * @param    dat ������
 *
 * @return   ��
 *
 * @note     ��
 *
 * @see      TFTSPI_Write_Word(0xFFFF);
 *
 * @date     2019/6/13 ������
 */
void TFTSPI_Write_Word(unsigned short dat)
{
		uint8_t Data[2] = {0,0};
		TFTSPI_CS_L;//Ƭѡ
    TFTSPI_DC_H;       //DC=1��������
		Data[0] = dat>>8;
		Data[1] = (uint8_t)dat;
#if USE_SPI_DMA
			HAL_SPI_Transmit_DMA(&SPI_Handler,Data, 2);
#else		
		HAL_SPI_Transmit(&SPI_Handler,Data, 2, 0xffff);//��������
#endif
}

#else

/*!
 * @brief    д����
 *
 * @param    cmd ������
 *
 * @return   ��
 *
 * @note     �ڲ�����
 *
 * @see      TFTSPI_Write_Cmd(0xb7); //LCD Driveing control
 *
 * @date     2019/6/13 ������
 */
void TFTSPI_Write_Cmd(uint8_t cmd) {
    unsigned short i;
    TFTSPI_DC_L;       //DC=0��������
		
    for (i = 0; i < 8; i++) {
        TFTSPI_SCK_L;
        tft18delay_1us(1);    // SCK=0  ILI9163_SCK=0;
        if (cmd & 0x80) {
            TFTSPI_SDI_H;
        }       // SDI=1
        else {
            TFTSPI_SDI_L;
        }       // SDI=0
        TFTSPI_SCK_H;
        tft18delay_1us(1);    // SCK=1  ILI9163_SCK = 1;
        cmd = (cmd << 1);
    }
}

/*!
 * @brief    д�ֽ�
 *
 * @param    dat ������
 *
 * @return   ��
 *
 * @note     �ڲ�����
 *
 * @see      TFTSPI_Write_Byte(0x00);    //CRL=0
 *
 * @date     2019/6/13 ������
 */
void TFTSPI_Write_Byte(uint8_t dat) {
    unsigned short i;
    TFTSPI_DC_H;       //DC=1��������
    for (i = 0; i < 8; i++) {
        TFTSPI_SCK_L;
        tft18delay_1us(1);    // SCK=0  ILI9163_SCK=0;
        if (dat & 0x80) {
            TFTSPI_SDI_H;
        }       // SDI=1
        else {
            TFTSPI_SDI_L;
        }       // SDI=0
        TFTSPI_SCK_H;
        tft18delay_1us(1);    // SCK=1 ILI9163_SCK = 1;
        dat = (dat << 1);
    }
}

/*!
 * @brief    д����
 *
 * @param    dat ������
 *
 * @return   ��
 *
 * @note     ��
 *
 * @see      TFTSPI_Write_Word(0xFFFF);
 *
 * @date     2019/6/13 ������
 */
void TFTSPI_Write_Word(unsigned short dat) {
    unsigned short i;

    TFTSPI_DC_H;       //DC=1��������

    for (i = 0; i < 16; i++) {
        TFTSPI_SCK_L;
			tft18delay_1us(1);  // SCK=0  ILI9163_SCK=0;
        if (dat & 0x8000) {
            TFTSPI_SDI_H;
        }       // SDI=1
        else {
            TFTSPI_SDI_L;
        }       // SDI=0
        TFTSPI_SCK_H;
          tft18delay_1us(1);  // SCK=1  ILI9163_SCK=1;
        dat <<= 1;
    }
}

#endif

/*!
 * @brief    ���¶�λ������Ϣλ��
 *
 * @param    xs ����ʼx
 * @param    ys ����ʼy
 * @param    xe ������x
 * @param    ys ������y
 *
 * @return   ��
 *
 * @note     �ڲ�����
 *
 * @see      TFTSPI_Set_Pos(10, 20, 30, 40);
 *
 * @date     2019/6/13 ������
 */
void TFTSPI_Set_Pos(unsigned short xs, unsigned short ys, unsigned short xe,
        unsigned short ye) {
    TFTSPI_Write_Cmd(0x2A);       //Colulm addRSTs set
    TFTSPI_Write_Byte(0x00);      //����ʼ�����8λ��ʼ��Ϊ��
    TFTSPI_Write_Byte(xs);            //����ʼ�����8λ
    TFTSPI_Write_Byte(0x00);      //����ֹ�����8λ��ʼ��Ϊ��
    TFTSPI_Write_Byte(xe);            //����ֹ�����8λ
    TFTSPI_Write_Cmd(0x2B);       //Colulm addRSTs set
    TFTSPI_Write_Byte(0x00);      //����ʼ�����8λ��ʼ��Ϊ��
    TFTSPI_Write_Byte(ys);            //����ʼ�����8λ
    TFTSPI_Write_Byte(0x00);      //����ֹ�����8λ��ʼ��Ϊ��
    TFTSPI_Write_Byte(ye);            //����ֹ�����8λ
    TFTSPI_Write_Cmd(0x2C);       //GRAM����MCU���ݻ�����
}


/*!
 * @brief    ���õ�ַ
 *
 * @param    ��
 *
 * @return   ��
 *
 * @note     �ڲ�����
 *
 * @see      TFTSPI_Addr_Rst();
 *
 * @date     2019/6/13 ������
 */
void TFTSPI_Addr_Rst(void) {
    TFTSPI_Write_Cmd(0x2a);     //����MCU�ɲ�����LCD�ڲ�RAM��������ʼ����������
    TFTSPI_Write_Byte(0x00);       //��������ʼ��ַ0x0000
    TFTSPI_Write_Byte(0x00);
    TFTSPI_Write_Byte(0x00);       //�����������ַ0x007f(240-1)
    TFTSPI_Write_Byte(0xEF);       //EF
    TFTSPI_Write_Cmd(0x2b);     //����MCU�ɲ�����LCD�ڲ�RAM��������ʼ��������
    TFTSPI_Write_Byte(0x00);       //��������ʼ��ַ0x0000
    TFTSPI_Write_Byte(0x00);
    TFTSPI_Write_Byte(0x00);       //�����������ַ0x009f(240-1)
    TFTSPI_Write_Byte(0xEF);       //Ef
    TFTSPI_Write_Cmd(0x2C);     //GRAM����MCU���ݻ�����
}

void TFT_sendGRAM_DMA(uint16_t* gram)
{
	TFTSPI_Addr_Rst();
	 TFTSPI_DC_H;       //DC=1��������
	//HAL_SPI_Transmit_DMA(&SPI_Handler,(uint8_t*)gram,TFT18H*TFT18W*2);
}
void TFT_sendGRAM(uint16_t* gram)
{
	TFTSPI_Addr_Rst();
	 TFTSPI_DC_H;       //DC=1��������
	//HAL_SPI_Transmit(&SPI_Handler,(uint8_t*)gram,TFT18H*TFT18W*2,0xffff);
}

/*!
 * @brief    ���ָ������
 *
 * @param    xs ����ʼx
 * @param    ys ����ʼy
 * @param    xe ������x
 * @param    ys ������y
 * @param    color ��������ɫ
 *
 * @return   ��
 *
 * @note     ��ʼ����ֹ������(0-127)��������(0-159),��ʾ��ɫuint16
 *
 * @see      TFTSPI_Fill_Area(10, 20, 30, 40, u16YELLOW);
 *
 * @date     2019/6/13 ������
 */
void TFTSPI_Fill_Area(unsigned short xs, unsigned short ys, unsigned short xe,
        unsigned short ye, unsigned short color) {
    unsigned int i, j;
	#if USE_EX_GRAM
			for (i = ys; i < ye; i++) {
					for (j = xs ;j < xe; j++) {
							ex_gram[i*TFT18W + j] = color;
					}
			}
	#else 
			TFTSPI_Set_Pos(xs, ys, xe, ye);
			for (i = 0; i < (xe - xs); i++) {
					for (j = 0; j < (ye - ys); j++) {
							TFTSPI_Write_Word(color);
					}
			}
	#endif

}

/*!
 * @brief    ȫ����ʾ��ɫ����
 *
 * @param    color ��������ɫ
 *
 * @return   ��
 *
 * @note     ��ʼ����ֹ������(0-127)��������(0-159),��ʾ��ɫuint16
 *
 * @see      TFTSPI_CLS(u16YELLOW);
 *
 * @date     2019/6/13 ������
 */
void TFTSPI_CLS(unsigned short color) {
    uint32_t i, j;
	#if USE_EX_GRAM
			for (i = 0; i < TFT18H; i++) {
					for (j = 0 ;j < TFT18W; j++) {
							ex_gram[i*TFT18W + j] = color;
					}
			}
	#else 
    TFTSPI_Addr_Rst();
		uint8_t Data[2] = {0,0};
    TFTSPI_DC_H;       //DC=1��������
		Data[0] = color >>8;
		Data[1] = (uint8_t)color;
	for(i = 0;i<TFT18W*TFT18H;i++)
		HAL_SPI_Transmit(&SPI_Handler,Data, 2, 0xffff);//��������
		
//    for (i = 0; i < TFT18W; i++)      //160
//            {
//        for (j = 0; j < TFT18H; j++) {
//            TFTSPI_Write_Word(color);
//        }
//    }
		
	#endif

}

/*!
 * @brief    ����������
 *
 * @param    xs ����ʼx
 * @param    ys ����ʼy
 * @param    xe ������x
 * @param    ys ������y
 * @param    color_dat ��������ɫ
 *
 * @return   ��
 *
 * @note     ��ʼ����ֹ������(0-127)��������(0-159),��ʾ��ɫuint16
 *
 * @see      TFTSPI_Draw_Part(10, 20, 30, 40, u16YELLOW);
 *
 * @date     2019/6/13 ������
 */
void TFTSPI_Draw_Part(unsigned short xs, unsigned short ys, unsigned short xe,
        unsigned short ye, unsigned short color_dat) {
    unsigned short i, j;
	#if USE_EX_GRAM
    for (j = ys; j < (ye + 1); j++) {
        for (i = xs; i < (xe + 1); i++) {
            ex_gram[j*TFT18W + i] = color_dat;
        }
    }
	#else 
    TFTSPI_Set_Pos(xs, ys, xe, ye);
		uint8_t Data[2] = {0,0};
    TFTSPI_DC_H;       //DC=1��������
		Data[0] = color_dat >>8;
		Data[1] = (uint8_t)color_dat;
	for(i = 0;i<(ye - ys + 1)*(xe - xs + 1);i++)
		HAL_SPI_Transmit(&SPI_Handler,Data, 2, 0xffff);//��������					
//    for (j = 0; j < (ye - ys + 1); j++) {
//        for (i = 0; i < (xe - xs + 1); i++) {
//            TFTSPI_Write_Word(color_dat);
//        }
//    }
	#endif

}

/*!
 * @brief    �����α߿�
 *
 * @param    xs ����ʼx
 * @param    ys ����ʼy
 * @param    xe ������x
 * @param    ys ������y
 * @param    color_dat ����ɫ
 *
 * @return   ��
 *
 * @note     ��ʼ����ֹ������(0-127)��������(0-159),��ʾ��ɫuint16
 *
 * @see      TFTSPI_Draw_Rectangle(10, 20, 30, 40, u16YELLOW);
 *
 * @date     2019/6/13 ������
 */
void TFTSPI_Draw_Rectangle(unsigned short xs, unsigned short ys,
        unsigned short xe, unsigned short ye, unsigned short color_dat) {
    TFTSPI_Draw_Line(xs, ys, xs, ye, color_dat);    //���������
    TFTSPI_Draw_Line(xe, ys, xe, ye, color_dat);    //�������ұ�
    TFTSPI_Draw_Line(xs, ys, xe, ys, color_dat);    //�������ϱ�
    TFTSPI_Draw_Line(xs, ye, xe, ye, color_dat);    //�������±�
}

/*!
 * @brief    ��Բ
 *
 * @param    x ��Բ��x   (0-127)
 * @param    y ��Բ��y   (0-159)
 * @param    r ���뾶    (0-128)
 * @param    color_dat ����ɫ
 *
 * @return   ��
 *
 * @note     Բ�����겻Ҫ������Ļ��Χ
 *
 * @see      TFTSPI_Draw_Circle(50, 50, 30, u16YELLOW);
 *
 * @date     2019/6/13 ������
 */
void TFTSPI_Draw_Circle(unsigned short x, unsigned short y, unsigned short r,
        unsigned short color_dat) {
    unsigned short dx, dy = r;

    if ((x >= r) && ((TFT18W - x) >= r) && (y >= r) && ((TFT18H - y) >= r)) //ȷ������Բ����Ļ��Χ�ڣ�û�г�������ߣ�(�ݲ�֧������Ļ���ཻ)
            {
        for (dx = 0; dx <= r; dx++) {
            while((r * r + 1 - dx * dx) < (dy * dy)) dy--;
            TFTSPI_Draw_Dot(x + dx, y - dy, color_dat);
            TFTSPI_Draw_Dot(x - dx, y - dy, color_dat);
            TFTSPI_Draw_Dot(x - dx, y + dy, color_dat);
            TFTSPI_Draw_Dot(x + dx, y + dy, color_dat);

            TFTSPI_Draw_Dot(x + dy, y - dx, color_dat);
            TFTSPI_Draw_Dot(x - dy, y - dx, color_dat);
            TFTSPI_Draw_Dot(x - dy, y + dx, color_dat);
            TFTSPI_Draw_Dot(x + dy, y + dx, color_dat);
        }
    }
}

/*!
 * @brief    ����
 *
 * @param    xs ����ʼx
 * @param    ys ����ʼy
 * @param    xe ������x
 * @param    ys ������y
 * @param    color_dat ����ɫ
 *
 * @return   ��
 *
 * @note     ��ʼ����ֹ������(0-127)��������(0-159),��ʾ��ɫuint16
 *
 * @see      TFTSPI_Draw_Line(10, 20, 30, 40, u16YELLOW);
 *
 * @date     2019/6/13 ������
 */
void TFTSPI_Draw_Line(unsigned short xs, unsigned short ys, unsigned short xe,
        unsigned short ye, unsigned short color_dat) {
  uint16_t i;
	int16_t x, y, dx, dy, d, incrE, incrNE, temp;
	int16_t x0 = xs, y0 = ys, x1 = xe, y1 = ye;
	uint8_t yflag = 0, xyflag = 0;
#if USE_EX_GRAM
    if (xs == xe)                            //����ǻ���ֱ����ֻ�����ֱ�������
            {
				for (i = ys; i < (ye + 1); i++) {
            ex_gram[i*TFT18W + xs] = color_dat;
        }
    } else if (ys == ye)                       //�����ˮƽ����ֻ��Ҫ��ˮƽ�������
            {
				for (i = xs; i < (xe + 1); i++) {
            ex_gram[ys*TFT18W +i] = color_dat;
        }
    } else                              //�����б�ߣ������¼��㣬ʹ�û��㺯������ֱ��
    {
			/*ʹ��Bresenham�㷨��ֱ�ߣ����Ա����ʱ�ĸ������㣬Ч�ʸ���*/
			/*�ο��ĵ���https://www.cs.montana.edu/courses/spring2009/425/dslectures/Bresenham.pdf*/
			/*�ο��̳̣�https://www.bilibili.com/video/BV1364y1d7Lo*/
			
			if (x0 > x1)	//0�ŵ�X�������1�ŵ�X����
			{
				/*������������*/
				/*������Ӱ�컭�ߣ����ǻ��߷����ɵ�һ���������������ޱ�Ϊ��һ��������*/
				temp = x0; x0 = x1; x1 = temp;
				temp = y0; y0 = y1; y1 = temp;
			}
			
			if (y0 > y1)	//0�ŵ�Y�������1�ŵ�Y����
			{
				/*��Y����ȡ��*/
				/*ȡ����Ӱ�컭�ߣ����ǻ��߷����ɵ�һ�������ޱ�Ϊ��һ����*/
				y0 = -y0;
				y1 = -y1;
				
				/*�ñ�־λyflag����ס��ǰ�任���ں���ʵ�ʻ���ʱ���ٽ����껻����*/
				yflag = 1;
			}
			
			if (y1 - y0 > x1 - x0)	//����б�ʴ���1
			{
				/*��X������Y���껥��*/
				/*������Ӱ�컭�ߣ����ǻ��߷����ɵ�һ����0~90�ȷ�Χ��Ϊ��һ����0~45�ȷ�Χ*/
				temp = x0; x0 = y0; y0 = temp;
				temp = x1; x1 = y1; y1 = temp;
				
				/*�ñ�־λxyflag����ס��ǰ�任���ں���ʵ�ʻ���ʱ���ٽ����껻����*/
				xyflag = 1;
			}
			
			/*����ΪBresenham�㷨��ֱ��*/
			/*�㷨Ҫ�󣬻��߷������Ϊ��һ����0~45�ȷ�Χ*/
			dx = x1 - x0;
			dy = y1 - y0;
			incrE = 2 * dy;
			incrNE = 2 * (dy - dx);
			d = 2 * dy - dx;
			x = x0;
			y = y0;
			
			/*����ʼ�㣬ͬʱ�жϱ�־λ�������껻����*/
			if (yflag && xyflag){TFTSPI_Draw_Dot(y, -x, color_dat);}
			else if (yflag)		{TFTSPI_Draw_Dot(x, -y, color_dat);}
			else if (xyflag)	{TFTSPI_Draw_Dot(y, x, color_dat);}
			else				{TFTSPI_Draw_Dot(x,y, color_dat);}
			
			while (x < x1)		//����X���ÿ����
			{
				x ++;
				if (d < 0)		//��һ�����ڵ�ǰ�㶫��
				{
					d += incrE;
				}
				else			//��һ�����ڵ�ǰ�㶫����
				{
					y ++;
					d += incrNE;
				}
				
				/*��ÿһ���㣬ͬʱ�жϱ�־λ�������껻����*/
				if (yflag && xyflag){TFTSPI_Draw_Dot(y, -x, color_dat);}
				else if (yflag)		{TFTSPI_Draw_Dot(x, -y, color_dat);}
				else if (xyflag)	{TFTSPI_Draw_Dot(y, x, color_dat);}
				else				{TFTSPI_Draw_Dot(x, y, color_dat);}
			}			
    }
#else 
    if (xs == xe)                            //����ǻ���ֱ����ֻ�����ֱ�������
            {
        TFTSPI_Set_Pos(xs, ys, xe, ye);
        for (i = 0; i < (ye - ys + 1); i++) {
            TFTSPI_Write_Word(color_dat);
        }
    } else if (ys == ye)                       //�����ˮƽ����ֻ��Ҫ��ˮƽ�������
            {
        TFTSPI_Set_Pos(xs, ys, xe, ye);
        for (i = 0; i < (xe - xs + 1); i++) {
            TFTSPI_Write_Word(color_dat);
        }
    } else                              //�����б�ߣ������¼��㣬ʹ�û��㺯������ֱ��
    {
			/*ʹ��Bresenham�㷨��ֱ�ߣ����Ա����ʱ�ĸ������㣬Ч�ʸ���*/
			/*�ο��ĵ���https://www.cs.montana.edu/courses/spring2009/425/dslectures/Bresenham.pdf*/
			/*�ο��̳̣�https://www.bilibili.com/video/BV1364y1d7Lo*/
			
			if (x0 > x1)	//0�ŵ�X�������1�ŵ�X����
			{
				/*������������*/
				/*������Ӱ�컭�ߣ����ǻ��߷����ɵ�һ���������������ޱ�Ϊ��һ��������*/
				temp = x0; x0 = x1; x1 = temp;
				temp = y0; y0 = y1; y1 = temp;
			}
			
			if (y0 > y1)	//0�ŵ�Y�������1�ŵ�Y����
			{
				/*��Y����ȡ��*/
				/*ȡ����Ӱ�컭�ߣ����ǻ��߷����ɵ�һ�������ޱ�Ϊ��һ����*/
				y0 = -y0;
				y1 = -y1;
				
				/*�ñ�־λyflag����ס��ǰ�任���ں���ʵ�ʻ���ʱ���ٽ����껻����*/
				yflag = 1;
			}
			
			if (y1 - y0 > x1 - x0)	//����б�ʴ���1
			{
				/*��X������Y���껥��*/
				/*������Ӱ�컭�ߣ����ǻ��߷����ɵ�һ����0~90�ȷ�Χ��Ϊ��һ����0~45�ȷ�Χ*/
				temp = x0; x0 = y0; y0 = temp;
				temp = x1; x1 = y1; y1 = temp;
				
				/*�ñ�־λxyflag����ס��ǰ�任���ں���ʵ�ʻ���ʱ���ٽ����껻����*/
				xyflag = 1;
			}
			
			/*����ΪBresenham�㷨��ֱ��*/
			/*�㷨Ҫ�󣬻��߷������Ϊ��һ����0~45�ȷ�Χ*/
			dx = x1 - x0;
			dy = y1 - y0;
			incrE = 2 * dy;
			incrNE = 2 * (dy - dx);
			d = 2 * dy - dx;
			x = x0;
			y = y0;
			
			/*����ʼ�㣬ͬʱ�жϱ�־λ�������껻����*/
			if (yflag && xyflag){TFTSPI_Draw_Dot(y, -x, color_dat);}
			else if (yflag)		{TFTSPI_Draw_Dot(x, -y, color_dat);}
			else if (xyflag)	{TFTSPI_Draw_Dot(y, x, color_dat);}
			else				{TFTSPI_Draw_Dot(x,y, color_dat);}
			
			while (x < x1)		//����X���ÿ����
			{
				x ++;
				if (d < 0)		//��һ�����ڵ�ǰ�㶫��
				{
					d += incrE;
				}
				else			//��һ�����ڵ�ǰ�㶫����
				{
					y ++;
					d += incrNE;
				}
				
				/*��ÿһ���㣬ͬʱ�жϱ�־λ�������껻����*/
				if (yflag && xyflag){TFTSPI_Draw_Dot(y, -x, color_dat);}
				else if (yflag)		{TFTSPI_Draw_Dot(x, -y, color_dat);}
				else if (xyflag)	{TFTSPI_Draw_Dot(y, x, color_dat);}
				else				{TFTSPI_Draw_Dot(x, y, color_dat);}
			}	
		}			
#endif					

}

/*!
 * @brief    ����
 *
 * @param    x ��x
 * @param    y ��y
 * @param    color_dat ����ɫ
 *
 * @return   ��
 *
 * @note     ��ʼ����ֹ������(0-127)��������(0-159),��ʾ��ɫuint16
 *
 * @see      TFTSPI_Draw_Dot(10, 20, u16YELLOW);
 *
 * @date     2019/6/13 ������
 */
void TFTSPI_Draw_Dot(unsigned short x, unsigned short y,
        unsigned short color_dat) {
#if USE_EX_GRAM
    ex_gram[y*TFT18W + x] = color_dat;
#else 
	TFTSPI_Set_Pos(x, y, x, y);
	TFTSPI_Write_Word(color_dat);
#endif

}

void TFT_ShowChar(uint8_t X, uint8_t Y, char Char,unsigned short word_color, unsigned short back_color, enum Font_size Size)
{
	unsigned short i, j;
#if USE_EX_GRAM
	switch(Size)
	{
		case fsize_6X8:
			if(X<0||X>((TFT18W-1) - 6) || Y<0||Y>((TFT18H-1) - 8))
			{
				return;
			}
			for (j = 0; j < 8; j++) {
					for (i = 0; i < 6; i++) {
							if ((Font_code8X6[Char - 32][i]) & (0x01 << j))
									TFTSPI_Draw_Dot(X+i, Y+j, word_color);    //����
							else
									TFTSPI_Draw_Dot(X+i, Y+j, back_color);    //����
					}
			}			
			break;
			
		case fsize_8X16:
			if(X<0||X>((TFT18W-1) - 8) || Y<0||Y>((TFT18H-1) - 16))
			{
				return;
			}
			for (j = 0; j < 16; j++) {
					for (i = 0; i < 8; i++) {
							if ((Font_code8X16[Char - 32][j]) & (0x01 << i))
									TFTSPI_Draw_Dot(X+i, Y+j, word_color);    //����
							else
									TFTSPI_Draw_Dot(X+i, Y+j, back_color);    //����
					}
			}
			break;
		case fsize_12X24:
			if(X<0||X>((TFT18W-1) - 12) || Y<0||Y>((TFT18H-1) - 24))
			{
				return;
			}
			for (j = 0; j < 48; j++) {
					for (i = 0; i < 8; i++) {
							if ((Font_code12X24[Char - 32][j]) & (0x0001 << i))
									TFTSPI_Draw_Dot(X+i, Y+j, word_color);    //����
							else
									TFTSPI_Draw_Dot(X+i, Y+j, back_color);    //����
					}
			}
			break;
		case fsize_16X32:
			if(X<0||X>((TFT18W-1) - 8) || Y<0||Y>((TFT18H-1) - 16))
			{
				return;
			}
			for (j = 0; j < 64; j++) {
					for (i = 0; i < 8; i++) {
							if ((Font_code16X32[Char - 32][j]) & (0x0001 << i))
									TFTSPI_Draw_Dot(X+i, Y+j, word_color);    //����
							else
									TFTSPI_Draw_Dot(X+i, Y+j, back_color);    //����
					}
			}
			break;			
	}
#else
	switch(Size)
	{
		case fsize_6X8:
			TFTSPI_Set_Pos(X, Y , X+5, (Y + 1) * 8 - 1);   //��λ�ַ���ʾ����
			for (j = 0; j < 8; j++) {
					for (i = 0; i < 6; i++) {
							if ((Font_code8X6[Char - 32][i]) & (0x01 << j))
									TFTSPI_Write_Word(word_color);
							else
									TFTSPI_Write_Word(back_color);
					}
			}			
			break;
			
			
		case fsize_8X16:
			TFTSPI_Set_Pos(X, Y ,X+7, (Y + 1) * 16 - 1); //��λ�ַ���ʾ����
			for (j = 0; j < 16; j++) {
					for (i = 0; i < 8; i++) {
							if ((Font_code8X16[Char - 32][j]) & (0x01 << i))
									TFTSPI_Write_Word(word_color);
							else
									TFTSPI_Write_Word(back_color);
					}
			}
			break;
		case fsize_12X24:
			TFTSPI_Set_Pos(X, Y ,X+11, (Y + 1) * 24 - 1); //��λ�ַ���ʾ����
			for (j = 0; j < 48; j++) {
					for (i = 0; i < 8; i++) {
						if(j % 2 == 1 && i>=4)
						{
							//����16bit�ֿ�����4bit
							break;
						}

						if ((Font_code12X24[Char - 32][j]) & (0x0001 << i))
								TFTSPI_Write_Word(word_color);
						else
								TFTSPI_Write_Word(back_color);


					}
			}
			break;
		case fsize_16X32:
			TFTSPI_Set_Pos(X, Y ,X+15, (Y + 1) * 32 - 1); //��λ�ַ���ʾ����
			for (j = 0; j < 64; j++) {
					for (i = 0; i < 8; i++) {
							if ((Font_code16X32[Char - 32][j]) & (0x0001 << i))
									TFTSPI_Write_Word(word_color);
							else
									TFTSPI_Write_Word(back_color);
					}
			}
			break;
			
		default://�������Ĭ����С����
			TFTSPI_Set_Pos(X, Y , X+5, (Y + 1) * 8 - 1);   //��λ�ַ���ʾ����
			for (j = 0; j < 8; j++) {
					for (i = 0; i < 6; i++) {
							if ((Font_code8X6[Char - 32][i]) & (0x01 << j))
									TFTSPI_Write_Word(word_color);
							else
									TFTSPI_Write_Word(back_color);
					}
			}			
			break;
	}
		
#endif	
}

void TFT_ShowString(uint8_t X, uint8_t Y, char *String,unsigned short word_color, unsigned short back_color, enum Font_size Size)
{
	switch(Size)
	{
		case fsize_6X8:
				while(*String) {
					TFT_ShowChar(X,Y,*String++,word_color,back_color,fsize_6X8);
					X+=6;
				}		
			break;
			
		case fsize_8X16:
				while(*String) {
					TFT_ShowChar(X,Y,*String++,word_color,back_color,fsize_8X16);
					X+=8;
				}	
			break;	
				
		case fsize_12X24:
				while(*String) {
					TFT_ShowChar(X,Y,*String++,word_color,back_color,fsize_12X24);
					X+=12;
				}	
			break;				
		case fsize_16X32:
				while(*String) {
					TFT_ShowChar(X,Y,*String++,word_color,back_color,fsize_16X32);
					X+=16;
				}	
			break;	
	}
}

void TFT_Printf(uint8_t X, uint8_t Y,unsigned short word_color, unsigned short back_color ,enum Font_size Size, char *format, ...)
{
	char String[30];						//�����ַ�����
	va_list arg;							//����ɱ�����б��������͵ı���arg
	va_start(arg, format);					//��format��ʼ�����ղ����б�arg����
	vsprintf(String, format, arg);			//ʹ��vsprintf��ӡ��ʽ���ַ����Ͳ����б��ַ�������
	va_end(arg);							//��������arg
	TFT_ShowString(X,Y,String,word_color,back_color,Size);
}

/******************************************************************************
      ����˵������ʾͼƬ
      ������ݣ�x,y�������
                length ͼƬ����
                width  ͼƬ���
                pic[]  ͼƬ����    
      ����ֵ��  ��
******************************************************************************/
void TFT_ShowPicture(uint8_t x,uint8_t y,uint8_t length,uint8_t width,const uint8_t pic[])
{
	TFTSPI_Set_Pos(x,y,x+length-1,y+width-1);   //��λͼƬ��ʾ����
	TFTSPI_DC_H;       //DC=1��������

	HAL_SPI_Transmit(&SPI_Handler,pic, 2*length*width, 0xffff);//��������

}


