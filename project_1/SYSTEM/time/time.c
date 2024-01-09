#include "time.h"
#include "led.h"
#include "delay.h"
#define MAX_DELAY      0xFFFFFFFFU

/**
  * @brief  TIM3��ʼ������
  * @param  None
  * @retval None
  */
void TIM3_PWM_Init(uint16_t arr, uint16_t psc)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;


    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʹ�ܶ�ʱ��3ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC  | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��

    GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE); //Timer3������ӳ��  TIM3_CH1->PC6

    //���ø�����Ϊ�����������,���TIM3 CH1��PWM���岨��	GPIOC.6
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //TIM_CH2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIO

    //��ʼ��TIM3
    TIM_TimeBaseStructure.TIM_Period = arr-1; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
    TIM_TimeBaseStructure.TIM_Prescaler = psc-1; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

    //��ʼ��TIM3 Channel1 PWMģʽ
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC1

    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ���

    TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3
	TIM_CCxCmd(TIM3, TIM_Channel_1, TIM_CCx_Disable);//TIM3�ر�PWM����
}
/**
  * @brief  TIM3��ʱ����
  * @param  None
  * @retval None
  */
void Timer_Delays(uint16_t s)
{
	delay_ms(s*500);
	delay_ms(s*500);
}
/**
  * @brief  TIM3�жϷ����ӳ���
  * @param  None
  * @retval None
  */
void  TIM3_IRQHandler (void)
{
	//����жϱ�־λflag
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)
	{		
	//�߼�����
		g_counter++;
	}
	//����жϱ�־λ
	 TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
}

/**
  * @brief  led pwm control.
  * @param  GPIOx��io�����ڵĵ���
  * @param  GPIO_Pin��io������
  * @param  ms����ʱms��
  * @param  times����˸����
  * @retval None
  */
void led_PWM_Blink(void)
{		
	static uint16_t led_value = 0;
	static uint8_t Pwm_Dir = Pwm_Up;
	if(Pwm_Dir)
	{
	led_value++;//led_value�Լ�
		if(led_value>=800)
		{
			Pwm_Dir = Pwm_Down;
		}
	}
	else
	{	
		led_value--;//led����value�Լ�
		if(led_value<=0)
		{
			Pwm_Dir = Pwm_Up;
		}
	}
	TIM_SetCompare1(TIM3,led_value);
}


/**
  * @brief  TIM3��ʱ����
  * @param  None
  * @retval None
  */
void Timer_Delay(uint32_t delay)
{
  uint32_t timerstart = g_counter;
  uint32_t timerwait = delay;
  /* Add a freq to guarantee minimum wait */
  if (timerwait < MAX_DELAY)
  {
    timerwait += (uint32_t)(1);
  }

  while (( g_counter - timerstart) < timerwait)
  {
  }
}

