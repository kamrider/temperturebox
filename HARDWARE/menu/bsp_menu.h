#ifndef __BSP_MENU_H
#define __BSP_MENU_H

#include "stm32f10x.h"
#include "oled.h"
#include "OLED2.h"
#include "pid.h"
#include "bsp_key.h"
#include "bsp_ds18b20.h"

// 声明外部变量
extern float temperature;

// 菜单状态枚举
typedef enum {
    MENU_MAIN,         // 主菜单
    MENU_MODE_SELECT,  // 模式选择
    MENU_TEMP_SET,     // 温度设置（仅制冷/加热模式）
    MENU_HUMID_SET,    // 加湿设置
} MenuState;

// 工作模式枚举
typedef enum {
    MODE_STANDBY,    // 待机
    MODE_COOLING,    // 制冷
    MODE_HEATING,    // 加热
    MODE_DEHUMID,    // 除湿
} WorkMode;

// 系统状态结构体
typedef struct {
    WorkMode workMode;    // 当前工作模式
    uint8_t humidOn;      // 加湿开关状态
    float heatingTemp;    // 加热模式的目标温度
    uint8_t fanState;     // 风扇状态
} SystemStatus;

// 函数声明
void Menu_Init(void);
void Menu_Process(void);
void Menu_Display(void);
void Menu_KeyHandle(void);

extern MenuState currentMenu;
extern uint8_t needRefreshMenu;
extern SystemStatus sysStatus;

#define TEMP_THRESHOLD   0.5f    // 温度阈值，避免频繁切换
#define RELAY_DELAY_MS   1000    // 继电器切换最小间隔时间（毫秒）

#endif