#ifndef __OLED_H
#define __OLED_H

#include "main.h"
#include "i2c.h"

/* OLED�������� */
#define OLED_I2C_ADDR    0x78        // OLED��I2C��ַ
#define OLED_WIDTH       128          // OLED���
#define OLED_PAGE        8           // OLEDҳ�����߶�/8��
#define OLED_I2C_CMD     0x00        // OLEDд����
#define OLED_I2C_DATA    0x40        // OLEDд����

/* OLED�������� */
#define OLED_CMD_DISPLAY_OFF   0xAE  // �ر���ʾ
#define OLED_CMD_DISPLAY_ON    0xAF  // ������ʾ
#define OLED_CMD_SET_CONTRAST  0x81  // ���öԱȶ�
#define OLED_CMD_NORMAL_MODE   0xA6  // ������ʾģʽ
#define OLED_CMD_INVERSE_MODE  0xA7  // ��ɫ��ʾģʽ

/* �������� */
void OLED_Init(void);                // OLED��ʼ��
void OLED_Clear(void);               // ����
void OLED_Display_On(void);          // ������ʾ
void OLED_Display_Off(void);         // �ر���ʾ
void OLED_WriteCmd(uint8_t cmd);     // д����
void OLED_WriteData(uint8_t data);   // д����
void OLED_SetPosition(uint8_t x, uint8_t y); // ������ʾλ��
void OLED_ShowChar(uint8_t x, uint8_t y, char chr); // ��ʾһ���ַ�
void OLED_ShowString(uint8_t x, uint8_t y, const char *str); // ��ʾ�ַ���
void OLED_SetContrast(uint8_t contrast); // ������ʾ�Աȶȣ���Χ0x00-0xFF

#endif /* __OLED_H */ 

