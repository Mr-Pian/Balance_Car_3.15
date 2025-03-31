#include "Music.h"
#include "tim.h"
#include "delay.h"
#include "gpio.h"

#define dSo 1275*2*1.7
#define dLa 2272*1.7
#define dXi 2024*1.7
#define Do 1911*1.7
#define Re 1702*1.7
#define Mi 1516*1.7
#define Fa 1431*1.7
#define So 1275*1.7
#define La 1136*1.7
#define Xi 1012*1.7
#define uDo 956*1.7


#define B1 461
#define B2 666


Music_typedef YOASOBI_1={
{956*1.7,1012*1.7,1275*1.7,
//uDo,Xi,So,
So,La,Mi,Re,Do,Re,La,
So,Re,Mi,Do,Re,Do,Do,So,
Fa,Mi,Re,Do,dXi,Do,Re,Fa,
Mi,Re,Mi,La,So,Mi,So,
So,La,Fa,Mi,Re,Do,Re,La,
So,La,Mi,Do,Re,Do,dLa,dXi,
Do,dLa,Do,Re,Mi,Do,Mi,La,
1205.5*1.7,Do,Do,uDo,Xi,So,
So,La,Mi,Re,Do,Re,La,
So,La,Mi,Do,Re,Do,Do,So,	
Fa,Mi,Re,Do,dXi,Do,Re,Fa,
Mi,Re,Mi,La,So,Mi,So,
So,La,Fa,Mi,Re,Xi,La,So,	
So,La,Xi,uDo,uDo,Mi,Re,Do,
0,dLa,Do,Fa,Mi,Do,Re,Do
},
{B1/2,B1/2,B1/2,
B1*3/4,B1/2,B1*3/4,B1/2,B1/2,B1/2,B1/2,
B1/2,B1/4,B1/2,B1/8,B1*5/8,B1,B1/2,B1/2,
B1/2,B1/2,B1/2,B1/2,B1/2,B1/2,B1/2,B1/2,
B1/2,B1/4,B1/2,B1*3/4,B1,B1/2,B1/2,
B1/8,B1*5/8,B1*3/4,B1/2,B1/2,B1/2,B1/2,B1/2,
B1/2,B1/4,B1/2,B1/8,B1*5/8,B1,B1/2,B1/2,
B1/2,B1/2,B1/2,B1/2,B1/2,B1/2,B1/2,B1/2,	
B1*3/2,B1/2,B1/2,B1/2,B1/2,B1/2,
B1*3/4,B1/2,B1*3/4,B1/2,B1/2,B1/2,B1/2,
B1/2,B1/4,B1/2,B1/8,B1*5/8,B1,B1/2,B1/2,
B1/2,B1/2,B1/2,B1/2,B1/2,B1/2,B1/2,B1/2,
B1/2,B1/4,B1/2,B1*3/4,B1,B1/2,B1/2,
B1/8,B1*5/8,B1*3/4,B1/2,B1/2,B1/2,B1/2,B1/2,	
B1/2,B1/2,B1/2,B1/2,B1/2,B1/2,B1/2,B1/2,
B1/2,B1/2,B1/2,B1/4,B1*3/4,B1/2,B1/2,B1},
120,
};


Music_typedef Birthday={
{dSo,dSo,dLa,dSo,Do,dXi,
dSo,dSo,dLa,dSo,Re,Do,
dSo,dSo,So,Mi,Do,dXi,dLa,
0,Fa,Fa,Mi,Do,Re,Do},
{B2/2,B2/2,B2,B2,B2,B2*2,
B2/2,B2/2,B2,B2,B2,B2*2,
B2/2,B2/2,B2,B2,B2,B2,B2,
B2/6,B2/2,B2/2,B2,B2,B2,B2*4},
28,
};

Music_typedef Open={
	{Do, Re, So},
	{B2/2, B2/2, B2},
	3,
};

extern int status;

//void Tone_Scale(void){

//	__HAL_TIM_SetAutoreload(&htim4,2272*1.7);//  down la
//	  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);
//	 HAL_Delay(300);
//	 HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_3);
//	
//	__HAL_TIM_SetAutoreload(&htim4,2024*1.7);// down xi
//	  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);
//	 HAL_Delay(300);
//	 HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_3);
//	
//	__HAL_TIM_SetAutoreload(&htim4,1911*1.7);// do
//	  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);
//	 HAL_Delay(300);
//	 HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_3);
//	 
//	 __HAL_TIM_SetAutoreload(&htim4,1702*1.7); //re
//	  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);
//	 HAL_Delay(300);
//	 HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_3);
//	 
//	  __HAL_TIM_SetAutoreload(&htim4,1516*1.7);//mi
//	  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);
//	 HAL_Delay(300);
//	 HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_3);
//	 
//	 
//	  __HAL_TIM_SetAutoreload(&htim4,1431*1.7); //fa  
//	  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);
//	 HAL_Delay(300);
//	 HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_3);
//	 
//	 	  __HAL_TIM_SetAutoreload(&htim4,1275*1.7);  //so
//	  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);
//	 HAL_Delay(300);
//	 HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_3);
//	 
//	 __HAL_TIM_SetAutoreload(&htim4,1136*1.7);  //la
//	  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);
//	 HAL_Delay(300);
//	 HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_3);
//	 
//	 __HAL_TIM_SetAutoreload(&htim4,1012*1.7);  //xi
//	  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);
//	 HAL_Delay(300);
//	 HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_3);
//	 
//	 __HAL_TIM_SetAutoreload(&htim4,956*1.7);  //do **预分频低于1000无法发出音调
//	  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);
//	 HAL_Delay(300);
//	 HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_3);

//}

#if SOFTWARE_PWM
void Software_Pwm(float tone, float Beat)
{
	int times = (int)((Beat/(tone/2.0))*1000.0);
	while (times--)
	{
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_8);
		delay_us(tone/2);
	}
}
#endif

void Play_Music(Music_typedef* music)
{
	#if SOFTWARE_PWM
	for(int i=0;i<=music->num;i++)
	{
		Software_Pwm((float)music->tones[i], (float)music->beats[i]);
	}
	#else
	for( int i=0;i<=music->num;i++)
	{
	 __HAL_TIM_SetAutoreload(&htim4,music->tones[i]);  
		HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);
	 HAL_Delay(music->beats[i]);
	 HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_3);
	}
	#endif
}