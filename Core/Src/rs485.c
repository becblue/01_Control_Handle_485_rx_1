#include "rs485.h"
#include "oled.h"     // 添加OLED相关函数声明
#include <stdio.h>    // 添加sprintf函数声明
#include <string.h>   // 添加strlen函数声明

/* CRC 高位字节值表 */
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

/* CRC 低位字节值表 */
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

/* Modbus寄存器和线圈定义 */
#define REG_HOLDING_START    0       // 保持寄存器起始地址
#define REG_HOLDING_END     15       // 保持寄存器结束地址
#define REG_HOLDING_NREGS   16       // 保持寄存器数量

#define REG_INPUT_START     0       // 输入寄存器起始地址
#define REG_INPUT_END      15       // 输入寄存器结束地址
#define REG_INPUT_NREGS    16       // 输入寄存器数量

#define COIL_START         0       // 线圈起始地址
#define COIL_END          15       // 线圈结束地址
#define COIL_NCOILS       16       // 线圈数量

#define DISCRETE_START     0       // 离散输入起始地址
#define DISCRETE_END      15       // 离散输入结束地址
#define DISCRETE_NCOILS   16       // 离散输入数量

/* Modbus数据存储区 */
static uint16_t usRegHoldingBuf[REG_HOLDING_NREGS];     // 保持寄存器缓冲区
static uint16_t usRegInputBuf[REG_INPUT_NREGS];         // 输入寄存器缓冲区
static uint8_t ucCoilBuf[COIL_NCOILS/8 + 1];           // 线圈缓冲区
static uint8_t ucDiscreteBuf[DISCRETE_NCOILS/8 + 1];    // 离散输入缓冲区

/* 在文件开头添加测试相关变量 */
static uint32_t g_lastRecvTime = 0;    // 最后一次接收时间

/* 添加发送相关变量 */
static uint8_t g_tx_buffer[RS485_TX_BUF_SIZE];  // 发送缓冲区
static uint16_t g_tx_len = 0;                   // 发送长度
static uint8_t g_tx_busy = 0;                   // 发送忙标志

/* 在文件开头添加寄存器映射定义 */
/* Modbus寄存器地址定义 */
#define REG_KEY1_STATE        0    // KEY1状态寄存器地址
#define REG_KEY2_STATE        1    // KEY2状态寄存器地址
#define REG_LED1_CONTROL      2    // LED1控制寄存器地址
#define REG_LED2_CONTROL      3    // LED2控制寄存器地址
#define REG_LED3_CONTROL      4    // LED3控制寄存器地址
#define REG_SYSTEM_STATUS     5    // 系统状态寄存器地址

/**
  * @brief  计算CRC16校验值
  * @param  _pBuf: 数据缓冲区
  * @param  _usLen: 数据长度
  * @retval CRC16校验值
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
  * @brief  验证接收数据的CRC
  * @param  _pBuf: 数据缓冲区
  * @param  _usLen: 数据长度(包含CRC的2个字节)
  * @retval 0: CRC错误, 1: CRC正确
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
  * @brief  RS485初始化
  * @param  None
  * @retval None
  */
void RS485_Init(void)
{
    /* 初始化时设置为接收模式 */
    HAL_GPIO_WritePin(RS485_EN_GPIO_Port, RS485_EN_Pin, GPIO_PIN_RESET);  // 低电平
    
    /* 清空接收缓冲区 */
    g_RS485_RxCount = 0;
    g_RS485_Frame_Flag = 0;
    for(uint16_t i = 0; i < RS485_RX_BUF_SIZE; i++)
    {
        g_RS485_RxBuf[i] = 0;
    }
}

/**
  * @brief  RS485发送数据
  * @param  _ucaBuf: 数据缓冲区
  * @param  _usLen: 数据长度
  * @retval None
  */
