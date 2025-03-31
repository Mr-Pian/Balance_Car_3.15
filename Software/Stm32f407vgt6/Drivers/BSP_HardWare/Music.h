#ifndef __MUSIC_H
#define __MUSIC_H

#include "main.h"

#define SOFTWARE_PWM 1  //Ê¹ÄÜÈí¼þpwm

typedef struct 
{
	int tones[256];
	int beats[256];
	int num;
}Music_typedef;

void Tone_Scale(void);
void Play_Music(Music_typedef* music);
void Software_Pwm(float tone, float Beat);

extern Music_typedef YOASOBI_1;
extern Music_typedef Birthday;
extern Music_typedef Open;

#endif