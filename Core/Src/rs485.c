#include "rs485.h"
#include "oled.h"     // ���OLED��غ�������
#include <stdio.h>    // ���sprintf��������
#include <string.h>   // ���strlen��������

/* CRC ��λ�ֽ�ֵ�� */
static const uint8_t s_CRCHi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40
};

/* CRC ��λ�ֽ�ֵ�� */
static const uint8_t s_CRCLo[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,
    0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,
    0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9,
    0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
    0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,
    0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,
    0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38,
    0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF,
    0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,
    0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
    0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB,
    0x69, 0xA9, 0xA8, 0x68
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

/* ��ӷ�����ر��� */
static uint8_t g_tx_buffer[RS485_TX_BUF_SIZE];  // ���ͻ�����
static uint16_t g_tx_len = 0;                   // ���ͳ���
static uint8_t g_tx_busy = 0;                   // ����æ��־

/* ���ļ���ͷ��ӼĴ���ӳ�䶨�� */
/* Modbus�Ĵ�����ַ���� */
#define REG_KEY1_STATE        0    // KEY1״̬�Ĵ�����ַ
#define REG_KEY2_STATE        1    // KEY2״̬�Ĵ�����ַ
#define REG_LED1_CONTROL      2    // LED1���ƼĴ�����ַ
#define REG_LED2_CONTROL      3    // LED2���ƼĴ�����ַ
#define REG_LED3_CONTROL      4    // LED3���ƼĴ�����ַ
#define REG_SYSTEM_STATUS     5    // ϵͳ״̬�Ĵ�����ַ

/**
  * @brief  ����CRC16У��ֵ
  * @param  _pBuf: ���ݻ�����
  * @param  _usLen: ���ݳ���
  * @retval CRC16У��ֵ
  */
uint16_t CRC16_Modbus(uint8_t *_pBuf, uint16_t _usLen)
{
    uint8_t ucCRCHi = 0xFF;
    uint8_t ucCRCLo = 0xFF;
    uint16_t usIndex;

    while (_usLen--)
    {
        usIndex = ucCRCLo ^ *(_pBuf++);
        ucCRCLo = ucCRCHi ^ s_CRCHi[usIndex];
        ucCRCHi = s_CRCLo[usIndex];
    }
    return ((uint16_t)ucCRCHi << 8 | ucCRCLo);
}

/**
  * @brief  ��֤�������ݵ�CRC
  * @param  _pBuf: ���ݻ�����
  * @param  _usLen: ���ݳ���(����CRC��2���ֽ�)
  * @retval 0: CRC����, 1: CRC��ȷ
  */
uint8_t RS485_VerifyCRC(uint8_t *_pBuf, uint16_t _usLen)
{
    uint16_t crc1, crc2;
    
    if(_usLen < RS485_MIN_FRAME_LEN)
    {
        return 0;
    }
    
    crc1 = CRC16_Modbus(_pBuf, _usLen - 2);
    crc2 = ((uint16_t)_pBuf[_usLen - 2] << 8) | _pBuf[_usLen - 1]);
    
    return (crc1 == crc2) ? 1 : 0;
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
    /* ������ */
    if(_ucaBuf == NULL || _usLen == 0 || _usLen > RS485_TX_BUF_SIZE)
    {
        return;
    }
    
    /* �ȴ���һ�η������ */
    while(g_tx_busy)
    {
        HAL_Delay(1);
    }
    
    /* �������ݵ����ͻ����� */
    memcpy(g_tx_buffer, _ucaBuf, _usLen);
    g_tx_len = _usLen;
    
    /* ���÷��ͱ�־ */
    g_tx_busy = 1;
    
    /* ����Ϊ����ģʽ */
    HAL_GPIO_WritePin(RS485_EN_GPIO_Port, RS485_EN_Pin, GPIO_PIN_SET);
    
    /* ��ʱԼ35us (4������ʱ��) */
    for(uint8_t i = 0; i < 35; i++)
    {
        __NOP();
    }
    
    /* �������� */
    HAL_UART_Transmit_IT(&huart3, g_tx_buffer, g_tx_len);
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
    /* ����LED���ƼĴ��� */
    LED_SetState(LED_R, (usRegHoldingBuf[REG_LED1_CONTROL] != 0) ? LED_ON : LED_OFF);
    LED_SetState(LED_G, (usRegHoldingBuf[REG_LED2_CONTROL] != 0) ? LED_ON : LED_OFF);
    LED_SetState(LED_B, (usRegHoldingBuf[REG_LED3_CONTROL] != 0) ? LED_ON : LED_OFF);
}

/**
  * @brief  �޸�OLED��ʾ����
  * @param  None
  * @retval None
  */
