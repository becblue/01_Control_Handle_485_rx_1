#ifndef __OLED_H
#define __OLED_H

/* HAL��ͷ�ļ� */
#include "stm32f1xx_hal.h"
#include "main.h"

/* ��׼��ͷ�ļ� */
#include <stdint.h>
#include <stddef.h>

/* Ӧ�ò�ͷ�ļ� */
#include "sys_status.h"  // ������У����ڻ�ȡϵͳ״̬��������

// OLED��������
#define OLED_ADDRESS    0x78  // OLED I2C��ַ
#define OLED_WIDTH      128   // OLED���
#define OLED_HEIGHT     64    // OLED�߶�
#define OLED_PAGE_NUM   8     // OLEDҳ����64/8=8ҳ��

// OLED��������
#define OLED_CMD_SET_CONTRAST        0x81  // ���öԱȶ�
#define OLED_CMD_DISPLAY_ON          0xAF  // ��ʾ��
#define OLED_CMD_DISPLAY_OFF         0xAE  // ��ʾ��
#define OLED_CMD_SET_DISPLAY_OFFSET  0xD3  // ������ʾƫ��
#define OLED_CMD_SET_COM_PINS        0xDA  // ����COM��������

// ��������
HAL_StatusTypeDef OLED_Init(void);  // �޸ķ�������
void OLED_Clear(void);                     // ����
void OLED_Display_On(void);               // ������ʾ
void OLED_Display_Off(void);              // �ر���ʾ
void OLED_Set_Pos(uint8_t x, uint8_t y);  // ���ù��λ��
void OLED_ShowChar(uint8_t x, uint8_t y, char chr); // ��ʾһ���ַ�
void OLED_ShowString(uint8_t x, uint8_t y, const char *str); // ��ʾ�ַ���
void OLED_Refresh(void);                   // ˢ����ʾ

// ������ʾ����
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len);  // ��ʾ����
void OLED_ShowHexNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len);  // ��ʾ16������
void OLED_ShowBinNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len);  // ��ʾ2������

// ״̬��ʾ����
void OLED_ShowTime(uint8_t x, uint8_t y, uint8_t hour, uint8_t min, uint8_t sec);  // ��ʾʱ��
void OLED_ShowCPU(uint8_t x, uint8_t y, uint8_t usage);  // ��ʾCPUʹ����
void OLED_ShowKeys(uint8_t x, uint8_t y, uint8_t keys);  // ��ʾ����״̬
void OLED_ShowLEDs(uint8_t x, uint8_t y, uint8_t leds);  // ��ʾLED״̬
void OLED_ShowComm(uint8_t x, uint8_t y, uint8_t status);  // ��ʾͨ��״̬
void OLED_ShowError(uint8_t x, uint8_t y, uint16_t count);  // ��ʾ�������

// ���ֹ�����
void OLED_UpdateDisplay(void);  // ����������ʾ����

// ��ʾ������
extern uint8_t OLED_GRAM[OLED_PAGE_NUM][OLED_WIDTH];

extern I2C_HandleTypeDef hi2c1;  // I2C�������

// ��ѧ��������
uint32_t oled_pow(uint8_t m, uint8_t n);  // ����������ʾ���ݺ���

// ������������
extern const uint8_t F6x8[][6];  // 6x8����

extern RTC_HandleTypeDef hrtc;  // RTC�������

#endif 

