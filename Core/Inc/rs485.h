#ifndef __RS485_H
#define __RS485_H

#include "main.h"
#include "usart.h"

/* RS485接收缓冲区定义 */
#define RS485_RX_BUF_SIZE   256     // 接收缓冲区大小
#define RS485_TX_BUF_SIZE   256     // 发送缓冲区大小

/* RS485方向控制函数 */
void RS485_Init(void);              // RS485初始化
void RS485_SendBuf(uint8_t *_ucaBuf, uint16_t _usLen);  // 发送数据
void RS485_ReciveNew(uint8_t *buf, uint16_t len);  // 修改函数声明，增加参数

/* 外部变量声明 */
extern uint8_t g_RS485_RxBuf[RS485_RX_BUF_SIZE];  // 接收缓冲区
extern uint16_t g_RS485_RxCount;                   // 接收计数器
extern uint8_t g_RS485_Frame_Flag;                 // 帧接收完成标志
extern uint8_t g_lastRecvData[RS485_RX_BUF_SIZE]; // 最后一次接收的数据
extern uint16_t g_recvLen;                         // 接收到的数据长度
extern uint8_t rx_buffer[256];  // 声明外部变量
extern uint16_t rx_count;       // 声明外部变量
extern uint32_t g_lastRecvTime;  // 声明最后接收时间变量

/* CRC校验相关函数声明 */
uint16_t CRC16_Modbus(uint8_t *_pBuf, uint16_t _usLen);  // CRC16校验计算
uint8_t RS485_VerifyCRC(uint8_t *_pBuf, uint16_t _usLen); // 验证接收数据的CRC

/* RS485帧格式定义 */
#define RS485_ADDR_OFFSET   0    // 地址偏移
#define RS485_FUNC_OFFSET   1    // 功能码偏移
#define RS485_DATA_OFFSET   2    // 数据偏移
#define RS485_CRC_LENGTH    2    // CRC长度
#define RS485_MIN_FRAME_LEN 4    // 最小帧长度(地址+功能码+CRC)

/* Modbus功能码定义 */
#define MODBUS_FUNC_READ_COILS          0x01  // 读线圈
#define MODBUS_FUNC_READ_DISCRETE       0x02  // 读离散输入
#define MODBUS_FUNC_READ_HOLDING        0x03  // 读保持寄存器
#define MODBUS_FUNC_READ_INPUT          0x04  // 读输入寄存器
#define MODBUS_FUNC_WRITE_COIL          0x05  // 写单个线圈
#define MODBUS_FUNC_WRITE_HOLDING       0x06  // 写单个保持寄存器
#define MODBUS_FUNC_WRITE_COILS         0x0F  // 写多个线圈
#define MODBUS_FUNC_WRITE_HOLDINGS      0x10  // 写多个保持寄存器

/* Modbus错误码定义 */
#define MODBUS_ERR_NONE                 0x00  // 无错误
#define MODBUS_ERR_FUNC                 0x01  // 功能码错误
#define MODBUS_ERR_ADDR                 0x02  // 地址错误
#define MODBUS_ERR_VALUE                0x03  // 数据值错误
#define MODBUS_ERR_ERROR                0x04  // 执行错误

/* Modbus处理函数声明 */
void RS485_ProcessData(uint8_t *_pBuf, uint16_t _usLen);

/* RS485处理函数声明 */
void RS485_Handler(void);              // RS485数据处理函数

/* RS485测试相关函数 */
void RS485_EchoTest(uint8_t *data, uint16_t len);  // 回环测试函数
void RS485_SendTestData(void);              // 发送测试数据
void RS485_DisplayStatus(void);             // 显示通信状态

/* RS485显示相关函数 */
void RS485_DisplayData(void);              // 显示接收到的数据

#endif /* __RS485_H */

