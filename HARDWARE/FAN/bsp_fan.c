#include "bsp_fan.h"

void FAN_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 使能GPIO时钟
    RCC_APB2PeriphClockCmd(FAN_GPIO_CLK, ENABLE);

    // 配置GPIO为推挽输出
    GPIO_InitStructure.GPIO_Pin = FAN_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(FAN_GPIO_PORT, &GPIO_InitStructure);

    // 初始化为关闭状态
    FAN_OFF();
}

void FAN_ON(void)
{
    GPIO_SetBits(FAN_GPIO_PORT, FAN_GPIO_PIN);
}

void FAN_OFF(void)
{
    GPIO_ResetBits(FAN_GPIO_PORT, FAN_GPIO_PIN);
}

uint8_t FAN_GetState(void)
{
    return GPIO_ReadOutputDataBit(FAN_GPIO_PORT, FAN_GPIO_PIN);
}