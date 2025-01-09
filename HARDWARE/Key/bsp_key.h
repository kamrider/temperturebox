#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#include "stm32f10x.h"

// 定义按键端口和引脚
#define KEY1_GPIO_PORT    GPIOE
#define KEY1_GPIO_CLK     RCC_APB2Periph_GPIOE
#define KEY1_GPIO_PIN     GPIO_Pin_3

#define KEY2_GPIO_PORT    GPIOE
#define KEY2_GPIO_CLK     RCC_APB2Periph_GPIOE
#define KEY2_GPIO_PIN     GPIO_Pin_4

#define KEY3_GPIO_PORT    GPIOA
#define KEY3_GPIO_CLK     RCC_APB2Periph_GPIOA
#define KEY3_GPIO_PIN     GPIO_Pin_0

// 低电平为按下（PE3, PE4）
#define KEY_ON_LOW   0
#define KEY_OFF_LOW  1

// 高电平为按下（PA0）
#define KEY_ON_HIGH  1
#define KEY_OFF_HIGH 0

void Key_GPIO_Config(void);
uint8_t Key_Scan(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
uint8_t Key_Scan_High(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

#endif

