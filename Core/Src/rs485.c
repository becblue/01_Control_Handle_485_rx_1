#include "rs485.h"
#include "oled.h"     // ���OLED��غ�������
#include "led.h"      // ���LED��ض���
#include "key.h"      // ��Ӱ�����ض���
#include <stdio.h>    // ���sprintf��������
#include <string.h>   // ���strlen��������

/* LED�Ͱ�����ض��� */
#ifndef LED_R
#define LED_R    0    // ��ɫLED
#define LED_G    1    // ��ɫLED
#define LED_B    2    // ��ɫLED
#define LED_ON   0    // LED�������͵�ƽ��Ч��
#define LED_OFF  1    // LEDϨ��
#endif

#ifndef KEY1
#define KEY1    0    // KEY1����
#define KEY2    1    // KEY2����
#endif

/* CRC ��λ�ֽ�ֵ�� */
static const uint8_t auchCRCHi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};

/* CRC ��λ�ֽ�ֵ�� */
static const uint8_t auchCRCLo[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04,
    0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8,
    0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
    0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 0x11, 0xD1, 0xD0, 0x10,
    0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
    0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38,
    0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C,
    0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0,
    0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
    0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
    0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C,
    0xB4, 0x74, 0x75, 0xB5, 0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
    0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54,
    0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98,
    0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};

/* Modbus�Ĵ�������Ȧ���� */
#define REG_HOLDING_START    0       // ���ּĴ�����ʼ��ַ
#define REG_HOLDING_END     15       // ���ּĴ���������ַ
#define REG_HOLDING_NREGS   16       // ���ּĴ�������

#define REG_INPUT_START     0       // ����Ĵ�����ʼ��ַ
#define REG_INPUT_END      15       // ����Ĵ���������ַ
#define REG_INPUT_NREGS    16       // ����Ĵ�������

#define COIL_START         0       // ��Ȧ��ʼ��ַ
#define COIL_END          15       // ��Ȧ������ַ
#define COIL_NCOILS       16       // ��Ȧ����

#define DISCRETE_START     0       // ��ɢ������ʼ��ַ
#define DISCRETE_END      15       // ��ɢ���������ַ
#define DISCRETE_NCOILS   16       // ��ɢ��������

/* Modbus���ݴ洢�� */
static uint16_t usRegHoldingBuf[REG_HOLDING_NREGS];     // ���ּĴ���������
static uint16_t usRegInputBuf[REG_INPUT_NREGS];         // ����Ĵ���������
static uint8_t ucCoilBuf[COIL_NCOILS/8 + 1];           // ��Ȧ������
static uint8_t ucDiscreteBuf[DISCRETE_NCOILS/8 + 1];    // ��ɢ���뻺����

/* ���ļ���ͷ��Ӳ�����ر��� */
static uint32_t g_lastRecvTime = 0;    // ���һ�ν���ʱ��

/* ���ļ���ͷ��ӷ�����ر��� */
static uint8_t g_tx_busy = 0;                   // ����æ��־

/* ���ļ���ͷ��ӼĴ���ӳ�䶨�� */
/* Modbus�Ĵ�����ַ���� */
#define REG_KEY1_STATE        0    // KEY1״̬�Ĵ�����ַ
#define REG_KEY2_STATE        1    // KEY2״̬�Ĵ�����ַ
#define REG_LED1_CONTROL      2    // LED1���ƼĴ�����ַ
#define REG_LED2_CONTROL      3    // LED2���ƼĴ�����ַ
#define REG_LED3_CONTROL      4    // LED3���ƼĴ�����ַ
#define REG_SYSTEM_STATUS     5    // ϵͳ״̬�Ĵ�����ַ

/* ��Ӳ�����صļĴ������� */
#define TEST_COIL_START       0x0000  // ��������Ȧ��ʼ��ַ
#define TEST_DISCRETE_START   0x1000  // ��������ɢ������ʼ��ַ
#define TEST_HOLDING_START    0x2000  // �����ñ��ּĴ�����ʼ��ַ
#define TEST_INPUT_START      0x3000  // ����������Ĵ�����ʼ��ַ

