#ifndef __LED_H
#define __LED_H

/* 添加必要的头文件 */
#include "main.h"
#include <stddef.h>
#include <stdint.h>
#include "stm32f1xx_hal.h"  // HAL库头文件

// LED 状态枚举
typedef enum {
    LED_OFF = 0,        // LED关闭
    LED_ON,             // LED常亮
    LED_FAST_BLINK,     // LED快闪
    LED_SLOW_BLINK,     // LED慢闪
    LED_SINGLE_BLINK    // LED单次闪烁
} LED_State;

// LED控制函数声明
void LED_Init(void);                    // LED初始化
void LED_SetState(uint8_t led, LED_State state);  // 设置LED状态
void LED_Process(void);                 // LED状态处理（在主循环中调用）
void LED_BlinkOnce(uint8_t led);       // LED单闪

#endif 


