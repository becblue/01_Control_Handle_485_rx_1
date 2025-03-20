#ifndef __OLED_H
#define __OLED_H

/* HAL库头文件 */
#include "stm32f1xx_hal.h"
#include "main.h"

/* 标准库头文件 */
#include <stdint.h>
#include <stddef.h>

/* 应用层头文件 */
#include "sys_status.h"  // 添加这行，用于获取系统状态函数声明

// OLED参数定义
#define OLED_ADDRESS    0x78  // OLED I2C地址
#define OLED_WIDTH      128   // OLED宽度
#define OLED_HEIGHT     64    // OLED高度
#define OLED_PAGE_NUM   8     // OLED页数（64/8=8页）

// OLED控制命令
#define OLED_CMD_SET_CONTRAST        0x81  // 设置对比度
#define OLED_CMD_DISPLAY_ON          0xAF  // 显示开
#define OLED_CMD_DISPLAY_OFF         0xAE  // 显示关
#define OLED_CMD_SET_DISPLAY_OFFSET  0xD3  // 设置显示偏移
#define OLED_CMD_SET_COM_PINS        0xDA  // 设置COM引脚配置

// 函数声明
HAL_StatusTypeDef OLED_Init(void);  // 修改返回类型
void OLED_Clear(void);                     // 清屏
void OLED_Display_On(void);               // 开启显示
void OLED_Display_Off(void);              // 关闭显示
void OLED_Set_Pos(uint8_t x, uint8_t y);  // 设置光标位置
void OLED_ShowChar(uint8_t x, uint8_t y, char chr); // 显示一个字符
void OLED_ShowString(uint8_t x, uint8_t y, const char *str); // 显示字符串
void OLED_Refresh(void);                   // 刷新显示

// 数字显示函数
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len);  // 显示数字
void OLED_ShowHexNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len);  // 显示16进制数
void OLED_ShowBinNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len);  // 显示2进制数

// 状态显示函数
void OLED_ShowTime(uint8_t x, uint8_t y, uint8_t hour, uint8_t min, uint8_t sec);  // 显示时间
void OLED_ShowCPU(uint8_t x, uint8_t y, uint8_t usage);  // 显示CPU使用率
void OLED_ShowKeys(uint8_t x, uint8_t y, uint8_t keys);  // 显示按键状态
void OLED_ShowLEDs(uint8_t x, uint8_t y, uint8_t leds);  // 显示LED状态
void OLED_ShowComm(uint8_t x, uint8_t y, uint8_t status);  // 显示通信状态
void OLED_ShowError(uint8_t x, uint8_t y, uint16_t count);  // 显示错误计数

// 布局管理函数
void OLED_UpdateDisplay(void);  // 更新所有显示内容

// 显示缓冲区
extern uint8_t OLED_GRAM[OLED_PAGE_NUM][OLED_WIDTH];

extern I2C_HandleTypeDef hi2c1;  // I2C句柄声明

// 数学函数声明
uint32_t oled_pow(uint8_t m, uint8_t n);  // 用于数字显示的幂函数

// 字体数组声明
extern const uint8_t F6x8[][6];  // 6x8字体

extern RTC_HandleTypeDef hrtc;  // RTC句柄声明

#endif 