void RS485_DisplayData(void)
{
    char buf[32];
    
    /* �����ʾ���� */
    OLED_ShowString(0, 2, "                    ");
    OLED_ShowString(0, 3, "                    ");
    OLED_ShowString(0, 4, "                    ");
    OLED_ShowString(0, 5, "                    ");
    
    /* ��ʾ����״̬ */
    sprintf(buf, "KEY: %d %d", 
            usRegInputBuf[REG_KEY1_STATE],
            usRegInputBuf[REG_KEY2_STATE]);
    OLED_ShowString(0, 2, buf);
    
    /* ��ʾLED״̬ */
    sprintf(buf, "LED: %d %d %d",
            usRegHoldingBuf[REG_LED1_CONTROL],
            usRegHoldingBuf[REG_LED2_CONTROL],
            usRegHoldingBuf[REG_LED3_CONTROL]);
    OLED_ShowString(0, 3, buf);
    
    /* ��ʾͨ��״̬ */
    if(g_RS485_Frame_Flag)
    {
        sprintf(buf, "RX:%d bytes", g_RS485_RxCount);
        OLED_ShowString(0, 4, buf);
        
        /* ��ʾ���4���ֽڵ����� */
        sprintf(buf, "D:%02X %02X %02X %02X",
                g_RS485_RxBuf[g_RS485_RxCount-4],
                g_RS485_RxBuf[g_RS485_RxCount-3],
                g_RS485_RxBuf[g_RS485_RxCount-2],
                g_RS485_RxBuf[g_RS485_RxCount-1]);
        OLED_ShowString(0, 5, buf);
    }
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
        if(RS485_VerifyCRC(g_RS485_RxBuf, g_RS485_RxCount))
        {
            /* CRCУ��ͨ������������ */
            RS485_ProcessData(g_RS485_RxBuf, g_RS485_RxCount);
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
static void MB_ReadCoils(uint8_t *_pFrame, uint16_t _ucLen)
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
    if(usAddr + usNCoils <= COIL_START + COIL_NCOILS)
    {
        /* ׼����Ӧ���� */
        ucByteCnt = (usNCoils + 7) / 8;  // ������Ҫ���ֽ���
        _pFrame[RS485_DATA_OFFSET] = ucByteCnt;
        pData = &_pFrame[RS485_DATA_OFFSET + 1];
        
        /* ������Ȧ״̬ */
        for(uint16_t i = 0; i < ucByteCnt; i++)
        {
            pData[i] = 0;  // ���㵱ǰ�ֽ�
            for(uint8_t j = 0; j < 8; j++)
            {
                if((i * 8 + j) < usNCoils)
                {
                    ucByteOffset = (usAddr - COIL_START + i * 8 + j) / 8;
                    ucBitOffset = (usAddr - COIL_START + i * 8 + j) % 8;
                    
                    if(ucCoilBuf[ucByteOffset] & (1 << ucBitOffset))
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
    
    /* ����ַ�Ƿ���Ч */
    if(usAddr <= COIL_END)
    {
        ucByteOffset = (usAddr - COIL_START) / 8;
        ucBitOffset = (usAddr - COIL_START) % 8;
        
        /* д����Ȧ״̬ */
        if(usValue == 0xFF00)  // ��λ
        {
            ucCoilBuf[ucByteOffset] |= (1 << ucBitOffset);
        }
        else  // ��λ
        {
            ucCoilBuf[ucByteOffset] &= ~(1 << ucBitOffset);
        }
        
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
    uint8_t ucFuncCode = _pBuf[RS485_FUNC_OFFSET];
    
    switch(ucFuncCode)
    {
        case MODBUS_FUNC_READ_COILS:      // ����Ȧ
            MB_ReadCoils(_pBuf, _usLen);
            break;
            
        case MODBUS_FUNC_READ_DISCRETE:   // ����ɢ����
            MB_ReadDiscreteInputs(_pBuf, _usLen);
            break;
            
        case MODBUS_FUNC_READ_INPUT:      // ������Ĵ���
            MB_ReadInputRegister(_pBuf, _usLen);
            break;
            
        case MODBUS_FUNC_READ_HOLDING:    // �����ּĴ���
            MB_ReadRegister(_pBuf, _usLen);
            break;
            
        case MODBUS_FUNC_WRITE_COIL:      // д������Ȧ
            MB_WriteCoil(_pBuf, _usLen);
            break;
            
        case MODBUS_FUNC_WRITE_HOLDING:   // д�������ּĴ���
            MB_WriteRegister(_pBuf, _usLen);
            break;
            
        case MODBUS_FUNC_WRITE_HOLDINGS:  // д������ּĴ���
            MB_WriteMultipleRegisters(_pBuf, _usLen);
            break;
            
        default:
            /* ��֧�ֵĹ����룬���ش�����Ӧ */
            _pBuf[RS485_FUNC_OFFSET] |= 0x80;   // ���ô����־
            _pBuf[RS485_DATA_OFFSET] = MODBUS_ERR_FUNC;
            
            /* ���㲢���CRC */
            uint16_t usCRC = CRC16_Modbus(_pBuf, 3);
            _pBuf[3] = (uint8_t)(usCRC >> 8);
            _pBuf[4] = (uint8_t)(usCRC & 0xFF);
            
            /* ���ʹ�����Ӧ */
            RS485_SendBuf(_pBuf, 5);
            break;
    }
}

