#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_dma.h"
#include "misc.h"
#include "delay.h"	
#include "usart.h"

uint8_t uart1RecvData[32] = {0};    // 接收数据缓冲区
uint8_t uart1RecvFlag = 0;          // 接收完成标志位
uint8_t uart1RecvLen = 0;           // 接收的数据长度

uint8_t uart1SendData[32] = {0};    // 发送数据缓冲区
uint8_t uart1SendFlag = 0;          // 发送完成标志位
uint8_t monitor_flag=0;
uint8_t alarm_flag=0;
uint8_t command1[]="monitor";
uint8_t command2[]="alarm";
uint8_t error[]="command error please input agian";

/* 串口1 GPIO引脚初始化 */
void Uart1GpioInit(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   // 使能GPIOA时钟
    
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;    // TX 推挽输出
    GPIO_InitStruct.GPIO_Pin = UART1_TX_PIN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(UART1_TX_PORT, &GPIO_InitStruct);     // PA9
    
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;      // RX上拉输入
    GPIO_InitStruct.GPIO_Pin = UART1_RX_PIN;
    GPIO_Init(UART1_RX_PORT, &GPIO_InitStruct);     // PA10
}



/* 串口1配置 115200 8n1 */
void Uart1Config(void)
{
    USART_InitTypeDef USART_InitStruct;		// 串口配置
    NVIC_InitTypeDef NVIC_InitStructure;	// 中断配置
    DMA_InitTypeDef DMA_InitStruct;			// DMA 配置
    
    USART_DeInit(USART1);   // 寄存器恢复默认值
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);  // 使能串口时钟
    
    /* 串口参数配置 */
    USART_InitStruct.USART_BaudRate = BAUD_RATE;            // 波特率：115200
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;    // 无流控
    USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;    // 收发
    USART_InitStruct.USART_Parity = USART_Parity_No;                // 无校验位 
    USART_InitStruct.USART_StopBits = USART_StopBits_1;             // 1个停止位
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;        // 8个数据位
    USART_Init(USART1, &USART_InitStruct);
    USART_Cmd(USART1, ENABLE);  // 使能串口
    
    /* 串口中断配置 */
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             // 使能
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   // 抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;          // 子优先级
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;           // 串口1中断
    NVIC_Init(&NVIC_InitStructure);     // 嵌套向量中断控制器初始化

    USART_ITConfig(USART1, USART_IT_TC,   ENABLE);  // 使能串口发送中断，发送完成产生 USART_IT_TC 中断
    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);  // 使能串口空闲中断，接收一帧数据产生 USART_IT_IDLE 空闲中断
    
    /* 串口DMA配置 */
    DMA_DeInit(DMA1_Channel4);  // DMA1 通道4，寄存器复位
    DMA_DeInit(DMA1_Channel5);  // DMA1 通道5，寄存器复位
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);  // 使能 DMA1 时钟
    
    // RX DMA1 通道5
    DMA_InitStruct.DMA_BufferSize = sizeof(uart1RecvData);      // 定义了接收的最大长度
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;             // 串口接收，方向是外设->内存
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;                   // 本次是外设到内存，所以关闭内存到内存
    DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)uart1RecvData;// 内存的基地址，要存储在哪里
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;// 内存数据宽度，按照字节存储
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;        // 内存递增，每次串口收到数据存在内存中，下次收到自动存储在内存的下一个位置
    DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;                  // 正常模式
    DMA_InitStruct.DMA_PeripheralBaseAddr = USART1_BASE + 0x04; // 外设的基地址，串口的数据寄存器
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;    // 外设的数据宽度，按照字节存储，与内存的数据宽度一致
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;   // 接收只有一个数据寄存器 RDR，所以外设地址不递增
    DMA_InitStruct.DMA_Priority = DMA_Priority_High;            // 优先级
    DMA_Init(DMA1_Channel5, &DMA_InitStruct);
    
    // TX DMA1 通道4  
    DMA_InitStruct.DMA_BufferSize = 0;                          // 发送缓冲区的大小，初始化为0不发送
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;             // 发送是方向是外设到内存，外设作为目的地
    DMA_InitStruct.DMA_MemoryBaseAddr =(uint32_t)uart1SendData; // 发送内存地址，从哪里发送
    DMA_Init(DMA1_Channel4, &DMA_InitStruct);
     
    USART_DMACmd(USART1, USART_DMAReq_Tx | USART_DMAReq_Rx, ENABLE);// 使能DMA串口发送和接受请求
    DMA_Cmd(DMA1_Channel5, ENABLE);     // 使能接收
    DMA_Cmd(DMA1_Channel4, DISABLE);    // 禁止发送
}

/* 清除DMA的传输数量寄存器 */
void uart1DmaClear(void)
{
    DMA_Cmd(DMA1_Channel5, DISABLE);    // 关闭 DMA1_Channel5 通道
    DMA_SetCurrDataCounter(DMA1_Channel5, sizeof(uart1RecvData));   // 重新写入要传输的数据数量
    DMA_Cmd(DMA1_Channel5, ENABLE);     // 使能 DMA1_Channel5 通道
}

/* 串口1发送数组 */
void uart1SendArray(uint8_t *arr, uint8_t len)
{
    if(len == 0)	// 判断长度是否有效
      return;
	
	uint8_t sendLen = len>sizeof(uart1SendData) ? sizeof(uart1SendData) : len;	// 防止越界
    while (DMA_GetCurrDataCounter(DMA1_Channel4));  // 检查DMA发送通道内是否还有数据
    if(arr) 
      memcpy(uart1SendData, arr, sendLen);
    
    // DMA发送数据-要先关 设置发送长度 开启DMA
    DMA_Cmd(DMA1_Channel4, DISABLE);
    DMA_SetCurrDataCounter(DMA1_Channel4, sendLen);   // 重新写入要传输的数据数量
    DMA_Cmd(DMA1_Channel4, ENABLE);     // 启动DMA发送  
}
/* 串口1接收数据处理 */
void USART1_ReciveDataHand(void)
{
	 if(uart1RecvFlag == 1)	// 接收到数据
        {
            uart1RecvFlag = 0;  // 接收标志清空
            uart1DmaClear();    // 清空DMA接收通道
          if(memcmp(uart1RecvData,command1,sizeof(command1))==0)//指令1monitor
		  {
			  monitor_flag=1;
		  }
		  else if(memcmp(uart1RecvData,command2,sizeof(command2))==0)//指令2alarm
		  {
			  alarm_flag=1;
		  }
		  else
		  {
			 uart1SendArray(error, sizeof(error));
		  }	  
          memset(uart1RecvData, '\0', sizeof(uart1RecvData)); // 清空接收缓冲区
        }   
        if(uart1SendFlag == 1)
        {
            uart1SendFlag = 0;  // 清空发送标志   
        }
}