/* ���ļ���ͷ����ȫ�ֱ��� */
uint8_t g_RS485_RxBuf[RS485_RX_BUF_SIZE];  // ���ջ�����
uint16_t g_RS485_RxCount = 0;               // ���ռ�����
uint8_t g_RS485_Frame_Flag = 0;             // ֡������ɱ�־
uint8_t g_lastRecvData[RS485_RX_BUF_SIZE] = {0}; // ���һ�ν��յ�����
uint16_t g_recvLen = 0;                     // ���յ������ݳ���

/* ���RS485_ReciveNew������ʵ�� */
void RS485_ReciveNew(uint8_t *buf, uint16_t len)
{
    /* ������յ������� */
    if(len <= RS485_RX_BUF_SIZE)
    {
        /* �������ݵ�RS485���ջ����� */
        memcpy(g_RS485_RxBuf, buf, len);
        g_RS485_RxCount = len;
        
        /* �������һ�ν��յ����� */
        memcpy(g_lastRecvData, buf, len);
        g_recvLen = len;
        
        /* ���֡������� */
        g_RS485_Frame_Flag = 1;
        
        /* ��ӡ���յ������ݵ����Դ��� */
        printf("\r\n[RS485] Received %d bytes:\r\n", len);
        for(uint16_t i = 0; i < len; i++)
        {
            printf("0x%02X ", buf[i]);
        }
        printf("\r\n");
    }
}

/**
  * @brief  ����Modbus CRC16У��ֵ
  * @param  pucFrame: ����ָ֡��
  * @param  usLen: ���ݳ���
  * @retval CRCУ��ֵ
  */
uint16_t CRC16_Modbus(uint8_t *pucFrame, uint16_t usLen)
{
    uint8_t ucCRCHi = 0xFF;    /* ���ֽڳ�ʼ�� */
    uint8_t ucCRCLo = 0xFF;    /* ���ֽڳ�ʼ�� */
    int iIndex;

    while(usLen--)
    {
        iIndex = ucCRCHi ^ *pucFrame++;
        ucCRCHi = ucCRCLo ^ auchCRCHi[iIndex];
        ucCRCLo = auchCRCLo[iIndex];
    }
    
    /* ��ӵ�����Ϣ */
    printf("[Modbus] CRC Calculation: Hi=0x%02X, Lo=0x%02X\n", ucCRCHi, ucCRCLo);
    
    /* ����CRCֵ�����ֽ���ǰ�����ֽ��ں� */
    return (uint16_t)(ucCRCHi << 8 | ucCRCLo);
}

/**
  * @brief  ��֤Modbus֡��CRC
  * @param  pucFrame: ����ָ֡��
  * @param  usLen: ���ݳ���(����CRC��2���ֽ�)
  * @retval 0: CRC����, 1: CRC��ȷ
  */
uint8_t RS485_VerifyCRC(uint8_t *pucFrame, uint16_t usLen)
{
    uint16_t usCRC16;

    if(usLen < RS485_MIN_FRAME_LEN)
    {
        return 0;
    }

    usCRC16 = CRC16_Modbus(pucFrame, usLen - 2);
    
    /* ��ӵ�����Ϣ����ʾ���ֽ���ǰ�����ֽ��ں� */
    printf("[Modbus] CRC Check: Calculated=0x%04X, Received=0x%04X\n", 
           usCRC16, 
           (uint16_t)((pucFrame[usLen - 2] << 8) | pucFrame[usLen - 1]));
    
    /* �Ƚ�CRC�����ֽ���ǰ�����ֽ��ں� */
    if((uint8_t)(usCRC16 >> 8) == pucFrame[usLen - 2] &&
       (uint8_t)(usCRC16 & 0xFF) == pucFrame[usLen - 1])
    {
        return 1;
    }
    
    return 0;
}

