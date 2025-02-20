#ifndef __OLED_H
#define __OLED_H

#include "main.h"
#include "i2c.h"

/* OLED参数定义 */
#define OLED_I2C_ADDR    0x78        // OLED的I2C地址
#define OLED_WIDTH       128          // OLED宽度
#define OLED_PAGE        8           // OLED页数（高度/8）
#define OLED_I2C_CMD     0x00        // OLED写命令
#define OLED_I2C_DATA    0x40        // OLED写数据

/* OLED控制命令 */
#define OLED_CMD_DISPLAY_OFF   0xAE  // 关闭显示
#define OLED_CMD_DISPLAY_ON    0xAF  // 开启显示
#define OLED_CMD_SET_CONTRAST  0x81  // 设置对比度
#define OLED_CMD_NORMAL_MODE   0xA6  // 正常显示模式
#define OLED_CMD_INVERSE_MODE  0xA7  // 反色显示模式

/* 函数声明 */
void OLED_Init(void);                // OLED初始化
void OLED_Clear(void);               // 清屏
void OLED_Display_On(void);          // 开启显示
void OLED_Display_Off(void);         // 关闭显示
void OLED_WriteCmd(uint8_t cmd);     // 写命令
void OLED_WriteData(uint8_t data);   // 写数据
void OLED_SetPosition(uint8_t x, uint8_t y); // 设置显示位置
void OLED_ShowChar(uint8_t x, uint8_t y, char chr); // 显示一个字符
void OLED_ShowString(uint8_t x, uint8_t y, const char *str); // 显示字符串
void OLED_SetContrast(uint8_t contrast); // 设置显示对比度，范围0x00-0xFF

#endif /* __OLED_H */ 

