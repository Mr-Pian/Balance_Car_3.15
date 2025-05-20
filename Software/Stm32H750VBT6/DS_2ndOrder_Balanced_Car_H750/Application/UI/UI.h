#ifndef _UI_H
#define _UI_H
#include "main.h"
#define EEPROM_ADDRESS_BASE 0x0000 //eeprom��������Ļ�ַ 

/**********************************************************/
struct option_class
{
	char* Name;				//ѡ������
	void (*func)(void);		//����ָ��
	uint8_t NameLen;		//��������ռ�����ֽ�,��strlen�������ֿ�Ȳ���׼ȷ,������ⴢ�����ֿ��
};

struct _parameter_class
{
	char* Name;				//ѡ������
	float (*fpara);//��������ָ��
	uint32_t (*uipara);
	uint16_t E2P_addr;
	uint8_t NameLen;

};

/**********************************************************/


void run_menu(struct option_class* option);
void run_parameter(struct _parameter_class* option);
uint8_t Get_NameLen(char* String);

/**********************************************************/
void main_menu(void);
void UI_Show_Logo(void);
void UI_Show_RUN_Data(void);
/**********************************************************/

#endif