void RS485_SendBuf(uint8_t *_ucaBuf, uint16_t _usLen)
{
    /* 检查参数 */
    if(_ucaBuf == NULL || _usLen == 0 || _usLen > RS485_TX_BUF_SIZE)
    {
        return;
    }
    
    /* 等待上一次发送完成 */
    while(g_tx_busy)
    {
        HAL_Delay(1);
    }
    
    /* 复制数据到发送缓冲区 */
    memcpy(g_tx_buffer, _ucaBuf, _usLen);
    g_tx_len = _usLen;
    
    /* 设置发送标志 */
    g_tx_busy = 1;
    
    /* 设置为发送模式 */
    HAL_GPIO_WritePin(RS485_EN_GPIO_Port, RS485_EN_Pin, GPIO_PIN_SET);
    
    /* 延时约35us (4个比特时间) */
    for(uint8_t i = 0; i < 35; i++)
    {
        __NOP();
    }
    
    /* 启动发送 */
    HAL_UART_Transmit_IT(&huart3, g_tx_buffer, g_tx_len);
}

/**
  * @brief  串口发送完成回调函数
  * @param  huart: 串口句柄
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART3)
    {
        /* 等待发送完成 */
        while(__HAL_UART_GET_FLAG(&huart3, UART_FLAG_TC) == RESET)
        {
        }
        
        /* 延时约17.4us (2个比特时间) */
        for(uint8_t i = 0; i < 17; i++)
        {
            __NOP();
        }
        
        /* 切换为接收模式 */
        HAL_GPIO_WritePin(RS485_EN_GPIO_Port, RS485_EN_Pin, GPIO_PIN_RESET);
        
        /* 清除发送忙标志 */
        g_tx_busy = 0;
    }
}

/**
  * @brief  RS485接收超时检测
  * @param  None
  * @retval None
  */
void RS485_RxCheck(void)
{
    /* 如果正在接收数据且超过3.5个字符时间没有新数据 */
    if(g_RS485_RxCount > 0 && (HAL_GetTick() - g_lastRecvTime) > 2)  // 2ms > 3.5T@115200
    {
        /* 接收完成，保存数据 */
        memcpy(g_lastRecvData, g_RS485_RxBuf, g_RS485_RxCount);
        g_recvLen = g_RS485_RxCount;
        
        /* 打印接收到的数据 */
        printf("\r\n[RS485] Complete packet received, length=%d\r\n", g_recvLen);
        for(uint16_t i = 0; i < g_recvLen; i++)
        {
            printf("0x%02X ", g_lastRecvData[i]);
        }
        printf("\r\n");
        
        /* 设置帧接收完成标志 */
        g_RS485_Frame_Flag = 1;
        
        /* 清空接收计数器 */
        g_RS485_RxCount = 0;
    }
}

/**
  * @brief  修改寄存器处理函数
  * @param  None
  * @retval None
  */
void UpdateModbusRegisters(void)
{
    /* 更新按键状态到输入寄存器 */
    usRegInputBuf[REG_KEY1_STATE] = KEY_GetState(KEY1);
    usRegInputBuf[REG_KEY2_STATE] = KEY_GetState(KEY2);
    
    /* 更新系统状态 */
    usRegInputBuf[REG_SYSTEM_STATUS] = 0x0001;  // 系统运行正常
}

void ProcessModbusRegisters(void)
{
    /* 处理LED控制寄存器 */
    LED_SetState(LED_R, (usRegHoldingBuf[REG_LED1_CONTROL] != 0) ? LED_ON : LED_OFF);
    LED_SetState(LED_G, (usRegHoldingBuf[REG_LED2_CONTROL] != 0) ? LED_ON : LED_OFF);
    LED_SetState(LED_B, (usRegHoldingBuf[REG_LED3_CONTROL] != 0) ? LED_ON : LED_OFF);
}

/**
  * @brief  修改OLED显示函数
  * @param  None
  * @retval None
  */
