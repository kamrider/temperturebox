//              ----------------------------------------------------------------
//              ��Ļ���߽�0.96����Ļ������SPIͨ��
//              GND    ��Դ��
//              VCC  ��5V��3.3v��Դ
//              D0   ��PG12��SCL��
//              D1   ��PD5��SDA��
//              RES  ��PD4
//              DC   ��PD15
//              CS   ��PD1    
//              ͨ����ʱ��3������Ƚ�ͨ��1����ɿ�PWM�ź�
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
	// OLED_Init();  // ����ʹ��OLED1
	OLED2_Init();    // ֻʹ��OLED2
	Key_GPIO_Config();
	USART_Config();
	USART2_Init(115200);
	Timer4_init();
	TIM4_IRQHandler();
	DS18B20_Init();
	PID_Init();
	Isr_Init();
	DHT22_Init();  // ���DHT22��ʼ��
//	AIR780_Init();
//	delay_ms(2000);  // ��ģ���������ʱ��
	  Menu_Init();
	while(1)
	{	
//		if(Key_Scan(GPIOA,GPIO_Pin_0))
//			PID.setTemp += 0.5;
//		
//		if(Key_Scan(GPIOC,GPIO_Pin_13))
//			PID.setTemp -= 0.5;

//		// ��ȡ�¶Ȳ�����PID����
		temperature = DS18B20_GetTemp_SkipRom();
		if(DHT22_Read_Data(&temperature, &humidity) == 0) {
			// ��ʾʪ��
			OLED2_ShowString(3,1,"Humidity:");
			OLED2_ShowNum(3,10,(int)humidity,2);
			OLED2_ShowString(3,12,".");
			OLED2_ShowNum(3,13,(int)((humidity-(int)humidity)*10),1);
			OLED2_ShowString(3,14,"%");
		} else {
			// ��ʾ������Ϣ
			OLED2_ShowString(3,1,"DHT22 Error    ");
		}
		
//			        Menu_KeyHandle();
//        Menu_Display();	
		
		
//		// ��OLED2����ʾʵʱ�¶Ⱥ�Ŀ���¶�
//		OLED2_ShowString(1,1,"Temp:");                 // ��ʾ��ǰ�¶ȱ�ǩ
//		OLED2_ShowNum(1,6,(int)temperature,2);         // ��ʾ��������
//		OLED2_ShowString(1,8,".");                     // ��ʾС����
//		OLED2_ShowNum(1,9,(int)((temperature-(int)temperature)*10),1); // ��ʾС������
//		OLED2_ShowString(1,10,"C");                    // ��ʾ���϶ȷ���
//		
//		OLED2_ShowString(2,1,"Target:");               // ��ʾĿ���¶ȱ�ǩ
//		OLED2_ShowNum(2,8,(int)PID.setTemp,2);        // ��ʾ��������
//		OLED2_ShowString(2,10,".");                    // ��ʾС����
//		OLED2_ShowNum(2,11,(int)((PID.setTemp-(int)PID.setTemp)*10),1); // ��ʾС������
//		OLED2_ShowString(2,12,"C");                    // ��ʾ���϶ȷ���
//		
//		// ԭ�е�OLED��ʾ���뱣�ֲ���
//		OLED_ShowString(0,0,"Temperatrue:",16);
//		OLED_ShowFloatNumber(0,16,temperature,16);
//		OLED_ShowString(44,16,"degrees",16);				
//		OLED_ShowString(0,32,"Target temp:",16);
//		OLED_ShowFloatNumber(0,48,PID.setTemp,16);
//		OLED_ShowString(33,48,"degrees",16);	
//		OLED_Refresh();
//		
//		PID_Calculate();
//		CCR1_Val = PID.OUT;  // PID.OUTֵԽ������Խǿ

//		GENERAL_TIM_Init();
//		printf ( "%.2f,%.2f,%.2f,%.2f,%.2f,%.d\n",PID.setTemp,DS18B20_GetTemp_SkipRom (),PID.t1,PID.t2,PID.t3,CCR1_Val);
//		
//		if(AIR780_MQTT_Publish(temperature) != 1)
//		{
//			printf("MQTT����ʧ��\r\n");
//			AIR780_Init();
//		}
		
		delay_ms(50);
		// �������Ͳ˵�
        Menu_KeyHandle();
        
        // ǿ��ˢ�����˵����¶���ʾ
        if(currentMenu == MENU_MAIN || currentMenu == MENU_TEMP_SET) {
            needRefreshMenu = 1;
        }
        Menu_Display();
        
        // ����ϵͳ״̬����PID����
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
        
        // �������
        printf("%.2f,%.2f,%.2f,%.2f,%.2f,%d,%d,%d,%.1f\n", 
            PID.setTemp,
            temperature,
            PID.t1,
            PID.t2,
            PID.t3,
            CCR1_Val,
            sysStatus.workMode,
            sysStatus.humidOn,
            humidity    // ���ʪ����ʾ
        );
	}
}