/**
  * @brief  RS485��ʼ��
  * @param  None
  * @retval None
  */
void RS485_Init(void)
{
    /* ��ʼ��ʱ����Ϊ����ģʽ */
    HAL_GPIO_WritePin(RS485_EN_GPIO_Port, RS485_EN_Pin, GPIO_PIN_RESET);  // �͵�ƽ
    
    /* ��ս��ջ����� */
    g_RS485_RxCount = 0;
    g_RS485_Frame_Flag = 0;
    for(uint16_t i = 0; i < RS485_RX_BUF_SIZE; i++)
    {
        g_RS485_RxBuf[i] = 0;
    }
}

/**
  * @brief  RS485��������
  * @param  _ucaBuf: ���ݻ�����
  * @param  _usLen: ���ݳ���
  * @retval None
  */
void RS485_SendBuf(uint8_t *_ucaBuf, uint16_t _usLen)
{
    /* �ȴ���һ�η������ */
    while(g_tx_busy)
    {
        __NOP();
    }
    
    /* ���÷��ͱ�־ */
    g_tx_busy = 1;
    
    /* �л�Ϊ����ģʽ */
    HAL_GPIO_WritePin(RS485_EN_GPIO_Port, RS485_EN_Pin, GPIO_PIN_SET);
    
    /* ��ӡ���͵����� */
    printf("\r\n[Modbus] Response(%d bytes): ", _usLen);
    for(uint16_t i = 0; i < _usLen; i++)
    {
        printf("%02X ", _ucaBuf[i]);
    }
    printf("\r\n");
    
    /* �������� */
    HAL_UART_Transmit_IT(&huart3, _ucaBuf, _usLen);
}

/**
  * @brief  ���ڷ�����ɻص�����
  * @param  huart: ���ھ��
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART3)
    {
        /* �ȴ�������� */
        while(__HAL_UART_GET_FLAG(&huart3, UART_FLAG_TC) == RESET)
        {
        }
        
        /* ��ʱԼ17.4us (2������ʱ��) */
        for(uint8_t i = 0; i < 17; i++)
        {
            __NOP();
        }
        
        /* �л�Ϊ����ģʽ */
        HAL_GPIO_WritePin(RS485_EN_GPIO_Port, RS485_EN_Pin, GPIO_PIN_RESET);
        
        /* �������æ��־ */
        g_tx_busy = 0;
    }
}

/**
  * @brief  RS485���ճ�ʱ���
  * @param  None
  * @retval None
  */
void RS485_RxCheck(void)
{
    /* ������ڽ��������ҳ���3.5���ַ�ʱ��û�������� */
    if(g_RS485_RxCount > 0 && (HAL_GetTick() - g_lastRecvTime) > 2)  // 2ms > 3.5T@115200
    {
        /* ������ɣ��������� */
        memcpy(g_lastRecvData, g_RS485_RxBuf, g_RS485_RxCount);
        g_recvLen = g_RS485_RxCount;
        
        /* ��ӡ���յ������� */
        printf("\r\n[RS485] Complete packet received, length=%d\r\n", g_recvLen);
        for(uint16_t i = 0; i < g_recvLen; i++)
        {
            printf("0x%02X ", g_lastRecvData[i]);
        }
        printf("\r\n");
        
        /* ����֡������ɱ�־ */
        g_RS485_Frame_Flag = 1;
        
        /* ��ս��ռ����� */
        g_RS485_RxCount = 0;
    }
}

/**
  * @brief  �޸ļĴ���������
  * @param  None
  * @retval None
  */
void UpdateModbusRegisters(void)
{
    /* ���°���״̬������Ĵ��� */
    usRegInputBuf[REG_KEY1_STATE] = KEY_GetState(KEY1);
    usRegInputBuf[REG_KEY2_STATE] = KEY_GetState(KEY2);
    
    /* ����ϵͳ״̬ */
    usRegInputBuf[REG_SYSTEM_STATUS] = 0x0001;  // ϵͳ��������
}