void RS485_DisplayData(void)
{
    char buf[32];
    
    /* 清除显示区域 */
    OLED_ShowString(0, 2, "                    ");
    OLED_ShowString(0, 3, "                    ");
    OLED_ShowString(0, 4, "                    ");
    OLED_ShowString(0, 5, "                    ");
    
    /* 显示按键状态 */
    sprintf(buf, "KEY: %d %d", 
            usRegInputBuf[REG_KEY1_STATE],
            usRegInputBuf[REG_KEY2_STATE]);
    OLED_ShowString(0, 2, buf);
    
    /* 显示LED状态 */
    sprintf(buf, "LED: %d %d %d",
            usRegHoldingBuf[REG_LED1_CONTROL],
            usRegHoldingBuf[REG_LED2_CONTROL],
            usRegHoldingBuf[REG_LED3_CONTROL]);
    OLED_ShowString(0, 3, buf);
    
    /* 显示通信状态 */
    if(g_RS485_Frame_Flag)
    {
        sprintf(buf, "RX:%d bytes", g_RS485_RxCount);
        OLED_ShowString(0, 4, buf);
        
        /* 显示最后4个字节的数据 */
        sprintf(buf, "D:%02X %02X %02X %02X",
                g_RS485_RxBuf[g_RS485_RxCount-4],
                g_RS485_RxBuf[g_RS485_RxCount-3],
                g_RS485_RxBuf[g_RS485_RxCount-2],
                g_RS485_RxBuf[g_RS485_RxCount-1]);
        OLED_ShowString(0, 5, buf);
    }
}

/**
  * @brief  RS485处理接收到的帧数据
  * @param  None
  * @retval None
  */
void RS485_Handler(void)
{
    /* 处理接收到的Modbus帧 */
    if(g_RS485_Frame_Flag)  // 接收到一帧数据
    {
        if(RS485_VerifyCRC(g_RS485_RxBuf, g_RS485_RxCount))
        {
            /* CRC校验通过，处理数据 */
            RS485_ProcessData(g_RS485_RxBuf, g_RS485_RxCount);
        }
        
        /* 清除接收标志 */
        g_RS485_Frame_Flag = 0;
        g_RS485_RxCount = 0;
    }

    /* 更新Modbus寄存器 */
    UpdateModbusRegisters();
    
    /* 处理Modbus寄存器值 */
    ProcessModbusRegisters();
    
    /* 更新显示 */
    RS485_DisplayData();
}

/**
  * @brief  处理Modbus读取功能
  * @param  _pFrame: 帧数据指针
  * @param  _ucLen: 帧长度
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
    
    /* 检查地址和数量是否有效 */
    if(usAddr + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS)
    {
        /* 准备响应数据 */
        _pFrame[RS485_DATA_OFFSET] = usNRegs * 2;  // 字节数
        pData = &_pFrame[RS485_DATA_OFFSET + 1];
        
        /* 复制数据 */
        for(uint16_t i = 0; i < usNRegs; i++)
        {
            *pData++ = (uint8_t)(usRegHoldingBuf[usAddr - REG_HOLDING_START + i] >> 8);
            *pData++ = (uint8_t)(usRegHoldingBuf[usAddr - REG_HOLDING_START + i] & 0xFF);
        }
        
        /* 计算响应帧长度 */
        ucByteCnt = usNRegs * 2 + 3;  // 数据字节数 + 功能码 + 字节计数
        
        /* 添加CRC */
        uint16_t usCRC = CRC16_Modbus(_pFrame, ucByteCnt);
        _pFrame[ucByteCnt] = (uint8_t)(usCRC >> 8);
        _pFrame[ucByteCnt + 1] = (uint8_t)(usCRC & 0xFF);
        
        /* 发送响应 */
        RS485_SendBuf(_pFrame, ucByteCnt + 2);
    }
}

/**
  * @brief  处理Modbus写入功能
  * @param  _pFrame: 帧数据指针
  * @param  _ucLen: 帧长度
  * @retval None
  */
static void MB_WriteRegister(uint8_t *_pFrame, uint16_t _ucLen)
{
    uint16_t usAddr;
    uint16_t usValue;
    
    usAddr = (_pFrame[RS485_DATA_OFFSET] << 8) | _pFrame[RS485_DATA_OFFSET + 1];
    usValue = (_pFrame[RS485_DATA_OFFSET + 2] << 8) | _pFrame[RS485_DATA_OFFSET + 3];
    
    /* 检查地址是否有效 */
    if(usAddr <= COIL_END)
    {
        /* 写入数据 */
        usRegHoldingBuf[usAddr - REG_HOLDING_START] = usValue;
        
        /* 发送响应（回显接收到的帧） */
        RS485_SendBuf(_pFrame, _ucLen);
    }
}

