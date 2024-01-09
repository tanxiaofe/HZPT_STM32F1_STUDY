#include "time.h"
#include "led.h"
#include "delay.h"
#define MAX_DELAY      0xFFFFFFFFU

/**
  * @brief  TIM3初始化程序
  * @param  None
  * @retval None
  */
void TIM3_PWM_Init(uint16_t arr, uint16_t psc)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;


    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//使能定时器3时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC  | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟

    GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE); //Timer3部分重映射  TIM3_CH1->PC6

    //设置该引脚为复用输出功能,输出TIM3 CH1的PWM脉冲波形	GPIOC.6
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //TIM_CH2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIO

    //初始化TIM3
    TIM_TimeBaseStructure.TIM_Period = arr-1; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
    TIM_TimeBaseStructure.TIM_Prescaler = psc-1; //设置用来作为TIMx时钟频率除数的预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE ); //使能指定的TIM3中断,允许更新中断

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

    //初始化TIM3 Channel1 PWM模式
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC1

    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR1上的预装载寄存器

    TIM_Cmd(TIM3, ENABLE);  //使能TIM3
	TIM_CCxCmd(TIM3, TIM_Channel_1, TIM_CCx_Disable);//TIM3关闭PWM功能
}
/**
  * @brief  TIM3延时函数
  * @param  None
  * @retval None
  */
void Timer_Delays(uint16_t s)
{
	delay_ms(s*500);
	delay_ms(s*500);
}
/**
  * @brief  TIM3中断服务子程序
  * @param  None
  * @retval None
  */
void  TIM3_IRQHandler (void)
{
	//检测中断标志位flag
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)
	{		
	//逻辑代码
		g_counter++;
	}
	//清除中断标志位
	 TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
}

/**
  * @brief  led pwm control.
  * @param  GPIOx：io口所在的的组
  * @param  GPIO_Pin：io口引脚
  * @param  ms：延时ms数
  * @param  times：闪烁次数
  * @retval None
  */
void led_PWM_Blink(void)
{		
	static uint16_t led_value = 0;
	static uint8_t Pwm_Dir = Pwm_Up;
	if(Pwm_Dir)
	{
	led_value++;//led_value自加
		if(led_value>=800)
		{
			Pwm_Dir = Pwm_Down;
		}
	}
	else
	{	
		led_value--;//led——value自减
		if(led_value<=0)
		{
			Pwm_Dir = Pwm_Up;
		}
	}
	TIM_SetCompare1(TIM3,led_value);
}


/**
  * @brief  TIM3延时函数
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

