#include "main.h"
#include "key.h"
#include "stdio.h"

extern car_typedef the_car;
uint8_t ui_key_falsh = 0; //UI与按键通信互斥量
void key_scan()
{
	uint8_t  key_value = 0xFF;
	//按键拉低电平有效
	key_value = ( ((uint8_t)KEY_OK_PIN)<<5|((uint8_t)KEY_BACK_PIN)<<4);//采集六个按键的状态
    if(ui_key_falsh==1)
    {
        /***兼容蓝牙模拟按键begin***/
        switch(the_car.BL_CMD)
        {
            case 1:
                key_value = 0x00; //OK
                the_car.BL_CMD = 0; //清除蓝牙指令
                break;
            case 2:
                key_value = 0x2F; //BACK
                the_car.BL_CMD = 0; //清除蓝牙指令
                break;
            case 3:
                key_value = 0x20; //UP
                the_car.BL_CMD = 0; //清除蓝牙指令
                break;
            case 4:
                key_value = 0x10; //DOWN
                the_car.BL_CMD = 0; //清除蓝牙指令
                break;
            case 5: 
                key_value = 0x0F; //LEFT
                the_car.BL_CMD = 0; //清除蓝牙指令
                break;
            case 6:     
                key_value = 0x1F; //RIGHT
                the_car.BL_CMD = 0; //清除蓝牙指令
                break;
            default:
            
                break;
        }
        /***兼容蓝牙模拟按键end***/

        switch(key_value)
        {		
            case 0x00://确认

                the_car.KEY.OK=1;//确认键按下
            break;
            case 0x2F://返回

                the_car.KEY.BACK=1;//返回键按下
            break;
            case 0x20://上

                the_car.KEY.UP=1;//上键按下
            break;
            case 0x10://下

                the_car.KEY.DOWN=1;//下键按下
            break;
            case 0x0F://左

                the_car.KEY.LEFT=1;//左键按下
            break;
            case 0x1F://右
                the_car.KEY.RIGHT=1;//右键按下
            break;
            default://其它情况
                the_car.KEY.OK=0;
                the_car.KEY.BACK=0;
                the_car.KEY.UP=0;
                the_car.KEY.DOWN=0;
                the_car.KEY.LEFT=0;
                the_car.KEY.RIGHT=0;     
        }
        ui_key_falsh = 0;//UI与KEY互斥量，表示KEY一次运行成功
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