/**
  * @brief  处理Modbus读取线圈功能
  * @param  _pFrame: 帧数据指针
  * @param  _ucLen: 帧长度
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
    
    /* 检查地址和数量是否有效 */
    if(usAddr + usNCoils <= COIL_START + COIL_NCOILS)
    {
        /* 准备响应数据 */
        ucByteCnt = (usNCoils + 7) / 8;  // 计算需要的字节数
        _pFrame[RS485_DATA_OFFSET] = ucByteCnt;
        pData = &_pFrame[RS485_DATA_OFFSET + 1];
        
        /* 复制线圈状态 */
        for(uint16_t i = 0; i < ucByteCnt; i++)
        {
            pData[i] = 0;  // 清零当前字节
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
        
        /* 计算响应帧长度 */
        ucByteCnt += 3;  // 数据字节数 + 功能码 + 字节计数
        
        /* 添加CRC */
        usCRC = CRC16_Modbus(_pFrame, ucByteCnt);
        _pFrame[ucByteCnt] = (uint8_t)(usCRC >> 8);
        _pFrame[ucByteCnt + 1] = (uint8_t)(usCRC & 0xFF);
        
        /* 发送响应 */
        RS485_SendBuf(_pFrame, ucByteCnt + 2);
    }
}

/**
  * @brief  处理Modbus写单个线圈功能
  * @param  _pFrame: 帧数据指针
  * @param  _ucLen: 帧长度
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
    
    /* 检查地址是否有效 */
    if(usAddr <= COIL_END)
    {
        ucByteOffset = (usAddr - COIL_START) / 8;
        ucBitOffset = (usAddr - COIL_START) % 8;
        
        /* 写入线圈状态 */
        if(usValue == 0xFF00)  // 置位
        {
            ucCoilBuf[ucByteOffset] |= (1 << ucBitOffset);
        }
        else  // 复位
        {
            ucCoilBuf[ucByteOffset] &= ~(1 << ucBitOffset);
        }
        
        /* 发送响应（回显接收到的帧） */
        RS485_SendBuf(_pFrame, _ucLen);
    }
}

/**
  * @brief  处理Modbus读输入寄存器功能
  * @param  _pFrame: 帧数据指针
  * @param  _ucLen: 帧长度
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
    
    /* 检查地址和数量是否有效 */
    if((usAddr <= REG_INPUT_END) && 
       (usAddr + usNRegs <= REG_INPUT_END + 1))
    {
        /* 准备响应数据 */
        _pFrame[RS485_DATA_OFFSET] = usNRegs * 2;  // 字节数
        pData = &_pFrame[RS485_DATA_OFFSET + 1];
        
        /* 复制数据 */
        for(uint16_t i = 0; i < usNRegs; i++)
        {
            *pData++ = (uint8_t)(usRegInputBuf[usAddr - REG_INPUT_START + i] >> 8);
            *pData++ = (uint8_t)(usRegInputBuf[usAddr - REG_INPUT_START + i] & 0xFF);
        }
        
        /* 计算响应帧长度 */
        ucByteCnt = usNRegs * 2 + 3;  // 数据字节数 + 功能码 + 字节计数
        
        /* 添加CRC */
        usCRC = CRC16_Modbus(_pFrame, ucByteCnt);
        _pFrame[ucByteCnt] = (uint8_t)(usCRC >> 8);
        _pFrame[ucByteCnt + 1] = (uint8_t)(usCRC & 0xFF);
        
        /* 发送响应 */
        RS485_SendBuf(_pFrame, ucByteCnt + 2);
    }
}

/**
  * @brief  处理Modbus写多个寄存器功能
  * @param  _pFrame: 帧数据指针
  * @param  _ucLen: 帧长度
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
    
    /* 检查地址和数量是否有效 */
    if((usAddr <= REG_HOLDING_END) && 
       (usAddr + usNRegs <= REG_HOLDING_END + 1) &&
       (ucByteCnt == usNRegs * 2))
    {
        /* 写入数据 */
        for(uint16_t i = 0; i < usNRegs; i++)
        {
            usRegHoldingBuf[usAddr - REG_HOLDING_START + i] = 
                (((uint16_t)pData[i * 2] << 8) | pData[i * 2 + 1]);
        }
        
        /* 准备响应数据 */
        _pFrame[RS485_DATA_OFFSET] = (uint8_t)(usAddr >> 8);
        _pFrame[RS485_DATA_OFFSET + 1] = (uint8_t)(usAddr & 0xFF);
        _pFrame[RS485_DATA_OFFSET + 2] = (uint8_t)(usNRegs >> 8);
        _pFrame[RS485_DATA_OFFSET + 3] = (uint8_t)(usNRegs & 0xFF);
        
        /* 计算并添加CRC */
        usCRC = CRC16_Modbus(_pFrame, 6);
        _pFrame[6] = (uint8_t)(usCRC >> 8);
        _pFrame[7] = (uint8_t)(usCRC & 0xFF);
        
        /* 发送响应 */
        RS485_SendBuf(_pFrame, 8);
    }
}

