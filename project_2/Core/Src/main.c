/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdlib.h>
#include "bsp_ili9341_lcd.h"
#include "lv_port_disp.h"
#include "lvgl.h"
#include "stm32f1xx_it.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BUFF_SIZE	100
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t rx_buff[BUFF_SIZE];
char str[20];
char str2[20];
char str3[20];
char str4[20];
uint8_t hour = 0;
uint8_t min =0 ;
uint8_t hour_set = 0;
uint8_t min_set =0 ;
uint8_t temp = 0;
uint8_t busy = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#ifdef __GNUC__
     #define PUTCHAR_PROTOTYPE int _io_putchar(int ch)
 #else
     #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
 #endif /* __GNUC__*/
/******************************************************************
     *@brief  Retargets the C library printf  function to the USART.
     *@param  None
     *@retval None
 ******************************************************************/
 PUTCHAR_PROTOTYPE
 {
     HAL_UART_Transmit(&huart1, (uint8_t *)&ch,1,0xFFFF);
     return ch;
 }

void Fake_clock(void)
{
	if(TimerCnt==60)	//60秒计时到达
	{
		min++;					//增加分钟数值
		TimerCnt=0;			//秒中断标志清零
	}
	else if(min==60)	//60分钟计时到达
	{
		hour++;				//增加小时数值
		min=0;				//分钟数值清零
	}
	else if(hour==24)	//24小时计时到达
	{
		hour=0;				//小时数归零
	}
}

static uint8_t char1[8];
static uint8_t char2[8];
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
//	lv_init();                  // lvgl初始化，如果这个没有初始化，那么下面的初始化会崩溃
//	lv_port_disp_init();        // 显示器初始化
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
	
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
	HAL_UARTEx_ReceiveToIdle_DMA(&huart1, rx_buff, BUFF_SIZE);	
	LCD_Init();		    //液晶初始化
	POINT_COLOR = BLUE;	//设置画笔的颜色
	LCD_Clear(WHITE);		//显示基本颜色
	Draw_Circle(119,159,100);
	Test_Show_CH_Font16(80,100,0,RED);
	Test_Show_CH_Font16(100,100,1,RED);
	Test_Show_CH_Font16(120,100,2,RED);
	Test_Show_CH_Font16(140,100,3,RED);
	//lvgl_first_demo_start();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {		
		HAL_DEBUG = 1;
		sprintf((char*)char1,"%02d:%02d:%02d",hour,min,TimerCnt);
		LCD_ShowString(90,150,char1);
		if(hour==hour_set&&min==min_set&&TimerCnt==0)	//设定时间到闹钟响
		{
			alarm_timeup();		
			alarmed_flag = 1;
		}
			//lv_task_handler();    // 运行所有lvgl的timer
		switch(KEY_FALG)
		{
			case KEY1_ENABLE:								//调整小时
				printf("请输入小时\n");
				hour_adjust_flag = 1;
				busy=1;
				KEY_FALG = KEY_DISABLE;
			break;
			case KEY2_ENABLE:								//调整分钟
					printf("请输入分钟\n");
					min_adjust_flag = 1;
					busy=1;
					KEY_FALG = KEY_DISABLE;
			break;								
			case KEY3_ENABLE:									//设置闹钟	
					printf("闹钟设定，请输入闹钟时间（格式 hh-mm）：\n");
					alarm_adjust_flag = 1;
					busy=1;
					KEY_FALG = KEY_DISABLE;
			break;
			case KEY4_ENABLE:	//关闭闹钟
					if(alarmed_flag)
					{alarm_stop();
					printf("闹钟关闭\n");
					alarmed_flag=0;
					KEY_FALG = KEY_DISABLE;
					}
					else
					{
						KEY_FALG = KEY_DISABLE;
					}
					break;
			default:		
						if(!busy)//每秒打印时间
					{					
						printf("当前时间：%d%d:%d%d:%d%d\n",hour/10,hour%10,min/10,min%10,TimerCnt/10,TimerCnt%10);
						HAL_Delay(1000);
					}
				break;
		}	

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enables the Clock Security System
  */
  HAL_RCC_EnableCSS();
}

/* USER CODE BEGIN 4 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef * huart, uint16_t Size)
{
    if(huart->Instance == USART1)
    {
        if (Size <= BUFF_SIZE)
        {
            HAL_UARTEx_ReceiveToIdle_DMA(&huart1, rx_buff, BUFF_SIZE); // 接收完毕后重启
						if(hour_adjust_flag)
						{	
								sprintf(str, "%s", rx_buff);
								temp=atoi(str);
								if(temp>23)
								{
									temp=0;
									printf("set error：please input again\n");
								}
								else
								{
									hour = temp;
									HAL_UART_Transmit(&huart1, rx_buff, Size, 0xffff);         // 将接收到的数据再发出
									temp=0;
									busy=0;
									TimerCnt=0;
									hour_adjust_flag = 0;
									
								}
							}
						 else if(min_adjust_flag)
							{
								sprintf(str, "%s", rx_buff);
								temp=atoi(str);
								if(temp>59)
								{
									temp=0;
									printf("set error：please input again\n");
								}
								else
								{
									min = temp;
									HAL_UART_Transmit(&huart1, rx_buff, Size, 0xffff);         // 将接收到的数据再发出
									temp=0;
									busy=0;
									TimerCnt=0;
									min_adjust_flag = 0;
								}
							}
						else if(alarm_adjust_flag)
						{		int temp1,temp2;
							sprintf(str, "%s", rx_buff);
							strncpy(str2,str,2);
							strncpy(str3,str+3,2);
							strncpy(str4,str+2,1);
							temp1=atoi(str2);
							temp2=atoi(str3);
							if(!(memcmp(str4,"-",sizeof("-"))==0))
							{
								printf("format error：please input again\n");
							}
							else if(temp1>23)
							{
								printf("format error：please input again\n");
								temp1=0;
							}
							else if(temp2>59) 
							{
								printf("format error：please input again\n");
								temp1=0;
								temp2=0;
							}
							else
							{
								hour_set= temp1;
								min_set = temp2;
								printf("alarm_set\n");
								HAL_UART_Transmit(&huart1, rx_buff, Size, 0xffff);         // 将接收到的数据再发出
								temp1=0;
								temp2=0;
								busy=0;
								alarm_adjust_flag = 0;
							}
						}
						else 
						{
							printf("command error：please input again\n");
						}
          memset(rx_buff, 0, BUFF_SIZE);							   // 清除接收缓存
        }
        else  // 接收数据长度大于BUFF_SIZE，错误处理
        {
            
        }
    }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef * huart)
{
    if(huart->Instance == USART1)
    {
		HAL_UARTEx_ReceiveToIdle_DMA(&huart1, rx_buff, BUFF_SIZE); // 接收发生错误后重启
		memset(rx_buff, 0, BUFF_SIZE);							   // 清除接收缓存
        
    }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
