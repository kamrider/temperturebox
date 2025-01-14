#include "bsp_air780.h"
#include "delay.h"

void AIR780_Init(void)
{
    delay_ms(1000);
    
    // 基本AT测试
    printf("Testing AT...\r\n");
    AIR780_SendCmd("AT\r\n");
    if(AIR780_WaitResponse("OK\r\n", 1000) != 1) {
        printf("AT test failed\r\n");
        return;
    }
    
    // 检查信号强度
    printf("Checking signal strength...\r\n");
    AIR780_SendCmd("AT+CSQ\r\n");
    if(AIR780_WaitResponse("OK\r\n", 3000) != 1) {
        printf("Signal strength check failed\r\n");
        return;
    }
    delay_ms(100);
    
    // 检查SIM卡状态
    printf("Checking SIM card status...\r\n");
    AIR780_SendCmd("AT+CPIN?\r\n");
    if(AIR780_WaitResponse("READY", 3000) != 1) {
        printf("SIM card check failed\r\n");
        return;
    }
    delay_ms(100);
    
    // 检查网络注册状态
    printf("Checking network registration...\r\n");
    AIR780_SendCmd("AT+CREG?\r\n");
    if(AIR780_WaitResponse("+CREG: 0,1", 3000) != 1) {
        printf("Network registration check failed\r\n");
        return;
    }
    delay_ms(100);
    
    // 检查网络附着
    printf("Checking network attachment...\r\n");
    AIR780_SendCmd("AT+CGATT?\r\n");
    if(AIR780_WaitResponse("+CGATT: 1", 3000) != 1) {
        printf("Network attachment check failed\r\n");
        return;
    }
    delay_ms(100);
    
//    // 设置APN
//    printf("Setting up APN...\r\n");
//    AIR780_SendCmd("AT+CSTT\r\n");
//    if(AIR780_WaitResponse("OK\r\n", 3000) != 1) {
//        printf("APN setup failed\r\n");
//        return;
//    }
//    delay_ms(100);
//    
//    // 激活移动场景
//    printf("Activating wireless connection...\r\n");
//    AIR780_SendCmd("AT+CIICR\r\n");
//    if(AIR780_WaitResponse("OK\r\n", 3000) != 1) {
//        printf("Wireless activation failed\r\n");
//        return;
//    }
//    delay_ms(100);
    
    // 获取PDP地址
    printf("Getting PDP address...\r\n");
    AIR780_SendCmd("AT+CGPADDR=1\r\n");
    if(AIR780_WaitResponse("+CGPADDR", 3000) != 1) {
        printf("PDP address acquisition failed\r\n");
        return;
    }
    delay_ms(1000);
    
    // 连接MQTT服务器
    if(AIR780_MQTT_Connect() != 1) {
        printf("MQTT connection failed\r\n");
        return;
    }
    
    printf("AIR780 initialization completed successfully\r\n");
}

void AIR780_SendCmd(char* cmd)
{
    USART2_SendString(cmd);
}

uint8_t AIR780_WaitResponse(char* response, uint32_t timeout)
{
    uint32_t timeCount = 0;
    USART2_CLR_Buf();  // 清空接收缓冲区
    
    while(timeCount < timeout)
    {
        if(strstr((char*)USART2_RX_BUF, response) != NULL)
        {
            // 通过串口1打印接收到的数据
            printf("AIR780 Response: %s\r\n", USART2_RX_BUF);
            return 1;
        }
        delay_ms(10);
        timeCount += 10;
    }
    return 0;
}

void AIR780_SendData(uint8_t* data, uint16_t size)
{
    while(size--)
    {
        USART2_SendByte(*data++);
    }
}

uint8_t AIR780_MQTT_Connect(void)
{
    char mqtt_cmd[512];
    uint8_t retry_count = 0;
    const uint8_t MAX_RETRY = 3;
    
    printf("Connecting to MQTT server...\r\n");
    
    // 配置MQTT参数 AT+MCONFIG
    sprintf(mqtt_cmd, "AT+MCONFIG=\"%s\",\"%s\",\"%s\"\r\n", 
            MQTT_DEVID, MQTT_PUBID, MQTT_TOKEN);
    AIR780_SendCmd(mqtt_cmd);
    if(AIR780_WaitResponse("OK\r\n", 10000) != 1) {
        printf("MQTT config failed\r\n");
        return 0;
    }
    delay_ms(1000);  // 增加延时
    
    while(retry_count < MAX_RETRY) {
        // 连接服务器 AT+MIPSTART
        printf("Attempting to connect to server (attempt %d/%d)...\r\n", 
               retry_count + 1, MAX_RETRY);
               
        sprintf(mqtt_cmd, "AT+MIPSTART=\"%s\",%s\r\n", 
                MQTT_SERVER, MQTT_PORT);
        AIR780_SendCmd(mqtt_cmd);
        
        if(AIR780_WaitResponse("CONNECT OK\r\n", 15000) == 1) {  // 增加超时时间
            printf("Server connection successful\r\n");
            break;
        }
        
        retry_count++;
        if(retry_count < MAX_RETRY) {
            printf("Connection failed, retrying in 5 seconds...\r\n");
            delay_ms(5000);
        }
    }
    
    if(retry_count >= MAX_RETRY) {
        printf("Server connection failed after %d attempts\r\n", MAX_RETRY);
        return 0;
    }
    
    delay_ms(1000);  // 增加延时
    
    // MQTT连接 AT+MCONNECT
    printf("Establishing MQTT connection...\r\n");
    AIR780_SendCmd("AT+MCONNECT=1,120\r\n");
    if(AIR780_WaitResponse("CONNACK OK\r\n", 10000) != 1) {
        printf("MQTT connect failed\r\n");
        return 0;
    }
    
    delay_ms(1000);  // 增加延时
    
    // 订阅主题 AT+MSUB
    printf("Subscribing to topic...\r\n");
    sprintf(mqtt_cmd, "AT+MSUB=\"%s\",0\r\n", MQTT_SUB_TOPIC);
    AIR780_SendCmd(mqtt_cmd);
    if(AIR780_WaitResponse("SUBACK\r\n", 5000) != 1) {
        printf("Topic subscribe failed\r\n");
        return 0;
    }
    
    printf("MQTT connected successfully\r\n");
    return 1;
}

uint8_t AIR780_MQTT_Publish(float temp_value, float humid_value)
{
    char mqtt_cmd[512];

    
    // 构建包含温度和湿度的JSON数据
    sprintf(mqtt_cmd, "AT+MPUB=\"%s\",0,0,\"{\\22id\\22:\\22123\\22,\\22params\\22:{\\22%s\\22:{\\22value\\22:%.2f},\\22%s\\22:{\\22value\\22:%.1f}}}\"\r\n",
            MQTT_PUB_TOPIC,
            MQTT_TEMP_PARAM,
            temp_value,
            MQTT_HUMID_PARAM,
            humid_value);
            
    AIR780_SendCmd(mqtt_cmd);
    if(AIR780_WaitResponse("OK\r\n", 5000) != 1) {
        printf("Publish failed\r\n");
        return 0;
    }
    
    return 1;
} 