void ProcessModbusRegisters(void)
{
    /* ������Ȧ״̬����LED */
    if(ucCoilBuf[0] & 0x01)  // ����0����Ȧ״̬
    {
        LED_SetState(LED_R, LED_ON);  // �򿪺�ɫLED
    }
    else
    {
        LED_SetState(LED_R, LED_OFF); // �رպ�ɫLED
    }
    
    if(ucCoilBuf[0] & 0x02)  // ����1����Ȧ״̬
    {
        LED_SetState(LED_G, LED_ON);  // ����ɫLED
    }
    else
    {
        LED_SetState(LED_G, LED_OFF); // �ر���ɫLED
    }
    
    if(ucCoilBuf[0] & 0x04)  // ����2����Ȧ״̬
    {
        LED_SetState(LED_B, LED_ON);  // ����ɫLED
    }
    else
    {
        LED_SetState(LED_B, LED_OFF); // �ر���ɫLED
    }
}

/**
  * @brief  �޸�OLED��ʾ����
  * @param  None
  * @retval None
  */
void RS485_DisplayData(void)
{
    char buf[32];
    
    /* ��ʾͨ��״̬ */
    sprintf(buf, "COM:%s", g_tx_busy ? "BUSY" : "IDLE");
    OLED_ShowString(0, 0, buf);
    
    /* ��ʾ�����յĹ����� */
    if(g_RS485_Frame_Flag && g_RS485_RxCount > 1)
    {
        sprintf(buf, "FC:0x%02X", g_RS485_RxBuf[1]);
        OLED_ShowString(64, 0, buf);
    }
    
    /* ��ʾ�Ĵ���״̬ */
    sprintf(buf, "REG:%04X %04X", 
            usRegHoldingBuf[0], usRegInputBuf[0]);
    OLED_ShowString(0, 1, buf);
    
    /* ��ʾLED�Ͱ���״̬ */
    sprintf(buf, "L:%d%d%d K:%d%d",
            LED_GetState(LED_R),
            LED_GetState(LED_G),
            LED_GetState(LED_B),
            KEY_GetState(KEY1),
            KEY_GetState(KEY2));
    OLED_ShowString(0, 2, buf);
}

/**
  * @brief  RS485������յ���֡����
  * @param  None
  * @retval None
  */
void RS485_Handler(void)
{
    /* ������յ���Modbus֡ */
    if(g_RS485_Frame_Flag)  // ���յ�һ֡����
    {
        printf("\r\n[Modbus] Processing frame...\n");
        
        if(RS485_VerifyCRC(g_RS485_RxBuf, g_RS485_RxCount))
        {
            printf("[Modbus] CRC check passed\n");
            /* CRCУ��ͨ������������ */
            RS485_ProcessData(g_RS485_RxBuf, g_RS485_RxCount);
        }
        else
        {
            printf("[Modbus] CRC check failed\n");
        }
        
        /* ������ձ�־ */
        g_RS485_Frame_Flag = 0;
        g_RS485_RxCount = 0;
    }

    /* ����Modbus�Ĵ��� */
    UpdateModbusRegisters();
    
    /* ����Modbus�Ĵ���ֵ */
    ProcessModbusRegisters();
    
    /* ������ʾ */
    RS485_DisplayData();
}

/**
  * @brief  ����Modbus��ȡ����
  * @param  _pFrame: ֡����ָ��
  * @param  _ucLen: ֡����
  * @retval None
  */
