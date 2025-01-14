#ifndef __BSP_HUMID_H
#define __BSP_HUMID_H

#include "stm32f10x.h"

// 加湿器引脚定义
#define HUMID_GPIO_PORT    GPIOA
#define HUMID_GPIO_PIN     GPIO_Pin_4
#define HUMID_GPIO_CLK     RCC_APB2Periph_GPIOA

// 控制宏定义
#define HUMID_ON()        GPIO_SetBits(HUMID_GPIO_PORT, HUMID_GPIO_PIN)
#define HUMID_OFF()       GPIO_ResetBits(HUMID_GPIO_PORT, HUMID_GPIO_PIN)
#define HUMID_GetState()  GPIO_ReadOutputDataBit(HUMID_GPIO_PORT, HUMID_GPIO_PIN)

// 函数声明
void HUMID_Init(void);

#endif 