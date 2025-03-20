#ifndef __SYS_STATUS_H
#define __SYS_STATUS_H

/* 标准库头文件 */
#include <stdint.h>
#include <stddef.h>

/* HAL库头文件 */
#include "stm32f1xx_hal.h"
#include "main.h"

// 系统状态获取函数
uint8_t Get_CPU_Usage(void);      // 获取CPU使用率
uint8_t Get_Key_Status(void);     // 获取按键状态
uint8_t Get_LED_Status(void);     // 获取LED状态
uint8_t Get_Comm_Status(void);    // 获取通信状态
uint16_t Get_Error_Count(void);   // 获取错误计数

// 系统状态设置函数
void Set_Comm_Status(uint8_t status);      // 设置通信状态
void Increment_Error_Count(void);          // 增加错误计数
void Update_Idle_Time(void);              // 更新空闲时间计数

// 系统状态初始化
void Sys_Status_Init(void);

// 通信状态定义
#define COMM_STATUS_OK     0      // 通信正常
#define COMM_STATUS_ERROR  1      // 通信错误
#define COMM_STATUS_WAIT   2      // 等待通信

#endif 

