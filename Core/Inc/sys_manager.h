#ifndef __SYS_MANAGER_H
#define __SYS_MANAGER_H

/* HAL��ͷ�ļ� */
#include "stm32f1xx_hal.h"
#include "main.h"

/* ��׼��ͷ�ļ� */
#include <stdint.h>

/* Ӧ�ò�ͷ�ļ� */
#include "led.h"
#include "oled.h"
#include "rs485.h"
#include "sys_status.h"
#include "rtc.h"

// ϵͳ��ʼ��
HAL_StatusTypeDef Sys_Init(void);

// ϵͳ����
void Sys_Run(void);

// ϵͳ������
void Sys_Error_Handler(void);

// ϵͳ״̬����
void Sys_Update_Status(void);

#endif 

