#ifndef _LED_H__
#define _LED_H__

#include "stm32f10x.h"

#define ON_ALL (uint16_t)(255 << 0)
#define OFF_ALL	(uint16_t)~(255 << 0)



void LED_Init(void);
void led_Blink(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,uint16_t ms,uint8_t times);
void led_Blink_timer(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,uint16_t ms,uint8_t times);
void LED_All_Blink(uint16_t ms,uint8_t times);
void led_Display_On(void);
void led_Display_Off(void);
void led_Display_Gary(void);
void led_Display_Usart(void);
void led_On_All(GPIO_TypeDef* GPIOx);
void led_Off_All(GPIO_TypeDef* GPIOx);
void Beep_Init(void);
void Beep_On_S(void);
void Beep_On(void);
void Beep_Off(void);

#endif