static void MB_ReadRegister(uint8_t *_pFrame, uint16_t _ucLen)
{
    uint16_t usAddr;
    uint16_t usNRegs;
    uint8_t ucByteCnt;
    uint8_t *pData;
    
    usAddr = (_pFrame[RS485_DATA_OFFSET] << 8) | _pFrame[RS485_DATA_OFFSET + 1];
    usNRegs = (_pFrame[RS485_DATA_OFFSET + 2] << 8) | _pFrame[RS485_DATA_OFFSET + 3];
    
    /* ����ַ�������Ƿ���Ч */
    if(usAddr + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS)
    {
        /* ׼����Ӧ���� */
        _pFrame[RS485_DATA_OFFSET] = usNRegs * 2;  // �ֽ���
        pData = &_pFrame[RS485_DATA_OFFSET + 1];
        
        /* �������� */
        for(uint16_t i = 0; i < usNRegs; i++)
        {
            *pData++ = (uint8_t)(usRegHoldingBuf[usAddr - REG_HOLDING_START + i] >> 8);
            *pData++ = (uint8_t)(usRegHoldingBuf[usAddr - REG_HOLDING_START + i] & 0xFF);
        }
        
        /* ������Ӧ֡���� */
        ucByteCnt = usNRegs * 2 + 3;  // �����ֽ��� + ������ + �ֽڼ���
        
        /* ���CRC */
        uint16_t usCRC = CRC16_Modbus(_pFrame, ucByteCnt);
        _pFrame[ucByteCnt] = (uint8_t)(usCRC >> 8);
        _pFrame[ucByteCnt + 1] = (uint8_t)(usCRC & 0xFF);
        
        /* ������Ӧ */
        RS485_SendBuf(_pFrame, ucByteCnt + 2);
    }
}

/**
  * @brief  ����Modbusд�빦��
  * @param  _pFrame: ֡����ָ��
  * @param  _ucLen: ֡����
  * @retval None
  */
static void MB_WriteRegister(uint8_t *_pFrame, uint16_t _ucLen)
{
    uint16_t usAddr;
    uint16_t usValue;
    
    usAddr = (_pFrame[RS485_DATA_OFFSET] << 8) | _pFrame[RS485_DATA_OFFSET + 1];
    usValue = (_pFrame[RS485_DATA_OFFSET + 2] << 8) | _pFrame[RS485_DATA_OFFSET + 3];
    
    /* ����ַ�Ƿ���Ч */
    if(usAddr <= COIL_END)
    {
        /* д������ */
        usRegHoldingBuf[usAddr - REG_HOLDING_START] = usValue;
        
        /* ������Ӧ�����Խ��յ���֡�� */
        RS485_SendBuf(_pFrame, _ucLen);
    }
}

/**
  * @brief  ����Modbus��ȡ��Ȧ����
  * @param  _pFrame: ֡����ָ��
  * @param  _ucLen: ֡����
  * @retval None
  */
static void MB_ReadCoils(uint8_t *_pFrame, uint16_t _usLen)
{
    uint16_t usAddr, usNCoils;
    uint8_t ucByteCount;
    uint16_t usCRC;
    
    /* ������ַ������ */
    usAddr = (_pFrame[2] << 8) | _pFrame[3];
    usNCoils = (_pFrame[4] << 8) | _pFrame[5];
    
    /* ����ַ�������Ƿ���Ч */
    if((usAddr + usNCoils <= COIL_NCOILS) && (usNCoils > 0))
    {
        /* �����ֽ��� */
        ucByteCount = (usNCoils + 7) / 8;
        
        /* ׼����Ӧ */
        _pFrame[2] = ucByteCount;  // �ֽ���
        
        /* ������Ȧ״̬ */
        for(uint8_t i = 0; i < ucByteCount; i++)
        {
            _pFrame[3 + i] = ucCoilBuf[usAddr/8 + i];
        }
        
        /* ����CRC */
        usCRC = CRC16_Modbus(_pFrame, 3 + ucByteCount);
        _pFrame[3 + ucByteCount] = (uint8_t)(usCRC >> 8);
        _pFrame[4 + ucByteCount] = (uint8_t)(usCRC & 0xFF);
        
        /* ������Ӧ */
        RS485_SendBuf(_pFrame, 5 + ucByteCount);
    }
    else
    {
        /* ���ش�����Ӧ */
        _pFrame[1] |= 0x80;  // ���ô����־
        _pFrame[2] = MODBUS_ERR_ADDR;  // ��ַ����
        
        /* ����CRC */
        usCRC = CRC16_Modbus(_pFrame, 3);
        _pFrame[3] = (uint8_t)(usCRC >> 8);
        _pFrame[4] = (uint8_t)(usCRC & 0xFF);
        
        /* ���ʹ�����Ӧ */
        RS485_SendBuf(_pFrame, 5);
    }
}

