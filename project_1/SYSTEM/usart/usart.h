#ifndef __USART_H__
#define __USART_H__

#include "stm32f10x.h"
#include <stdint.h>

#define UART1_TX_PORT   GPIOA
#define UART1_TX_PIN    GPIO_Pin_9
#define UART1_RX_PORT   GPIOA
#define UART1_RX_PIN    GPIO_Pin_10
#define UART1_EN_PORT   GPIOD
#define UART1_EN_PIN    GPIO_Pin_1
#define BAUD_RATE       (115200)
#define USART_RX_BUF_SIZE       128
#define monitor_disable			0

extern uint8_t monitor_flag;
extern uint8_t RX_flag;
extern uint8_t alarm_flag;
extern uint8_t uart1RecvData[32];
extern uint8_t uart1RecvFlag;
extern uint8_t uart1RecvLen;
extern uint8_t uart1SendFlag;


void Uart1GpioInit(void);
void Uart1Config(void);
void uart1DmaClear(void);
void uart1SendArray(uint8_t *arr, uint8_t len);
void USART1_ReciveDataHand(void);
#endif

