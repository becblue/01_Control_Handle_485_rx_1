#include "sys_manager.h"

// ϵͳ��ʼ��
HAL_StatusTypeDef Sys_Init(void)
{
    // ��ʼ��LED
    LED_Init();
    
    // ��ʼ��ϵͳ״̬
    Sys_Status_Init();
    
    // ��ʼ��OLED
    if(OLED_Init() != HAL_OK)
    {
        return HAL_ERROR;
    }
    
    // ��ʼ��RTCʱ��
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

// ϵͳ����
void Sys_Run(void)
{
    // ����LED״̬
    LED_Process();
    
    // ����ϵͳ״̬
    Sys_Update_Status();
    
    // ������ʾ
    OLED_UpdateDisplay();
    
    // ���¿���ʱ�����
    Update_Idle_Time();
}

// ϵͳ������
void Sys_Error_Handler(void)
{
    // ����ͨ��״̬Ϊ����
    Set_Comm_Status(COMM_STATUS_ERROR);
    
    // ���Ӵ������
    Increment_Error_Count();
    
    // ����LEDΪ����ָʾ״̬
    LED_SetState(0, LED_FAST_BLINK);  // LED1������ʾ����
}

// ϵͳ״̬����
void Sys_Update_Status(void)
{
    static uint32_t last_update = 0;
    uint32_t current_time = HAL_GetTick();
    
    // ÿ100ms����һ��״̬
    if(current_time - last_update >= 100)
    {
        // ����ͨ��״̬
        if(Get_Comm_Status() == COMM_STATUS_WAIT)
        {
            LED_SetState(1, LED_SLOW_BLINK);  // LED2������ʾ�ȴ�ͨ��
        }
        else if(Get_Comm_Status() == COMM_STATUS_OK)
        {
            LED_SetState(1, LED_ON);  // LED2������ʾͨ������
        }
        else
        {
            LED_SetState(1, LED_OFF);  // LED2Ϩ���ʾͨ�Ŵ���
        }
        
        last_update = current_time;
    }
}

// ע�͵���ɾ����RS485��صĴ���
/*
void RS485_Init(void)
{
    // RS485��ʼ������
}

void RS485_Process(void)
{
    // RS485�������
}
*/ 

void SendCommandToDisplay(uint8_t command)
{
    uint8_t commandFrame[4];
    commandFrame[0] = 0xAA;  // ֡ͷ
    commandFrame[1] = 0x01;  // ���ݳ���
    commandFrame[2] = command; // �������ݣ����磬0x01��ʾ��KEY1��
    commandFrame[3] = 0x55;  // ֡β

    // ͨ��USART1��������֡
    HAL_UART_Transmit(&huart1, commandFrame, sizeof(commandFrame), HAL_MAX_DELAY);
}

void SendKeyCommand(uint8_t key_state)
{
    uint8_t commandFrame[5];
    commandFrame[0] = 0xAA;    // ֡ͷ
    commandFrame[1] = 0x02;    // ���ݳ��ȣ�2�ֽڣ�
    commandFrame[2] = 0x01;    // �����루����KEY1��
    commandFrame[3] = key_state;// ״̬��0x01=����0x00=�أ�
    commandFrame[4] = 0x55;    // ֡β

    // ͨ��USART1��������֡
    HAL_UART_Transmit(&huart1, commandFrame, sizeof(commandFrame), HAL_MAX_DELAY);
}