/**
  * @brief  ����Modbusд������Ȧ����
  * @param  _pFrame: ֡����ָ��
  * @param  _ucLen: ֡����
  * @retval None
  */
static void MB_WriteCoil(uint8_t *_pFrame, uint16_t _ucLen)
{
    uint16_t usAddr;
    uint16_t usValue;
    uint8_t ucByteOffset;
    uint8_t ucBitOffset;
    
    usAddr = (_pFrame[RS485_DATA_OFFSET] << 8) | _pFrame[RS485_DATA_OFFSET + 1];
    usValue = (_pFrame[RS485_DATA_OFFSET + 2] << 8) | _pFrame[RS485_DATA_OFFSET + 3];
    
    /* ��ӵ�����Ϣ */
    printf("[Modbus] Write Coil: Addr=0x%04X, Value=0x%04X\n", usAddr, usValue);
    
    /* ����ַ�Ƿ���Ч */
    if(usAddr <= COIL_END)
    {
        ucByteOffset = (usAddr - COIL_START) / 8;
        ucBitOffset = (usAddr - COIL_START) % 8;
        
        /* д����Ȧ״̬ */
        if(usValue == 0xFF00)  // ��λ
        {
            ucCoilBuf[ucByteOffset] |= (1 << ucBitOffset);
            printf("[Modbus] Set coil %d ON\n", usAddr);
        }
        else  // ��λ
        {
            ucCoilBuf[ucByteOffset] &= ~(1 << ucBitOffset);
            printf("[Modbus] Set coil %d OFF\n", usAddr);
        }
        
        /* ��������LED״̬ */
        ProcessModbusRegisters();
        
        /* ������Ӧ�����Խ��յ���֡�� */
        RS485_SendBuf(_pFrame, _ucLen);
    }
}

/**
  * @brief  ����Modbus������Ĵ�������
  * @param  _pFrame: ֡����ָ��
  * @param  _ucLen: ֡����
  * @retval None
  */
static void MB_ReadInputRegister(uint8_t *_pFrame, uint16_t _ucLen)
{
    uint16_t usAddr;
    uint16_t usNRegs;
    uint16_t usCRC;
    uint8_t *pData;
    uint8_t ucByteCnt;
    
    usAddr = (_pFrame[RS485_DATA_OFFSET] << 8) | _pFrame[RS485_DATA_OFFSET + 1];
    usNRegs = (_pFrame[RS485_DATA_OFFSET + 2] << 8) | _pFrame[RS485_DATA_OFFSET + 3];
    
    /* ����ַ�������Ƿ���Ч */
    if((usAddr <= REG_INPUT_END) && 
       (usAddr + usNRegs <= REG_INPUT_END + 1))
    {
        /* ׼����Ӧ���� */
        _pFrame[RS485_DATA_OFFSET] = usNRegs * 2;  // �ֽ���
        pData = &_pFrame[RS485_DATA_OFFSET + 1];
        
        /* �������� */
        for(uint16_t i = 0; i < usNRegs; i++)
        {
            *pData++ = (uint8_t)(usRegInputBuf[usAddr - REG_INPUT_START + i] >> 8);
            *pData++ = (uint8_t)(usRegInputBuf[usAddr - REG_INPUT_START + i] & 0xFF);
        }
        
        /* ������Ӧ֡���� */
        ucByteCnt = usNRegs * 2 + 3;  // �����ֽ��� + ������ + �ֽڼ���
        
        /* ���CRC */
        usCRC = CRC16_Modbus(_pFrame, ucByteCnt);
        _pFrame[ucByteCnt] = (uint8_t)(usCRC >> 8);
        _pFrame[ucByteCnt + 1] = (uint8_t)(usCRC & 0xFF);
        
        /* ������Ӧ */
        RS485_SendBuf(_pFrame, ucByteCnt + 2);
    }
}

