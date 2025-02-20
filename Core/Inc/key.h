#ifndef __KEY_H
#define __KEY_H

#include "main.h"

/* 按键状态定义 */
#define KEY_PRESSED    1   // 按键按下（高电平）
#define KEY_RELEASED   0   // 按键释放（低电平）

/* 按键编号定义 */
#define KEY1    0              // KEY1按键
#define KEY2    1              // KEY2按键
#define KEY_NUM 2              // 按键总数

/* 按键消抖时间(ms) */
#define KEY_DEBOUNCE_TIME  20  // 消抖时间20ms

/* 函数声明 */
void KEY_Init(void);                       // 按键初始化
uint8_t KEY_Scan(uint8_t key_id);         // 按键扫描
void KEY_Handler(void);                    // 按键处理函数
uint8_t KEY_GetState(uint8_t key_id);     // 获取按键状态

#endif /* __KEY_H */ 

