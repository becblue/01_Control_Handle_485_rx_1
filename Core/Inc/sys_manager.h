#ifndef __SYS_MANAGER_H
#define __SYS_MANAGER_H

/* HAL库头文件 */
#include "stm32f1xx_hal.h"
#include "main.h"

/* 标准库头文件 */
#include <stdint.h>

/* 应用层头文件 */
#include "led.h"
#include "oled.h"
#include "rs485.h"
#include "sys_status.h"
#include "rtc.h"

// 系统初始化
HAL_StatusTypeDef Sys_Init(void);

// 系统运行
void Sys_Run(void);

// 系统错误处理
void Sys_Error_Handler(void);

// 系统状态更新
void Sys_Update_Status(void);

#endif 