/**
  * @brief  ����Modbusд����Ĵ�������
  * @param  _pFrame: ֡����ָ��
  * @param  _ucLen: ֡����
  * @retval None
  */
static void MB_WriteMultipleRegisters(uint8_t *_pFrame, uint16_t _ucLen)
{
    uint16_t usAddr;
    uint16_t usNRegs;
    uint16_t usCRC;
    uint8_t ucByteCnt;
    uint8_t *pData;
    
    usAddr = (_pFrame[RS485_DATA_OFFSET] << 8) | _pFrame[RS485_DATA_OFFSET + 1];
    usNRegs = (_pFrame[RS485_DATA_OFFSET + 2] << 8) | _pFrame[RS485_DATA_OFFSET + 3];
    ucByteCnt = _pFrame[RS485_DATA_OFFSET + 4];
    pData = &_pFrame[RS485_DATA_OFFSET + 5];
    
    /* ����ַ�������Ƿ���Ч */
    if((usAddr <= REG_HOLDING_END) && 
       (usAddr + usNRegs <= REG_HOLDING_END + 1) &&
       (ucByteCnt == usNRegs * 2))
    {
        /* д������ */
        for(uint16_t i = 0; i < usNRegs; i++)
        {
            usRegHoldingBuf[usAddr - REG_HOLDING_START + i] = 
                (((uint16_t)pData[i * 2] << 8) | pData[i * 2 + 1]);
        }
        
        /* ׼����Ӧ���� */
        _pFrame[RS485_DATA_OFFSET] = (uint8_t)(usAddr >> 8);
        _pFrame[RS485_DATA_OFFSET + 1] = (uint8_t)(usAddr & 0xFF);
        _pFrame[RS485_DATA_OFFSET + 2] = (uint8_t)(usNRegs >> 8);
        _pFrame[RS485_DATA_OFFSET + 3] = (uint8_t)(usNRegs & 0xFF);
        
        /* ���㲢���CRC */
        usCRC = CRC16_Modbus(_pFrame, 6);
        _pFrame[6] = (uint8_t)(usCRC >> 8);
        _pFrame[7] = (uint8_t)(usCRC & 0xFF);
        
        /* ������Ӧ */
        RS485_SendBuf(_pFrame, 8);
    }
}

/**
  * @brief  ����Modbus����ɢ���빦��
  * @param  _pFrame: ֡����ָ��
  * @param  _ucLen: ֡����
  * @retval None
  */
static void MB_ReadDiscreteInputs(uint8_t *_pFrame, uint16_t _ucLen)
{
    uint16_t usAddr;
    uint16_t usNCoils;
    uint16_t usCRC;
    uint8_t ucByteCnt;
    uint8_t ucBitOffset;
    uint8_t ucByteOffset;
    uint8_t *pData;
    
    usAddr = (_pFrame[RS485_DATA_OFFSET] << 8) | _pFrame[RS485_DATA_OFFSET + 1];
    usNCoils = (_pFrame[RS485_DATA_OFFSET + 2] << 8) | _pFrame[RS485_DATA_OFFSET + 3];
    
    /* ����ַ�������Ƿ���Ч */
    if((usAddr <= DISCRETE_END) && 
       (usAddr + usNCoils <= DISCRETE_END + 1))
    {
        /* ׼����Ӧ���� */
        ucByteCnt = (usNCoils + 7) / 8;  // ������Ҫ���ֽ���
        _pFrame[RS485_DATA_OFFSET] = ucByteCnt;
        pData = &_pFrame[RS485_DATA_OFFSET + 1];
        
        /* ������ɢ����״̬ */
        for(uint16_t i = 0; i < ucByteCnt; i++)
        {
            pData[i] = 0;  // ���㵱ǰ�ֽ�
            for(uint8_t j = 0; j < 8; j++)
            {
                if((i * 8 + j) < usNCoils)
                {
                    ucByteOffset = (usAddr - DISCRETE_START + i * 8 + j) / 8;
                    ucBitOffset = (usAddr - DISCRETE_START + i * 8 + j) % 8;
                    
                    if(ucDiscreteBuf[ucByteOffset] & (1 << ucBitOffset))
                    {
                        pData[i] |= (1 << j);
                    }
                }
            }
        }
        
        /* ������Ӧ֡���� */
        ucByteCnt += 3;  // �����ֽ��� + ������ + �ֽڼ���
        
        /* ���CRC */
        usCRC = CRC16_Modbus(_pFrame, ucByteCnt);
        _pFrame[ucByteCnt] = (uint8_t)(usCRC >> 8);
        _pFrame[ucByteCnt + 1] = (uint8_t)(usCRC & 0xFF);
        
        /* ������Ӧ */
        RS485_SendBuf(_pFrame, ucByteCnt + 2);
    }
}

