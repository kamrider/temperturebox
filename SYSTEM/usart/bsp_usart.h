#ifndef __BSP_USART_H
#define __BSP_USART_H

#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>

// 串口1相关宏定义
#define USART1_MAX_SEND_LEN     400  
#define USART1_MAX_RECV_LEN     400

// 串口2相关宏定义  
#define USART2_MAX_SEND_LEN     400  
#define USART2_MAX_RECV_LEN     400

// 串口1-USART1
#define DEBUG_USARTx                   USART1
#define DEBUG_USART_CLK                RCC_APB2Periph_USART1
#define DEBUG_USART_APBxClkCmd         RCC_APB2PeriphClockCmd
#define DEBUG_USART_BAUDRATE           115200

// USART1 GPIO 定义
#define DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define DEBUG_USART_TX_GPIO_PORT       GPIOA   
#define DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_9
#define DEBUG_USART_RX_GPIO_PORT       GPIOA
#define DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_10

// 串口2-USART2
#define DATA_USARTx                    USART2
#define DATA_USART_CLK                 RCC_APB1Periph_USART2
#define DATA_USART_APBxClkCmd          RCC_APB1PeriphClockCmd
#define DATA_USART_BAUDRATE            115200

// USART2 GPIO 定义
#define DATA_USART_GPIO_CLK            (RCC_APB2Periph_GPIOA)
#define DATA_USART_GPIO_APBxClkCmd     RCC_APB2PeriphClockCmd
    
#define DATA_USART_TX_GPIO_PORT        GPIOA   
#define DATA_USART_TX_GPIO_PIN         GPIO_Pin_2
#define DATA_USART_RX_GPIO_PORT        GPIOA
#define DATA_USART_RX_GPIO_PIN         GPIO_Pin_3

// 串口相关变量声明
extern uint8_t USART1_RX_BUF[USART1_MAX_RECV_LEN];
extern uint8_t USART2_RX_BUF[USART2_MAX_RECV_LEN];
extern uint16_t USART1_RX_STA;
extern uint16_t USART2_RX_STA;

// 函数声明
void USART1_Config(void);
void USART2_Config(void);
void USART1_SendByte(uint8_t ch);
void USART2_SendByte(uint8_t ch);
void USART1_SendString(char *str);
void USART2_SendString(char *str);
void USART2_CLR_Buf(void);
void USART2_Init(uint32_t baudrate);
void USART_Config(void);

#endif
