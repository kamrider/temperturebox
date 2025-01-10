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
    // KEY1 (PE3) - 增加/切换选项
    if(Key_Scan(KEY1_GPIO_PORT, KEY1_GPIO_PIN))
    {
        switch(currentMenu)
        {
            case MENU_MODE_SELECT:
                // 循环切换：待机->制冷->加热
                if(sysStatus.workMode == MODE_STANDBY) {
                    sysStatus.workMode = MODE_COOLING;
                } else if(sysStatus.workMode == MODE_COOLING) {
                    sysStatus.workMode = MODE_HEATING;
                } else {
                    sysStatus.workMode = MODE_STANDBY;
                }
                break;
                
            case MENU_TEMP_SET:
                // 增加温度（仅在制冷/加热模式）
                PID.setTemp += 0.5f;
                if(PID.setTemp > 35.0f) PID.setTemp = 35.0f;
                break;
                
            case MENU_HUMID_SET:
                sysStatus.humidOn = !sysStatus.humidOn;
                break;
        }
        needRefreshMenu = 1;
    }
    
    // KEY2 (PE4) - 减少/返回
    if(Key_Scan(KEY2_GPIO_PORT, KEY2_GPIO_PIN))
    {
        switch(currentMenu)
        {
            case MENU_TEMP_SET:
                // 减少温度
                PID.setTemp -= 0.5f;
                if(PID.setTemp < 15.0f) PID.setTemp = 15.0f;
                break;
        }
        needRefreshMenu = 1;
    }
    
    // KEY3 (PA0) - 确认/下一步
    if(Key_Scan_High(KEY3_GPIO_PORT, KEY3_GPIO_PIN))
    {
        switch(currentMenu)
        {
            case MENU_MAIN:
                currentMenu = MENU_MODE_SELECT;
                break;
                
            case MENU_MODE_SELECT:
                if(sysStatus.workMode == MODE_STANDBY) {
                    currentMenu = MENU_HUMID_SET;  // 待机模式直接到加湿设置
                } else {
                    currentMenu = MENU_TEMP_SET;   // 制冷/加热模式先设置温度
                }
                break;
                
            case MENU_TEMP_SET:
                currentMenu = MENU_HUMID_SET;      // 温度设置完成后到加湿设置
                break;
                
            case MENU_HUMID_SET:
                currentMenu = MENU_MAIN;           // 设置完成返回主菜单
                break;
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
            // 显示当前状态
            OLED2_ShowString(1,1,"Mode:");
            if(sysStatus.workMode == MODE_STANDBY)
                OLED2_ShowString(1,6,"Standby");
            else if(sysStatus.workMode == MODE_COOLING)
                OLED2_ShowString(1,6,"Cool");
            else
                OLED2_ShowString(1,6,"Heat");
            
            OLED2_ShowString(2,1,"Temp:");
            OLED2_ShowNum(2,6,(int)temperature,2);
            OLED2_ShowString(2,8,".");
            OLED2_ShowNum(2,9,(int)((temperature-(int)temperature)*10),1);
            OLED2_ShowString(2,10,"C");
            
            if(sysStatus.workMode != MODE_STANDBY) {
                OLED2_ShowString(3,1,"Target:");
                OLED2_ShowNum(3,8,(int)PID.setTemp,2);
                OLED2_ShowString(3,10,".");
                OLED2_ShowNum(3,11,(int)((PID.setTemp-(int)PID.setTemp)*10),1);
                OLED2_ShowString(3,12,"C");
            }
            
            OLED2_ShowString(4,1,"Humid:");
            OLED2_ShowString(4,7,sysStatus.humidOn ? "ON" : "OFF");
            break;
            
        case MENU_MODE_SELECT:
            OLED2_ShowString(1,1,"Select Mode:");
            OLED2_ShowString(2,1,"->");
            if(sysStatus.workMode == MODE_STANDBY)
                OLED2_ShowString(2,3,"Standby");
            else if(sysStatus.workMode == MODE_COOLING)
                OLED2_ShowString(2,3,"Cool");
            else
                OLED2_ShowString(2,3,"Heat");
            OLED2_ShowString(4,1,"KEY1:Change KEY3:OK");
            break;
            
        case MENU_TEMP_SET:
            OLED2_ShowString(1,1,"Set Temperature");
            OLED2_ShowNum(2,1,(int)PID.setTemp,2);
            OLED2_ShowString(2,3,".");
            OLED2_ShowNum(2,4,(int)((PID.setTemp-(int)PID.setTemp)*10),1);
            OLED2_ShowString(2,5,"C");
            OLED2_ShowString(4,1,"K1:+ K2:- K3:OK");
            break;
            
        case MENU_HUMID_SET:
            OLED2_ShowString(1,1,"Humid Control:");
            OLED2_ShowString(2,1,"->");
            OLED2_ShowString(2,3,sysStatus.humidOn ? "ON" : "OFF");
            OLED2_ShowString(4,1,"KEY1:Change KEY3:OK");
            break;
    }
    
    needRefreshMenu = 0;
}

void Menu_Process(void)
{
    Menu_KeyHandle();
    Menu_Display();
}