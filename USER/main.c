//              ----------------------------------------------------------------
//              屏幕是七脚0.96寸屏幕，采用SPI通信
//              GND    电源地
//              VCC  接5V或3.3v电源
//              D0   接PG12（SCL）
//              D1   接PD5（SDA）
//              RES  接PD4
//              DC   接PD15
//              CS   接PD1    
//              通过定时器3，输出比较通道1输出可控PWM信号
//              ----------------------------------------------------------------
#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "bmp.h"
#include "led.h"
#include "bsp_SysTick.h"
#include "bsp_ds18b20.h"
#include "bsp_key.h"
#include "bsp_GeneralTim.h" 
#include "pid.h"
#include "timer.h"
#include "bsp_usart.h"
#include "OLED2.h"
#include "bsp_air780.h"
#include "bsp_menu.h"
#include "bsp_dht22.h"


extern uint16_t CCR1_Val;
uint16_t Kms10;
uint16_t Kms500;
extern float temperature;
extern float iSetVal;
//extern float Temp[0],Temp[1],Temp[2];
float humidity = 0.0f;

void Isr_Init()
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  NVIC_InitStructure.NVIC_IRQChannel =TIM4_IRQn;// TIM4_IRQChannel; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init (&NVIC_InitStructure); 


  NVIC_InitStructure.NVIC_IRQChannel =TIM3_IRQn;// TIM3_IRQChannel; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init (&NVIC_InitStructure);
}


int main(void)
{
	delay_init();
	// OLED_Init();  // 不再使用OLED1
	OLED2_Init();    // 只使用OLED2
	Key_GPIO_Config();
	USART_Config();
	USART2_Init(115200);
	Timer4_init();
	TIM4_IRQHandler();
	DS18B20_Init();
	PID_Init();
	Isr_Init();
	DHT22_Init();  // 添加DHT22初始化
//	AIR780_Init();
//	delay_ms(2000);  // 给模块更多启动时间
	  Menu_Init();
	while(1)
	{	
//		if(Key_Scan(GPIOA,GPIO_Pin_0))
//			PID.setTemp += 0.5;
//		
//		if(Key_Scan(GPIOC,GPIO_Pin_13))
//			PID.setTemp -= 0.5;

//		// 获取温度并进行PID控制
		temperature = DS18B20_GetTemp_SkipRom();
		if(DHT22_Read_Data(&temperature, &humidity) == 0) {
			// 显示湿度
			OLED2_ShowString(3,1,"Humidity:");
			OLED2_ShowNum(3,10,(int)humidity,2);
			OLED2_ShowString(3,12,".");
			OLED2_ShowNum(3,13,(int)((humidity-(int)humidity)*10),1);
			OLED2_ShowString(3,14,"%");
		} else {
			// 显示错误信息
			OLED2_ShowString(3,1,"DHT22 Error    ");
		}
		
//			        Menu_KeyHandle();
//        Menu_Display();	
		
		
//		// 在OLED2上显示实时温度和目标温度
//		OLED2_ShowString(1,1,"Temp:");                 // 显示当前温度标签
//		OLED2_ShowNum(1,6,(int)temperature,2);         // 显示整数部分
//		OLED2_ShowString(1,8,".");                     // 显示小数点
//		OLED2_ShowNum(1,9,(int)((temperature-(int)temperature)*10),1); // 显示小数部分
//		OLED2_ShowString(1,10,"C");                    // 显示摄氏度符号
//		
//		OLED2_ShowString(2,1,"Target:");               // 显示目标温度标签
//		OLED2_ShowNum(2,8,(int)PID.setTemp,2);        // 显示整数部分
//		OLED2_ShowString(2,10,".");                    // 显示小数点
//		OLED2_ShowNum(2,11,(int)((PID.setTemp-(int)PID.setTemp)*10),1); // 显示小数部分
//		OLED2_ShowString(2,12,"C");                    // 显示摄氏度符号
//		
//		// 原有的OLED显示代码保持不变
//		OLED_ShowString(0,0,"Temperatrue:",16);
//		OLED_ShowFloatNumber(0,16,temperature,16);
//		OLED_ShowString(44,16,"degrees",16);				
//		OLED_ShowString(0,32,"Target temp:",16);
//		OLED_ShowFloatNumber(0,48,PID.setTemp,16);
//		OLED_ShowString(33,48,"degrees",16);	
//		OLED_Refresh();
//		
//		PID_Calculate();
//		CCR1_Val = PID.OUT;  // PID.OUT值越大，制冷越强

//		GENERAL_TIM_Init();
//		printf ( "%.2f,%.2f,%.2f,%.2f,%.2f,%.d\n",PID.setTemp,DS18B20_GetTemp_SkipRom (),PID.t1,PID.t2,PID.t3,CCR1_Val);
//		
//		if(AIR780_MQTT_Publish(temperature) != 1)
//		{
//			printf("MQTT发送失败\r\n");
//			AIR780_Init();
//		}
		
		delay_ms(50);
		// 处理按键和菜单
        Menu_KeyHandle();
        
        // 强制刷新主菜单的温度显示
        if(currentMenu == MENU_MAIN || currentMenu == MENU_TEMP_SET) {
            needRefreshMenu = 1;
        }
        Menu_Display();
        
        // 根据系统状态调整PID控制
        if(sysStatus.workMode == MODE_COOLING) {
            PID_Calculate();
            CCR1_Val = PID.OUT;
        }
        else if(sysStatus.workMode == MODE_HEATING) {
            PID_Calculate();
            CCR1_Val = 500 - PID.OUT;
        }
        else {  // MODE_STANDBY
            CCR1_Val = 0;
        }

        GENERAL_TIM_Init();
        
        // 调试输出
        printf("%.2f,%.2f,%.2f,%.2f,%.2f,%d,%d,%d,%.1f\n", 
            PID.setTemp,
            temperature,
            PID.t1,
            PID.t2,
            PID.t3,
            CCR1_Val,
            sysStatus.workMode,
            sysStatus.humidOn,
            humidity    // 添加湿度显示
        );
	}
}

