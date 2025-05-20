#ifndef _UI_H
#define _UI_H
#include "main.h"
#define EEPROM_ADDRESS_BASE 0x0000 //eeprom参数保存的基址 

/**********************************************************/
struct option_class
{
	char* Name;				//选项名字
	void (*func)(void);		//函数指针
	uint8_t NameLen;		//由于中文占三个字节,用strlen计算名字宽度不再准确,故需额外储存名字宽度
};

struct _parameter_class
{
	char* Name;				//选项名字
	float (*fpara);//参数变量指针
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
