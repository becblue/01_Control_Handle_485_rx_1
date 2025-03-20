#ifndef __SYS_STATUS_H
#define __SYS_STATUS_H

/* ��׼��ͷ�ļ� */
#include <stdint.h>
#include <stddef.h>

/* HAL��ͷ�ļ� */
#include "stm32f1xx_hal.h"
#include "main.h"

// ϵͳ״̬��ȡ����
uint8_t Get_CPU_Usage(void);      // ��ȡCPUʹ����
uint8_t Get_Key_Status(void);     // ��ȡ����״̬
uint8_t Get_LED_Status(void);     // ��ȡLED״̬
uint8_t Get_Comm_Status(void);    // ��ȡͨ��״̬
uint16_t Get_Error_Count(void);   // ��ȡ�������

// ϵͳ״̬���ú���
void Set_Comm_Status(uint8_t status);      // ����ͨ��״̬
void Increment_Error_Count(void);          // ���Ӵ������
void Update_Idle_Time(void);              // ���¿���ʱ�����

// ϵͳ״̬��ʼ��
void Sys_Status_Init(void);

// ͨ��״̬����
#define COMM_STATUS_OK     0      // ͨ������
#define COMM_STATUS_ERROR  1      // ͨ�Ŵ���
#define COMM_STATUS_WAIT   2      // �ȴ�ͨ��

#endif 

