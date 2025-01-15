#ifndef __BSP_FAN_H
#define __BSP_FAN_H

#include "stm32f10x.h"

// 风扇引脚定义
#define FAN_GPIO_PORT    GPIOC
#define FAN_GPIO_PIN     GPIO_Pin_2
#define FAN_GPIO_CLK     RCC_APB2Periph_GPIOC

// 函数声明
void FAN_Init(void);
void FAN_ON(void);
void FAN_OFF(void);
uint8_t FAN_GetState(void);

#endif