#ifndef __RS485_H
#define __RS485_H

/* 标准库头文件 */
#include <stdint.h>
#include <string.h>

/* HAL库头文件 */
#include "stm32f1xx_hal.h"
#include "main.h"
#include "usart.h"
#include "rtc.h"

/* 应用层头文件 */
#include "sys_status.h"
#include "led.h"

// RS485引脚定义
#define RS485_DE_GPIO_Port    GPIOB
#define RS485_DE_Pin         GPIO_PIN_8
#define RS485_TX_GPIO_Port   GPIOB
#define RS485_TX_Pin        GPIO_PIN_11
#define RS485_RX_GPIO_Port   GPIOB
#define RS485_RX_Pin        GPIO_PIN_10

// RS485方向控制宏定义
#define RS485_RX_MODE()      do { \
                                HAL_GPIO_WritePin(RS485_DE_GPIO_Port, RS485_DE_Pin, GPIO_PIN_RESET); /* 低电平=接收 */ \
                                HAL_Delay(1); /* 等待切换完成 */ \
                            } while(0)

#define RS485_TX_MODE()      do { \
                                HAL_GPIO_WritePin(RS485_DE_GPIO_Port, RS485_DE_Pin, GPIO_PIN_SET); /* 高电平=发送 */ \
                                HAL_Delay(1); /* 等待切换完成 */ \
                            } while(0)

// RS485缓冲区大小定义
#define RS485_RX_BUF_SIZE    64
#define RS485_TX_BUF_SIZE    64

// 帧格式定义
#define FRAME_HEADER     0xAA    // 帧头
#define FRAME_TAIL       0x55    // 帧尾
#define DEVICE_ADDR      0x02    // 本机地址

// 功能码定义
#define FUNC_READ_STATUS 0x03    // 读取状态
#define FUNC_SET_LED     0x06    // 设置LED
#define FUNC_GET_TIME    0x04    // 获取时间
#define FUNC_SET_TIME    0x10    // 设置时间

// 帧格式结构体
typedef struct {
    uint8_t header;    // 帧头 0xAA
    uint8_t cmd;       // 命令
    uint8_t len;       // 数据长度
    uint8_t data[32];  // 数据
    uint8_t check;     // 校验和
    uint8_t tail;      // 帧尾 0x55
} RS485_Frame_t;

// RS485初始化函数
void RS485_Init(void);

// RS485发送函数
HAL_StatusTypeDef RS485_Transmit(uint8_t *data, uint16_t size, uint32_t timeout);

// RS485接收函数
HAL_StatusTypeDef RS485_Receive(uint8_t *data, uint16_t size, uint32_t timeout);

// RS485中断回调函数
void RS485_RxCpltCallback(void);

// RS485处理函数（在主循环中调用）
void RS485_Process(void);

// 协议处理函数
HAL_StatusTypeDef RS485_SendFrame(uint8_t cmd, uint8_t *data, uint8_t len);
HAL_StatusTypeDef RS485_ParseFrame(void);

#endif 

