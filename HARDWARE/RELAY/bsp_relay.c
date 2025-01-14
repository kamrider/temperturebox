#include "bsp_relay.h"

void RELAY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 使能GPIOB时钟
    RCC_APB2PeriphClockCmd(RELAY_GPIO_CLK, ENABLE);

    // 配置继电器控制引脚
    GPIO_InitStructure.GPIO_Pin = RELAY_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(RELAY_GPIO_PORT, &GPIO_InitStructure);
    
    // 初始状态设为关闭
    RELAY_OFF();
}

void RELAY_ON(void)
{
    GPIO_SetBits(RELAY_GPIO_PORT, RELAY_GPIO_PIN);
}

void RELAY_OFF(void)
{
    GPIO_ResetBits(RELAY_GPIO_PORT, RELAY_GPIO_PIN);
}

uint8_t RELAY_GetState(void)
{
    return GPIO_ReadOutputDataBit(RELAY_GPIO_PORT, RELAY_GPIO_PIN);
} 