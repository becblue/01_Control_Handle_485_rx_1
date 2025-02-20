#ifndef __KEY_H
#define __KEY_H

#include "main.h"

/* ����״̬���� */
#define KEY_PRESSED    1   // �������£��ߵ�ƽ��
#define KEY_RELEASED   0   // �����ͷţ��͵�ƽ��

/* ������Ŷ��� */
#define KEY1    0              // KEY1����
#define KEY2    1              // KEY2����
#define KEY_NUM 2              // ��������

/* ��������ʱ��(ms) */
#define KEY_DEBOUNCE_TIME  20  // ����ʱ��20ms

/* �������� */
void KEY_Init(void);                       // ������ʼ��
uint8_t KEY_Scan(uint8_t key_id);         // ����ɨ��
void KEY_Handler(void);                    // ����������
uint8_t KEY_GetState(uint8_t key_id);     // ��ȡ����״̬

#endif /* __KEY_H */ 

