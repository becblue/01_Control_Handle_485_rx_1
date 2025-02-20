#ifndef __LED_H
#define __LED_H

#include "main.h"

/* LED��Ŷ��� */
#define LED_R    0    // ��ɫLED
#define LED_G    1    // ��ɫLED
#define LED_B    2    // ��ɫLED
#define LED_NUM  3    // LED����

/* LED״̬���� */
#define LED_ON   0    // LED�������͵�ƽ��Ч��
#define LED_OFF  1    // LEDϨ��

/* �������� */
void LED_Init(void);                    // LED��ʼ��
void LED_SetState(uint8_t led_id, uint8_t state);   // ����LED״̬
uint8_t LED_GetState(uint8_t led_id);   // ��ȡLED״̬
void LED_Toggle(uint8_t led_id);        // LED״̬��ת

#endif /* __LED_H */ 


