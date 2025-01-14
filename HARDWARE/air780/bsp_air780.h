#ifndef __BSP_AIR780_H
#define __BSP_AIR780_H

#include "stm32f10x.h"
#include "bsp_usart.h"  // 使用已有的串口驱动
#include <stdio.h>
#include <string.h>

// MQTT服务器配置
#define MQTT_SERVER      "mqtts.heclouds.com"
#define MQTT_PORT        "1883"
#define MQTT_PUBID       "hCZEHFO5sd"
#define MQTT_DEVID       "tempandshidu"
#define MQTT_TOKEN       "version=2018-10-31&res=products%2FhCZEHFO5sd%2Fdevices%2Ftempandshidu&et=1893817083&method=md5&sign=KRYjHSfhdmNb%2FAcfqaAA2A%3D%3D"

// MQTT主题
#define MQTT_SUB_TOPIC   "$sys/hCZEHFO5sd/tempandshidu/thing/property/post/reply"
#define MQTT_PUB_TOPIC   "$sys/hCZEHFO5sd/tempandshidu/thing/property/post"
#define MQTT_TEMP_PARAM  "temp_value"     // 温度参数名
#define MQTT_HUMID_PARAM "humid_value"    // 湿度参数名

// 函数声明
void AIR780_Init(void);
void AIR780_SendCmd(char* cmd);
uint8_t AIR780_WaitResponse(char* response, uint32_t timeout);
void AIR780_SendData(uint8_t* data, uint16_t size);
uint8_t AIR780_MQTT_Connect(void);
uint8_t AIR780_MQTT_Publish(float temp_value, float humid_value);

extern float humidity;  // 声明外部湿度变量

#endif /* __BSP_AIR780_H */ 