/**
  * @brief  ����Modbus����֡
  * @param  _pBuf: ���ݻ�����
  * @param  _usLen: ���ݳ���
  * @retval None
  */
void RS485_ProcessData(uint8_t *_pBuf, uint16_t _usLen)
{
    uint8_t ucFuncCode;
    
    /* ��ӡ���յ�������֡ */
    printf("\r\n[Modbus] RX Frame(%d): ", _usLen);
    for(uint16_t i = 0; i < _usLen; i++)
    {
        printf("%02X ", _pBuf[i]);
    }
    printf("\r\n");
    
    ucFuncCode = _pBuf[RS485_FUNC_OFFSET];
    
    /* �������� */
    switch(ucFuncCode)
    {
        case MODBUS_FUNC_READ_COILS:          // 0x01
            printf("[Modbus] Read Coils\r\n");
            MB_ReadCoils(_pBuf, _usLen);
            break;
            
        case MODBUS_FUNC_READ_DISCRETE:       // 0x02
            printf("[Modbus] Read Discrete Inputs\r\n");
            MB_ReadDiscreteInputs(_pBuf, _usLen);
            break;
            
        case MODBUS_FUNC_READ_HOLDING:        // 0x03
            printf("[Modbus] Read Holding Registers\r\n");
            MB_ReadRegister(_pBuf, _usLen);
            break;
            
        case MODBUS_FUNC_READ_INPUT:          // 0x04
            printf("[Modbus] Read Input Registers\r\n");
            MB_ReadInputRegister(_pBuf, _usLen);
            break;
            
        case MODBUS_FUNC_WRITE_COIL:         // 0x05
            printf("[Modbus] Write Single Coil\r\n");
            MB_WriteCoil(_pBuf, _usLen);
            break;
            
        case MODBUS_FUNC_WRITE_HOLDING:      // 0x06
            printf("[Modbus] Write Single Register\r\n");
            MB_WriteRegister(_pBuf, _usLen);
            break;
            
        case MODBUS_FUNC_WRITE_HOLDINGS:     // 0x10
            printf("[Modbus] Write Multiple Registers\r\n");
            MB_WriteMultipleRegisters(_pBuf, _usLen);
            break;
            
        default:
            printf("[Modbus] Error: Unsupported Function Code 0x%02X\r\n", ucFuncCode);
            /* ���ش�����Ӧ */
            _pBuf[RS485_FUNC_OFFSET] |= 0x80;
            _pBuf[RS485_DATA_OFFSET] = MODBUS_ERR_FUNC;
            uint16_t usCRC = CRC16_Modbus(_pBuf, 3);
            _pBuf[3] = (uint8_t)(usCRC >> 8);
            _pBuf[4] = (uint8_t)(usCRC & 0xFF);
            RS485_SendBuf(_pBuf, 5);
            break;
    }
}

