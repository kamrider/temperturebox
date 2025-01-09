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
    // KEY1 (PE3, 低电平触发) - 用于切换选项或增加温度
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
                
            case MENU_SET_TEMP:
                // 增加目标温度，步进0.5度
                PID.setTemp += 0.5f;
                // 限制最高温度为35度
                if(PID.setTemp > 35.0f) {
                    PID.setTemp = 35.0f;
                }
                break;
        }
        needRefreshMenu = 1;
    }
    
    // KEY2 (PE4, 低电平触发) - 用于切换菜单或减少温度
    if(Key_Scan(KEY2_GPIO_PORT, KEY2_GPIO_PIN))
    {
        switch(currentMenu)
        {
            case MENU_MODE_SELECT:
                currentMenu = MENU_HUMID_SELECT;
                break;
                
            case MENU_HUMID_SELECT:
                currentMenu = MENU_SET_TEMP;
                break;
                
            case MENU_SET_TEMP:
                // 减少目标温度，步进0.5度
                PID.setTemp -= 0.5f;
                // 限制最低温度为15度
                if(PID.setTemp < 15.0f) {
                    PID.setTemp = 15.0f;
                }
                break;
        }
        needRefreshMenu = 1;
    }
    
    // KEY3 (PA0, 高电平触发) - 用于确认并返回主菜单
    if(Key_Scan_High(KEY3_GPIO_PORT, KEY3_GPIO_PIN))
    {
        if(currentMenu != MENU_MAIN) {
            currentMenu = MENU_MAIN;
        }
        needRefreshMenu = 1;
    }
}

void Menu_Display(void)
{
    if(!needRefreshMenu) return;
    
    OLED2_Clear();
    
    switch(currentMenu)
    {
        case MENU_MAIN:
            // 第1行：当前模式
            OLED2_ShowString(1,1,"Mode:");
            if(sysStatus.workMode == MODE_STANDBY)
                OLED2_ShowString(1,6,"Standby");
            else if(sysStatus.workMode == MODE_COOLING)
                OLED2_ShowString(1,6,"Cool");
            else if(sysStatus.workMode == MODE_HEATING)
                OLED2_ShowString(1,6,"Heat");
            
            // 第2行：当前温度
            OLED2_ShowString(2,1,"Temp:");
            OLED2_ShowNum(2,6,(int)temperature,2);         // 使用temperature变量
            OLED2_ShowString(2,8,".");
            OLED2_ShowNum(2,9,(int)((temperature-(int)temperature)*10),1);
            OLED2_ShowString(2,10,"C");
            
            // 第3行：目标温度
            OLED2_ShowString(3,1,"Target:");
            OLED2_ShowNum(3,8,(int)PID.setTemp,2);
            OLED2_ShowString(3,10,".");
            OLED2_ShowNum(3,11,(int)((PID.setTemp-(int)PID.setTemp)*10),1);
            OLED2_ShowString(3,12,"C");
            
            // 第4行：加湿状态
            OLED2_ShowString(4,1,"Humid:");
            OLED2_ShowString(4,7,sysStatus.humidOn ? "ON" : "OFF");
            break;
            
        case MENU_MODE_SELECT:
            OLED2_ShowString(1,1,"Select Mode:");
            if(sysStatus.workMode == MODE_STANDBY)
                OLED2_ShowString(2,1,"-> Standby");
            else if(sysStatus.workMode == MODE_COOLING)
                OLED2_ShowString(2,1,"-> Cool");
            else if(sysStatus.workMode == MODE_HEATING)
                OLED2_ShowString(2,1,"-> Heat");
            break;
            
        case MENU_HUMID_SELECT:
            OLED2_ShowString(1,1,"Humid Control:");
            if(sysStatus.humidOn)
                OLED2_ShowString(2,1,"-> ON");
            else
                OLED2_ShowString(2,1,"-> OFF");
            break;
            
        case MENU_SET_TEMP:
            OLED2_ShowString(1,1,"Set Target:");
            OLED2_ShowNum(2,1,(int)PID.setTemp,2);
            OLED2_ShowString(2,3,".");
            OLED2_ShowNum(2,4,(int)((PID.setTemp-(int)PID.setTemp)*10),1);
            OLED2_ShowString(2,5,"C");
            break;
    }
    
    needRefreshMenu = 0;
}

void Menu_Process(void)
{
    Menu_KeyHandle();
    Menu_Display();
}