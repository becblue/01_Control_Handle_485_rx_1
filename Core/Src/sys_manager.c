#include "sys_manager.h"

// 系统初始化
HAL_StatusTypeDef Sys_Init(void)
{
    // 初始化LED
    LED_Init();
    
    // 初始化系统状态
    Sys_Status_Init();
    
    // 初始化OLED
    if(OLED_Init() != HAL_OK)
    {
        return HAL_ERROR;
    }
    
    // 初始化RTC时间
    RTC_TimeTypeDef sTime = {0};
    RTC_DateTypeDef sDate = {0};
    
    sTime.Hours = 12;
    sTime.Minutes = 0;
    sTime.Seconds = 0;
    
    if(HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
    {
        return HAL_ERROR;
    }
    
    sDate.Year = 25;
    sDate.Month = 1;
    sDate.Date = 1;
    
    if(HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
    {
        return HAL_ERROR;
    }
    
    return HAL_OK;
}

// 系统运行
void Sys_Run(void)
{
    // 处理LED状态
    LED_Process();
    
    // 更新系统状态
    Sys_Update_Status();
    
    // 更新显示
    OLED_UpdateDisplay();
    
    // 更新空闲时间计数
    Update_Idle_Time();
}

// 系统错误处理
void Sys_Error_Handler(void)
{
    // 设置通信状态为错误
    Set_Comm_Status(COMM_STATUS_ERROR);
    
    // 增加错误计数
    Increment_Error_Count();
    
    // 设置LED为错误指示状态
    LED_SetState(0, LED_FAST_BLINK);  // LED1快闪表示错误
}

// 系统状态更新
void Sys_Update_Status(void)
{
    static uint32_t last_update = 0;
    uint32_t current_time = HAL_GetTick();
    
    // 每100ms更新一次状态
    if(current_time - last_update >= 100)
    {
        // 更新通信状态
        if(Get_Comm_Status() == COMM_STATUS_WAIT)
        {
            LED_SetState(1, LED_SLOW_BLINK);  // LED2慢闪表示等待通信
        }
        else if(Get_Comm_Status() == COMM_STATUS_OK)
        {
            LED_SetState(1, LED_ON);  // LED2常亮表示通信正常
        }
        else
        {
            LED_SetState(1, LED_OFF);  // LED2熄灭表示通信错误
        }
        
        last_update = current_time;
    }
}

// 注释掉或删除与RS485相关的代码
/*
void RS485_Init(void)
{
    // RS485初始化代码
}

void RS485_Process(void)
{
    // RS485处理代码
}
*/ 

void SendCommandToDisplay(uint8_t command)
{
    uint8_t commandFrame[4];
    commandFrame[0] = 0xAA;  // 帧头
    commandFrame[1] = 0x01;  // 数据长度
    commandFrame[2] = command; // 数据内容（例如，0x01表示打开KEY1）
    commandFrame[3] = 0x55;  // 帧尾

    // 通过USART1发送数据帧
    HAL_UART_Transmit(&huart1, commandFrame, sizeof(commandFrame), HAL_MAX_DELAY);
}

void SendKeyCommand(uint8_t key_state)
{
    uint8_t commandFrame[5];
    commandFrame[0] = 0xAA;    // 帧头
    commandFrame[1] = 0x02;    // 数据长度（2字节）
    commandFrame[2] = 0x01;    // 功能码（控制KEY1）
    commandFrame[3] = key_state;// 状态（0x01=开，0x00=关）
    commandFrame[4] = 0x55;    // 帧尾

    // 通过USART1发送数据帧
    HAL_UART_Transmit(&huart1, commandFrame, sizeof(commandFrame), HAL_MAX_DELAY);
}

