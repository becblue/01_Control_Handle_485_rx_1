#ifndef __LED_H
#define __LED_H

/* ��ӱ�Ҫ��ͷ�ļ� */
#include "main.h"
#include <stddef.h>
#include <stdint.h>
#include "stm32f1xx_hal.h"  // HAL��ͷ�ļ�

// LED ״̬ö��
typedef enum {
    LED_OFF = 0,        // LED�ر�
    LED_ON,             // LED����
    LED_FAST_BLINK,     // LED����
    LED_SLOW_BLINK,     // LED����
    LED_SINGLE_BLINK    // LED������˸
} LED_State;

// LED���ƺ�������
void LED_Init(void);                    // LED��ʼ��
void LED_SetState(uint8_t led, LED_State state);  // ����LED״̬
void LED_Process(void);                 // LED״̬��������ѭ���е��ã�
void LED_BlinkOnce(uint8_t led);       // LED����

#endif 


