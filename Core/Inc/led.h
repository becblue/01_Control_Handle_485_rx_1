#ifndef __LED_H
#define __LED_H

#include "main.h"

/* LED编号定义 */
#define LED_R    0    // 红色LED
#define LED_G    1    // 绿色LED
#define LED_B    2    // 蓝色LED
#define LED_NUM  3    // LED总数

/* LED状态定义 */
#define LED_ON   0    // LED点亮（低电平有效）
#define LED_OFF  1    // LED熄灭

/* 函数声明 */
void LED_Init(void);                    // LED初始化
void LED_SetState(uint8_t led_id, uint8_t state);   // 设置LED状态
uint8_t LED_GetState(uint8_t led_id);   // 获取LED状态
void LED_Toggle(uint8_t led_id);        // LED状态翻转

#endif /* __LED_H */ 