/**
  * @brief  处理Modbus读离散输入功能
  * @param  _pFrame: 帧数据指针
  * @param  _ucLen: 帧长度
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
    
    /* 检查地址和数量是否有效 */
    if((usAddr <= DISCRETE_END) && 
       (usAddr + usNCoils <= DISCRETE_END + 1))
    {
        /* 准备响应数据 */
        ucByteCnt = (usNCoils + 7) / 8;  // 计算需要的字节数
        _pFrame[RS485_DATA_OFFSET] = ucByteCnt;
        pData = &_pFrame[RS485_DATA_OFFSET + 1];
        
        /* 复制离散输入状态 */
        for(uint16_t i = 0; i < ucByteCnt; i++)
        {
            pData[i] = 0;  // 清零当前字节
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
        
        /* 计算响应帧长度 */
        ucByteCnt += 3;  // 数据字节数 + 功能码 + 字节计数
        
        /* 添加CRC */
        usCRC = CRC16_Modbus(_pFrame, ucByteCnt);
        _pFrame[ucByteCnt] = (uint8_t)(usCRC >> 8);
        _pFrame[ucByteCnt + 1] = (uint8_t)(usCRC & 0xFF);
        
        /* 发送响应 */
        RS485_SendBuf(_pFrame, ucByteCnt + 2);
    }
}

/**
  * @brief  处理Modbus数据帧
  * @param  _pBuf: 数据缓冲区
  * @param  _usLen: 数据长度
  * @retval None
  */
void RS485_ProcessData(uint8_t *_pBuf, uint16_t _usLen)
{
    uint8_t ucFuncCode = _pBuf[RS485_FUNC_OFFSET];
    
    switch(ucFuncCode)
    {
        case MODBUS_FUNC_READ_COILS:      // 读线圈
            MB_ReadCoils(_pBuf, _usLen);
            break;
            
        case MODBUS_FUNC_READ_DISCRETE:   // 读离散输入
            MB_ReadDiscreteInputs(_pBuf, _usLen);
            break;
            
        case MODBUS_FUNC_READ_INPUT:      // 读输入寄存器
            MB_ReadInputRegister(_pBuf, _usLen);
            break;
            
        case MODBUS_FUNC_READ_HOLDING:    // 读保持寄存器
            MB_ReadRegister(_pBuf, _usLen);
            break;
            
        case MODBUS_FUNC_WRITE_COIL:      // 写单个线圈
            MB_WriteCoil(_pBuf, _usLen);
            break;
            
        case MODBUS_FUNC_WRITE_HOLDING:   // 写单个保持寄存器
            MB_WriteRegister(_pBuf, _usLen);
            break;
            
        case MODBUS_FUNC_WRITE_HOLDINGS:  // 写多个保持寄存器
            MB_WriteMultipleRegisters(_pBuf, _usLen);
            break;
            
        default:
            /* 不支持的功能码，返回错误响应 */
            _pBuf[RS485_FUNC_OFFSET] |= 0x80;   // 设置错误标志
            _pBuf[RS485_DATA_OFFSET] = MODBUS_ERR_FUNC;
            
            /* 计算并添加CRC */
            uint16_t usCRC = CRC16_Modbus(_pBuf, 3);
            _pBuf[3] = (uint8_t)(usCRC >> 8);
            _pBuf[4] = (uint8_t)(usCRC & 0xFF);
            
            /* 发送错误响应 */
            RS485_SendBuf(_pBuf, 5);
            break;
    }
}

