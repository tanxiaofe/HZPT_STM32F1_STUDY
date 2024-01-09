#include "stm32f10x.h"
#include "delay.h"
#include "led.h"
#include "time.h"
#include "usart.h"

/**
  * @brief  led initialization.
  * @param  None
  * @retval None
  */
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructer;
	
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	
	GPIO_InitStructer.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructer.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructer.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE,&GPIO_InitStructer);
	GPIO_Write(GPIOE, 0x0000);
}
/**
  * @brief  beep initialization.
  * @param  None
  * @retval None
  */
void Beep_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructer;
	
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStructer.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructer.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructer.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC,&GPIO_InitStructer);
	GPIO_Write(GPIOC, 0x0000);
}

/**
  * @brief  led blink control.
  * @param  GPIOx：io口所在的的组
  * @param  GPIO_Pin：io口引脚
  * @param  ms：延时ms数
  * @param  times：闪烁次数
  * @retval None
  */
void led_Blink(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,uint16_t ms,uint8_t times)
{	uint8_t  temp=0;
	while(temp<times)
	{//led on
	GPIO_SetBits( GPIOx, GPIO_Pin);
	delay_ms(ms);
	//led off
	GPIO_ResetBits(GPIOx, GPIO_Pin);
	delay_ms(ms);
	temp++;
	}
}
/**
  * @brief  led blink control.
  * @param  GPIOx：io口所在的的组
  * @param  GPIO_Pin：io口引脚
  * @param  ms：延时ms数
  * @param  times：闪烁次数
  * @retval None
  */
void led_Blink_timer(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,uint16_t ms,uint8_t times)
{	uint8_t  temp=0;
	while(temp<times)
	{//led on
	GPIO_SetBits( GPIOx, GPIO_Pin);
	Timer_Delay(ms);
	//led off
	GPIO_ResetBits(GPIOx, GPIO_Pin);
	Timer_Delay(ms);
	temp++;
	}
}
/**
  * @brief  led 演示函数每隔0.5s两两亮起，间隔1s两两灭掉
  * @param  None
  * @retval None
  */
void led_Display_On()
{
	GPIO_SetBits( GPIOE, GPIO_Pin_3);
	GPIO_SetBits( GPIOE, GPIO_Pin_4);
	Timer_Delay(50);
	GPIO_SetBits( GPIOE, GPIO_Pin_2);
	GPIO_SetBits( GPIOE, GPIO_Pin_5);
	Timer_Delay(50);
	GPIO_SetBits( GPIOE, GPIO_Pin_1);
	GPIO_SetBits( GPIOE, GPIO_Pin_6);
	Timer_Delay(50);
	GPIO_SetBits( GPIOE, GPIO_Pin_0);
	GPIO_SetBits( GPIOE, GPIO_Pin_7);
	Timer_Delay(200);
	GPIO_ResetBits( GPIOE, GPIO_Pin_0);
	GPIO_ResetBits( GPIOE, GPIO_Pin_7);
	Timer_Delay(100);
	GPIO_ResetBits( GPIOE, GPIO_Pin_1);
	GPIO_ResetBits( GPIOE, GPIO_Pin_6);
	Timer_Delay(100);
	GPIO_ResetBits( GPIOE, GPIO_Pin_2);
	GPIO_ResetBits( GPIOE, GPIO_Pin_5);
	Timer_Delay(100);
	GPIO_ResetBits( GPIOE, GPIO_Pin_3);
	GPIO_ResetBits( GPIOE, GPIO_Pin_4);
}

/**
  * @brief  led 演示函数每隔0.5s两两亮起，间隔1s两两灭掉用串口返回状态
  * @param  None
  * @retval None
  */
void led_Display_Usart()
{
	GPIO_SetBits( GPIOE, GPIO_Pin_3);
	GPIO_SetBits( GPIOE, GPIO_Pin_4);
	uart1SendArray("D4,D5 ON", sizeof("D4,D5 ON"));
	Timer_Delay(50);
	GPIO_SetBits( GPIOE, GPIO_Pin_2);
	GPIO_SetBits( GPIOE, GPIO_Pin_5);
	uart1SendArray("D3,D6 ON", sizeof("D3,D6 ON"));
	Timer_Delay(50);
	GPIO_SetBits( GPIOE, GPIO_Pin_1);
	GPIO_SetBits( GPIOE, GPIO_Pin_6);
	uart1SendArray("D2,D7 ON", sizeof("D4,D5 ON"));
	Timer_Delay(50);
	GPIO_SetBits( GPIOE, GPIO_Pin_0);
	GPIO_SetBits( GPIOE, GPIO_Pin_7);
	uart1SendArray("D1,D8 ON", sizeof("D1,D8 ON"));
	Timer_Delay(200);
	GPIO_ResetBits( GPIOE, GPIO_Pin_0);
	GPIO_ResetBits( GPIOE, GPIO_Pin_7);
	uart1SendArray("D1,D8 OFF", sizeof("D1,D8 OFF"));
	Timer_Delay(100);
	GPIO_ResetBits( GPIOE, GPIO_Pin_1);
	GPIO_ResetBits( GPIOE, GPIO_Pin_6);
	uart1SendArray("D2,D7 OFF", sizeof("D2,D7 OFF"));
	Timer_Delay(100);
	GPIO_ResetBits( GPIOE, GPIO_Pin_2);
	GPIO_ResetBits( GPIOE, GPIO_Pin_5);
	uart1SendArray("D3,D6 OFF", sizeof("D3,D6 OFF"));
	Timer_Delay(100);
	GPIO_ResetBits( GPIOE, GPIO_Pin_3);
	GPIO_ResetBits( GPIOE, GPIO_Pin_4);
	uart1SendArray("D4,D5 OFF", sizeof("D4,D5 OFF"));
	
}

