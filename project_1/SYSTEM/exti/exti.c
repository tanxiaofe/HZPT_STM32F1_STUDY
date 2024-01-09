#include "exti.h"
#include "led.h"
/**
  * @brief  GPIO中断初始化
  * @param  None
  * @retval None
  */
void GPIO_EXTI_Init(void)
{	
	EXTI_InitTypeDef EXTI_InitStructer;
	NVIC_InitTypeDef NVIC_InitStructer;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	//PC0
	GPIO_EXTILineConfig( GPIO_PortSourceGPIOC,GPIO_PinSource0);
	EXTI_InitStructer.EXTI_Line = EXTI_Line0;
	EXTI_InitStructer.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructer.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructer.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructer);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructer.NVIC_IRQChannel =EXTI0_IRQn;
	NVIC_InitStructer.NVIC_IRQChannelPreemptionPriority =0x01 ;
	NVIC_InitStructer.NVIC_IRQChannelSubPriority =0x01;
	NVIC_InitStructer.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructer);
	//PC1
	GPIO_EXTILineConfig( GPIO_PortSourceGPIOC,GPIO_PinSource1);
	EXTI_InitStructer.EXTI_Line = EXTI_Line1;
	EXTI_InitStructer.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructer.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructer.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructer);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructer.NVIC_IRQChannel =EXTI1_IRQn;
	NVIC_InitStructer.NVIC_IRQChannelPreemptionPriority =0x01 ;
	NVIC_InitStructer.NVIC_IRQChannelSubPriority =0x01;
	NVIC_InitStructer.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructer);
	//PC2
	GPIO_EXTILineConfig( GPIO_PortSourceGPIOC,GPIO_PinSource2);
	EXTI_InitStructer.EXTI_Line = EXTI_Line2;
	EXTI_InitStructer.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructer.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructer.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructer);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructer.NVIC_IRQChannel =EXTI2_IRQn;
	NVIC_InitStructer.NVIC_IRQChannelPreemptionPriority =0x01 ;
	NVIC_InitStructer.NVIC_IRQChannelSubPriority =0x01;
	NVIC_InitStructer.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructer);
	//PC3
	GPIO_EXTILineConfig( GPIO_PortSourceGPIOC,GPIO_PinSource3);
	EXTI_InitStructer.EXTI_Line = EXTI_Line3;
	EXTI_InitStructer.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructer.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructer.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructer);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructer.NVIC_IRQChannel =EXTI3_IRQn;
	NVIC_InitStructer.NVIC_IRQChannelPreemptionPriority =0x01 ;
	NVIC_InitStructer.NVIC_IRQChannelSubPriority =0x01;
	NVIC_InitStructer.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructer);
	
	
	
	
}
/**
  * @brief  GPIOPC0中断服务子程序
  * @param  None
  * @retval None
  */
void  EXTI0_IRQHandler (void)
{
	//检测中断标志位flag
	if(EXTI_GetITStatus(EXTI_Line0) != RESET)
	{		
	//逻辑代码
		key_flag = 1;
	}
	//清除中断标志位
	EXTI_ClearITPendingBit(EXTI_Line0);
}
/**
  * @brief  GPIOPC1中断服务子程序
  * @param  None
  * @retval None
  */
void  EXTI1_IRQHandler (void)
{
	//检测中断标志位flag
	if(EXTI_GetITStatus(EXTI_Line1) != RESET)
	{		
	//逻辑代码
		key_flag = 2;
	}
	//清除中断标志位
	EXTI_ClearITPendingBit(EXTI_Line1);
}
/**
  * @brief  GPIOPC2中断服务子程序
  * @param  None
  * @retval None
  */
void  EXTI2_IRQHandler (void)
{
	//检测中断标志位flag
	if(EXTI_GetITStatus(EXTI_Line2) != RESET)
	{		
	//逻辑代码
		key_flag = 3;
	}
	//清除中断标志位
	EXTI_ClearITPendingBit(EXTI_Line2);
}

/**
  * @brief  GPIOPC3中断服务子程序
  * @param  None
  * @retval None
  */
void  EXTI3_IRQHandler (void)
{
	//检测中断标志位flag
	if(EXTI_GetITStatus(EXTI_Line3) != RESET)
	{		
	//逻辑代码
		key_flag = 4;
	}
	//清除中断标志位
	EXTI_ClearITPendingBit(EXTI_Line3);
}
