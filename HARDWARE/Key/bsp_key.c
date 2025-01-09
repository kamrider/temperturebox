#include "bsp_key.h"  

/**
  * @brief  ���ð����õ���I/O��
  * @param  ��
  * @retval ��
  */
void Key_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// ʹ�ܰ���GPIOʱ��
	RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK | KEY2_GPIO_CLK | KEY3_GPIO_CLK, ENABLE);
	
	// ����KEY1 (PE3) �� KEY2 (PE4) - �͵�ƽ����
	GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  // ��������
	GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_PIN; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStructure);
	
	// ����KEY3 (PA0) - �ߵ�ƽ����
	GPIO_InitStructure.GPIO_Pin = KEY3_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  // ��������
	GPIO_Init(KEY3_GPIO_PORT, &GPIO_InitStructure);
}

// PE3��PE4�İ���ɨ�裨�͵�ƽ������
uint8_t Key_Scan(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	if(GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == 0)  // �͵�ƽ��ʾ����
	{
		delay_ms(10);  // ����
		if(GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == 0)
		{
			while(GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == 0); // �ȴ������ͷ�
			return 1;  // ����1��ʾ����������
		}
	}
	return 0;  // ����0��ʾû�а�������
}

// PA0�İ���ɨ�裨�ߵ�ƽ������
uint8_t Key_Scan_High(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	if(GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == 1)  // �ߵ�ƽ��ʾ����
	{
		delay_ms(10);  // ����
		if(GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == 1)
		{
			while(GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == 1); // �ȴ������ͷ�
			return 1;  // ����1��ʾ����������
		}
	}
	return 0;  // ����0��ʾû�а�������
}
/*********************************************END OF FILE**********************/