void LED_All_Blink(uint16_t ms,uint8_t times)
{
	uint8_t  temp=0;
	while(temp<times)
	{//led on
	  GPIO_Write(GPIOE,ON_ALL);
	  delay_ms(ms);
	//led off
	  GPIO_Write(GPIOE,OFF_ALL);
	  delay_ms(ms);
	  temp++;
	}
}
/**
  * @brief  led 演示函数每隔0.5s熄灭
  * @param  None
  * @retval None
  */
void led_Display_Off()
{
	delay_ms(500);
	GPIO_ResetBits(GPIOE,GPIO_Pin_0);
	delay_ms(500);
	GPIO_ResetBits(GPIOE,GPIO_Pin_1);
	delay_ms(500);
	GPIO_ResetBits(GPIOE,GPIO_Pin_2);
	delay_ms(500);
	GPIO_ResetBits(GPIOE,GPIO_Pin_3);
	delay_ms(500);
	GPIO_ResetBits(GPIOE,GPIO_Pin_4);
	delay_ms(500);
	GPIO_ResetBits(GPIOE,GPIO_Pin_5);
	delay_ms(500);
	GPIO_ResetBits(GPIOE,GPIO_Pin_6);
	delay_ms(500);
	GPIO_ResetBits(GPIOE,GPIO_Pin_7);
}

/**
  * @brief  led 演示格雷码学号 例如39号0101/1101
  * @param  None
  * @retval None
  */
void led_Display_Gary()
{
	GPIO_ResetBits(GPIOE,GPIO_Pin_0);
	GPIO_SetBits(GPIOE,GPIO_Pin_1);
	GPIO_ResetBits(GPIOE,GPIO_Pin_2);
	GPIO_SetBits(GPIOE,GPIO_Pin_3);
	GPIO_SetBits(GPIOE,GPIO_Pin_4);
	GPIO_SetBits(GPIOE,GPIO_Pin_5);
	GPIO_ResetBits(GPIOE,GPIO_Pin_6);
	GPIO_SetBits(GPIOE,GPIO_Pin_7);
}
/**
  * @brief  led turn on all.
  * @param  .
  * @retval None
  */
void led_On_All(GPIO_TypeDef* GPIOx)
{
	GPIO_Write(GPIOx,ON_ALL);

	
}
/**
  * @brief led turn off all.
  * @param .
  * @retval None
  */
void led_Off_All(GPIO_TypeDef* GPIOx)
{
		GPIO_Write(GPIOx,OFF_ALL);
	
}
/**
  * @brief  beep turn on delay2s.
  * @param  .
  * @retval None
  */
void Beep_On_S(void)
{
	GPIO_SetBits(GPIOC,GPIO_Pin_6);	
	Timer_Delay(2000);
}

/**
  * @brief  beep turn on .
  * @param  .
  * @retval None
  */
void Beep_On(void)
{
	GPIO_SetBits(GPIOC,GPIO_Pin_4);	
	Timer_Delay(1000);
}
/**
  * @brief beep turn off .
  * @param .
  * @retval None
  */
void Beep_Off(void)
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_4);
		
	
}
/**
  * @brief  报警信号发生函数
  * @param  None
  * @retval None
  */
void beep_alarm(void)
{	
	uint8_t  temp=0;
	if(alarm_flag)
	{	
		while(temp<3)
		{	
		TIM_PrescalerConfig(TIM3,14400,TIM_PSCReloadMode_Update);//设置频率为0.5hz
		TIM_SetCompare1(TIM3,5000);//设置占空比50%
		TIM_CCxCmd(TIM3, TIM_Channel_1, TIM_CCx_Enable);
		Timer_Delay(1);//延时2s
		TIM_CCxCmd(TIM3, TIM_Channel_1, TIM_CCx_Disable);
		TIM_PrescalerConfig(TIM3,7200,TIM_PSCReloadMode_Immediate);///设置频率为1hz
		TIM_SetCompare1(TIM3,5000);//设置占空比50%
		TIM_CCxCmd(TIM3, TIM_Channel_1, TIM_CCx_Enable);
		Timer_Delays(2);//延时2s
		TIM_CCxCmd(TIM3, TIM_Channel_1, TIM_CCx_Disable);
		temp++;
		}
		temp=0;
		TIM_PrescalerConfig(TIM3,72,TIM_PSCReloadMode_Immediate);
		alarm_flag = 0;//清除报警标志位
	}
}
