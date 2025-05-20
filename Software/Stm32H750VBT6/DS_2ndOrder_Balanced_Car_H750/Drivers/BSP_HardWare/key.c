#include "main.h"
#include "key.h"
#include "stdio.h"

extern car_typedef the_car;
uint8_t ui_key_falsh = 0; //UI�밴��ͨ�Ż�����
void key_scan()
{
	uint8_t  key_value = 0xFF;
	//�������͵�ƽ��Ч
	key_value = ( ((uint8_t)KEY_OK_PIN)<<5|((uint8_t)KEY_BACK_PIN)<<4);//�ɼ�����������״̬
    if(ui_key_falsh==1)
    {
        /***��������ģ�ⰴ��begin***/
        switch(the_car.BL_CMD)
        {
            case 1:
                key_value = 0x00; //OK
                the_car.BL_CMD = 0; //�������ָ��
                break;
            case 2:
                key_value = 0x2F; //BACK
                the_car.BL_CMD = 0; //�������ָ��
                break;
            case 3:
                key_value = 0x20; //UP
                the_car.BL_CMD = 0; //�������ָ��
                break;
            case 4:
                key_value = 0x10; //DOWN
                the_car.BL_CMD = 0; //�������ָ��
                break;
            case 5: 
                key_value = 0x0F; //LEFT
                the_car.BL_CMD = 0; //�������ָ��
                break;
            case 6:     
                key_value = 0x1F; //RIGHT
                the_car.BL_CMD = 0; //�������ָ��
                break;
            default:
            
                break;
        }
        /***��������ģ�ⰴ��end***/

        switch(key_value)
        {		
            case 0x00://ȷ��

                the_car.KEY.OK=1;//ȷ�ϼ�����
            break;
            case 0x2F://����

                the_car.KEY.BACK=1;//���ؼ�����
            break;
            case 0x20://��

                the_car.KEY.UP=1;//�ϼ�����
            break;
            case 0x10://��

                the_car.KEY.DOWN=1;//�¼�����
            break;
            case 0x0F://��

                the_car.KEY.LEFT=1;//�������
            break;
            case 0x1F://��
                the_car.KEY.RIGHT=1;//�Ҽ�����
            break;
            default://�������
                the_car.KEY.OK=0;
                the_car.KEY.BACK=0;
                the_car.KEY.UP=0;
                the_car.KEY.DOWN=0;
                the_car.KEY.LEFT=0;
                the_car.KEY.RIGHT=0;     
        }
        ui_key_falsh = 0;//UI��KEY����������ʾKEYһ�����гɹ�
    }
}

//void key_init()
//{
//	key_ok_PIN = 1;
//	KEY_BACK_PIN = 1;
//	KEY_UP_PIN = 1;
//	KEY_DOWN_PIN = 1;
//	KEY_LEFT_PIN = 1;
//	KEY_RIGHT_PIN = 1;
//}

