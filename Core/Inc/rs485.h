#ifndef __RS485_H
#define __RS485_H

/* ��׼��ͷ�ļ� */
#include <stdint.h>
#include <string.h>

/* HAL��ͷ�ļ� */
#include "stm32f1xx_hal.h"
#include "main.h"
#include "usart.h"
#include "rtc.h"

/* Ӧ�ò�ͷ�ļ� */
#include "sys_status.h"
#include "led.h"

// RS485���Ŷ���
#define RS485_DE_GPIO_Port    GPIOB
#define RS485_DE_Pin         GPIO_PIN_8
#define RS485_TX_GPIO_Port   GPIOB
#define RS485_TX_Pin        GPIO_PIN_11
#define RS485_RX_GPIO_Port   GPIOB
#define RS485_RX_Pin        GPIO_PIN_10

// RS485������ƺ궨��
#define RS485_RX_MODE()      do { \
                                HAL_GPIO_WritePin(RS485_DE_GPIO_Port, RS485_DE_Pin, GPIO_PIN_RESET); /* �͵�ƽ=���� */ \
                                HAL_Delay(1); /* �ȴ��л���� */ \
                            } while(0)

#define RS485_TX_MODE()      do { \
                                HAL_GPIO_WritePin(RS485_DE_GPIO_Port, RS485_DE_Pin, GPIO_PIN_SET); /* �ߵ�ƽ=���� */ \
                                HAL_Delay(1); /* �ȴ��л���� */ \
                            } while(0)

// RS485��������С����
#define RS485_RX_BUF_SIZE    64
#define RS485_TX_BUF_SIZE    64

// ֡��ʽ����
#define FRAME_HEADER     0xAA    // ֡ͷ
#define FRAME_TAIL       0x55    // ֡β
#define DEVICE_ADDR      0x02    // ������ַ

// �����붨��
#define FUNC_READ_STATUS 0x03    // ��ȡ״̬
#define FUNC_SET_LED     0x06    // ����LED
#define FUNC_GET_TIME    0x04    // ��ȡʱ��
#define FUNC_SET_TIME    0x10    // ����ʱ��

// ֡��ʽ�ṹ��
typedef struct {
    uint8_t header;    // ֡ͷ 0xAA
    uint8_t cmd;       // ����
    uint8_t len;       // ���ݳ���
    uint8_t data[32];  // ����
    uint8_t check;     // У���
    uint8_t tail;      // ֡β 0x55
} RS485_Frame_t;

// RS485��ʼ������
void RS485_Init(void);

// RS485���ͺ���
HAL_StatusTypeDef RS485_Transmit(uint8_t *data, uint16_t size, uint32_t timeout);

// RS485���պ���
HAL_StatusTypeDef RS485_Receive(uint8_t *data, uint16_t size, uint32_t timeout);

// RS485�жϻص�����
void RS485_RxCpltCallback(void);

// RS485������������ѭ���е��ã�
void RS485_Process(void);

// Э�鴦����
HAL_StatusTypeDef RS485_SendFrame(uint8_t cmd, uint8_t *data, uint8_t len);
HAL_StatusTypeDef RS485_ParseFrame(void);

#endif 

