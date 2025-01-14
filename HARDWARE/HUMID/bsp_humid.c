#include "bsp_humid.h"

void HUMID_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 使能GPIO时钟
    RCC_APB2PeriphClockCmd(HUMID_GPIO_CLK, ENABLE);

    // 配置GPIO为推挽输出
    GPIO_InitStructure.GPIO_Pin = HUMID_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(HUMID_GPIO_PORT, &GPIO_InitStructure);

    // 初始化为关闭状态
    HUMID_OFF();
} 