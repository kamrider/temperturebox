#ifndef __BSP_RELAY_H
#define __BSP_RELAY_H

#include "stm32f10x.h"

// 继电器引脚定义
#define RELAY_GPIO_PORT    GPIOB
#define RELAY_GPIO_PIN     GPIO_Pin_5
#define RELAY_GPIO_CLK     RCC_APB2Periph_GPIOB

// 继电器控制函数
void RELAY_Init(void);
void RELAY_ON(void);
void RELAY_OFF(void);
uint8_t RELAY_GetState(void);

#endif 