#include "bsp_dht22.h"
#include "delay.h"

static void DHT22_Mode_OUT_PP(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = DHT22_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DHT22_GPIO_PORT, &GPIO_InitStructure);
}

static void DHT22_Mode_IPU(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = DHT22_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(DHT22_GPIO_PORT, &GPIO_InitStructure);
}

void DHT22_Init(void)
{
    RCC_APB2PeriphClockCmd(DHT22_GPIO_CLK, ENABLE);
    DHT22_Mode_OUT_PP();
    GPIO_SetBits(DHT22_GPIO_PORT, DHT22_GPIO_PIN);
}

uint8_t DHT22_Read_Data(float *temperature, float *humidity)
{
    uint8_t data[5] = {0};
    uint8_t i, j;
    
    // 关闭所有中断
    __disable_irq();
    
    // 主机发送开始信号
    DHT22_Mode_OUT_PP();
    GPIO_ResetBits(DHT22_GPIO_PORT, DHT22_GPIO_PIN);
    delay_ms(2);    // 拉低至少1ms，这里用2ms确保稳定
    GPIO_SetBits(DHT22_GPIO_PORT, DHT22_GPIO_PIN);
    delay_us(30);   // 拉高20-40us
    
    // 切换为输入模式，准备接收数据
    DHT22_Mode_IPU();
    
    // 等待DHT22响应，超时检测
    uint16_t timeout = 0;
    while(GPIO_ReadInputDataBit(DHT22_GPIO_PORT, DHT22_GPIO_PIN) == 1)
    {
        timeout++;
        if(timeout > 100) {
            __enable_irq();  // 超时，重新开启中断
            return 1;  // 返回错误
        }
        delay_us(1);
    }
    
    // 等待DHT22的80us低电平响应信号
    timeout = 0;
    while(GPIO_ReadInputDataBit(DHT22_GPIO_PORT, DHT22_GPIO_PIN) == 0)
    {
        timeout++;
        if(timeout > 100) {
            __enable_irq();
            return 1;
        }
        delay_us(1);
    }
    
    // 等待DHT22的80us高电平响应信号
    timeout = 0;
    while(GPIO_ReadInputDataBit(DHT22_GPIO_PORT, DHT22_GPIO_PIN) == 1)
    {
        timeout++;
        if(timeout > 100) {
            __enable_irq();
            return 1;
        }
        delay_us(1);
    }
    
    // 开始接收40位数据
    for(i = 0; i < 5; i++)
    {
        for(j = 0; j < 8; j++)
        {
            // 等待50us低电平开始信号
            timeout = 0;
            while(GPIO_ReadInputDataBit(DHT22_GPIO_PORT, DHT22_GPIO_PIN) == 0)
            {
                timeout++;
                if(timeout > 100) {
                    __enable_irq();
                    return 1;
                }
                delay_us(1);
            }
            
            // 延时40us，判断高电平持续时间
            delay_us(40);
            
            if(GPIO_ReadInputDataBit(DHT22_GPIO_PORT, DHT22_GPIO_PIN) == 1) // 如果还是高电平，说明是1
            {
                data[i] |= (1 << (7-j));
                
                // 等待高电平结束
                timeout = 0;
                while(GPIO_ReadInputDataBit(DHT22_GPIO_PORT, DHT22_GPIO_PIN) == 1)
                {
                    timeout++;
                    if(timeout > 100) {
                        __enable_irq();
                        return 1;
                    }
                    delay_us(1);
                }
            }
        }
    }
    
    // 重新开启中断
    __enable_irq();
    
    // 校验数据
    if((data[0] + data[1] + data[2] + data[3]) == data[4])
    {
        *humidity = ((data[0] << 8) + data[1]) / 10.0;
        if(*humidity > 100.0f) return 1;  // 数据错误检查
        
        *temperature = (((data[2] & 0x7F) << 8) + data[3]) / 10.0;
        if(data[2] & 0x80) *temperature *= -1;
        
        return 0;  // 成功
    }
    
    return 1;  // 校验失败
} 