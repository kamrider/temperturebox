#ifndef __BSP_DHT22_H
#define __BSP_DHT22_H

#include "stm32f10x.h"

// DHT22 引脚定义
#define DHT22_GPIO_PORT    GPIOA
#define DHT22_GPIO_PIN     GPIO_Pin_5
#define DHT22_GPIO_CLK     RCC_APB2Periph_GPIOA

// 函数声明
void DHT22_Init(void);
uint8_t DHT22_Read_Data(float *temperature, float *humidity);

#endif 