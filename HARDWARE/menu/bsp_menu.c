#include "bsp_menu.h"

MenuState currentMenu = MENU_MAIN;
uint8_t needRefreshMenu = 1;
SystemStatus sysStatus = {MODE_STANDBY, 0};  // 初始化为待机模式，加湿关闭

void Menu_Init(void)
{
    currentMenu = MENU_MAIN;
    needRefreshMenu = 1;
    sysStatus.workMode = MODE_STANDBY;  // 初始化为待机模式
    sysStatus.humidOn = 0;              // 初始化加湿为关闭状态
}

void Menu_KeyHandle(void)
{
    // KEY1 (PE3, 低电平触发)
    if(Key_Scan(KEY1_GPIO_PORT, KEY1_GPIO_PIN))
    {
        switch(currentMenu)
        {
            case MENU_MAIN:
                currentMenu = MENU_MODE_SELECT;
                break;
                
            case MENU_MODE_SELECT:
                if(sysStatus.workMode == MODE_STANDBY) {
                    sysStatus.workMode = MODE_COOLING;
                } else if(sysStatus.workMode == MODE_COOLING) {
                    sysStatus.workMode = MODE_HEATING;
                } else {
                    sysStatus.workMode = MODE_STANDBY;
                }
                break;
                
            case MENU_HUMID_SELECT:
                sysStatus.humidOn = !sysStatus.humidOn;
                break;
        }
        needRefreshMenu = 1;
    }
    
    // KEY2 (PE4, 低电平触发)
    if(Key_Scan(KEY2_GPIO_PORT, KEY2_GPIO_PIN))
    {
        if(currentMenu == MENU_MODE_SELECT) {
            currentMenu = MENU_HUMID_SELECT;
        }
        needRefreshMenu = 1;
    }
    
    // KEY3 (PA0, 高电平触发)
    if(Key_Scan_High(KEY3_GPIO_PORT, KEY3_GPIO_PIN))
    {
        if(currentMenu == MENU_MODE_SELECT || currentMenu == MENU_HUMID_SELECT) {
            currentMenu = MENU_MAIN;  // 确认选择，返回主菜单
        }
        needRefreshMenu = 1;
    }
}

void Menu_Display(void)
{
    if(!needRefreshMenu) return;
    
    OLED_Clear();
    OLED2_Clear();
    
    switch(currentMenu)
    {
        case MENU_MAIN:
            OLED_ShowString(0, 0, "Mode:", 16);
            // 显示当前模式
            if(sysStatus.workMode == MODE_STANDBY)
                OLED_ShowString(48, 0, "Standby", 16);
            else if(sysStatus.workMode == MODE_COOLING)
                OLED_ShowString(48, 0, "Cool", 16);
            else if(sysStatus.workMode == MODE_HEATING)
                OLED_ShowString(48, 0, "Heat", 16);
            
            // 显示加湿状态
            if(sysStatus.humidOn)
                OLED_ShowString(0, 16, "Humid: ON", 16);
            else
                OLED_ShowString(0, 16, "Humid: OFF", 16);
            break;
            
        case MENU_MODE_SELECT:
            OLED_ShowString(0, 0, "Select Mode:", 16);
            if(sysStatus.workMode == MODE_STANDBY)
                OLED_ShowString(0, 16, "Standby ->", 16);
            else if(sysStatus.workMode == MODE_COOLING)
                OLED_ShowString(0, 16, "Cool ->", 16);
            else if(sysStatus.workMode == MODE_HEATING)
                OLED_ShowString(0, 16, "Heat ->", 16);
            break;
            
        case MENU_HUMID_SELECT:
            OLED_ShowString(0, 0, "Humid Control:", 16);
            if(sysStatus.humidOn)
                OLED_ShowString(0, 16, "ON ->", 16);
            else
                OLED_ShowString(0, 16, "OFF ->", 16);
            break;
    }
    
    OLED_Refresh();
    needRefreshMenu = 0;
}

void Menu_Process(void)
{
    Menu_KeyHandle();
    Menu_Display();
}