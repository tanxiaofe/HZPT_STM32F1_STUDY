#ifndef _EXTI_H__
#define _EXTI_H__

#define KEY1_ENABLE 1
#define KEY2_ENABLE 2
#define KEY3_ENABLE 3
#define KEY4_ENABLE 4
#define KEY_DISABLE 0

#include "stm32f10x.h"
#include "stm32f10x_it.h"
void GPIO_EXTI_Init(void);
void  EXTI0_IRQHandler (void);
void  EXTI1_IRQHandler (void);
void  EXTI2_IRQHandler (void);
void  EXTI3_IRQHandler (void);

#endif
