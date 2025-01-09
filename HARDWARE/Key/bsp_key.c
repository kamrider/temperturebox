#include "bsp_key.h"  

/**
  * @brief  配置按键用到的I/O口
  * @param  无
  * @retval 无
  */
void Key_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// 使能按键GPIO时钟
	RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK | KEY2_GPIO_CLK | KEY3_GPIO_CLK, ENABLE);
	
	// 配置KEY1 (PE3) 和 KEY2 (PE4) - 低电平触发
	GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  // 上拉输入
	GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_PIN; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStructure);
	
	// 配置KEY3 (PA0) - 高电平触发
	GPIO_InitStructure.GPIO_Pin = KEY3_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  // 下拉输入
	GPIO_Init(KEY3_GPIO_PORT, &GPIO_InitStructure);
}

// PE3和PE4的按键扫描（低电平触发）
uint8_t Key_Scan(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	if(GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == 0)  // 低电平表示按下
	{
		delay_ms(10);  // 消抖
		if(GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == 0)
		{
			while(GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == 0); // 等待按键释放
			return 1;  // 返回1表示按键被按下
		}
	}
	return 0;  // 返回0表示没有按键按下
}

// PA0的按键扫描（高电平触发）
uint8_t Key_Scan_High(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	if(GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == 1)  // 高电平表示按下
	{
		delay_ms(10);  // 消抖
		if(GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == 1)
		{
			while(GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == 1); // 等待按键释放
			return 1;  // 返回1表示按键被按下
		}
	}
	return 0;  // 返回0表示没有按键按下
}
/*********************************************END OF FILE**********************/
