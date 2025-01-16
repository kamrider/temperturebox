#ifndef __BSP_AIR780_H
#define __BSP_AIR780_H

#include "stm32f10x.h"
#include "bsp_usart.h"  // 使用已有的串口驱动
#include <stdio.h>
#include <string.h>

// MQTT服务器配置
#define MQTT_SERVER      "mqtts.heclouds.com"
#define MQTT_PORT        "1883"
#define MQTT_PUBID       "KUb3P9N4oJ"
#define MQTT_DEVID       "wwwo"
#define MQTT_TOKEN       "version=2018-10-31&res=products%2FKUb3P9N4oJ%2Fdevices%2Fwwwo&et=1893817083&method=md5&sign=OHoKOl0PYhsOOIFMCqUHzw%3D%3D"

// MQTT主题
#define MQTT_SUB_TOPIC   "$sys/KUb3P9N4oJ/wwwo/thing/property/post/reply"
#define MQTT_PUB_TOPIC   "$sys/KUb3P9N4oJ/wwwo/thing/property/post"
#define MQTT_TEMP_PARAM  "temp_value"     // 温度参数名
#define MQTT_HUMID_PARAM "humidity_value"    // 湿度参数名

// 函数声明
void AIR780_Init(void);
void AIR780_SendCmd(char* cmd);
uint8_t AIR780_WaitResponse(char* response, uint32_t timeout);
void AIR780_SendData(uint8_t* data, uint16_t size);
uint8_t AIR780_MQTT_Connect(void);
uint8_t AIR780_MQTT_Publish(float temp_value, float humid_value);

extern float humidity;  // 声明外部湿度变量

#endif /* __BSP_AIR780_H */ 