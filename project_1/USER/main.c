/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */  

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

#include <stdio.h>
#include "led.h"
#include "delay.h"
#include "key.h"
#include "exti.h"
#include "time.h"
#include "usart.h"



/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f10x_xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f10x.c file
     */     
		LED_Init();		//LED��ʾģ���ʼ��
		delay_init();	//��ʱģ���ʼ��
		Key_Init();		//������ʼ��
		GPIO_EXTI_Init();	//GPIO�жϳ�ʼ��
		TIM3_PWM_Init(10000,72);//PWM,��ʱ����10�����ж�
		Uart1GpioInit();	// ����GPIO��ʼ��
		Uart1Config();		// ���ں�DMA����
		
  /* Add your application code here
     */
  /* Infinite loop */
 while(1)
	{
		switch(key_flag)
		{	//S1
			case KEY1_ENABLE :
				led_Display_Gary();	//������չʾѧ��
			break;
			//S2
			case KEY2_ENABLE :
				if(monitor_flag)
				{	
					led_Off_All(GPIOE);	//ʹ��ȫ��
					led_Display_Usart();//LED���0.5s��������ͣ2s���������1s���״̬���ڴ�ӡ
					key_flag = KEY_DISABLE;//������־����
					monitor_flag = monitor_disable;//led״̬ʹ�ܱ�־λ����
				}
				else
				{
					led_Off_All(GPIOE);	//ʹ��ȫ��
					led_Display_On();	//LED���0.5s��������ͣ2s���������1s���
					key_flag = KEY_DISABLE;//������־����
				}
			break;
			//default
			default:
			break; 	
		}
		USART1_ReciveDataHand();//����1���ճ���
		beep_alarm();	//��������
	}
}
