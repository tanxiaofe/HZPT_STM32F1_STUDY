#include "stm32f10x.h"                  // Device header
#include "delay.h" 
/**
  * @brief  ������ʼ������
  * @param  None
  * @retval None
  */
void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure; //����һ���ṹ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
 
}
