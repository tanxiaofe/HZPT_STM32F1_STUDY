#ifndef _TIME_H__
#define _TIME_H__
#include "stm32f10x.h"
#include "stm32f10x_it.h"
#define Pwm_Up 1;
#define Pwm_Down 0;
void Timer_Delay(uint32_t delay);
void led_PWM_Blink(void);
void TIM3_PWM_Init(uint16_t arr,uint16_t psc);
void Timer_Delays(uint16_t s);
void beep_alarm(void);
#endif
