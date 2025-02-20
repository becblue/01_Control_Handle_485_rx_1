#ifndef __RS485_H
#define __RS485_H

#include "main.h"
#include "usart.h"

/* RS485���ջ��������� */
#define RS485_RX_BUF_SIZE   256     // ���ջ�������С
#define RS485_TX_BUF_SIZE   256     // ���ͻ�������С

/* RS485������ƺ��� */
void RS485_Init(void);              // RS485��ʼ��
void RS485_SendBuf(uint8_t *_ucaBuf, uint16_t _usLen);  // ��������
void RS485_ReciveNew(uint8_t *buf, uint16_t len);  // �޸ĺ������������Ӳ���

/* �ⲿ�������� */
extern uint8_t g_RS485_RxBuf[RS485_RX_BUF_SIZE];  // ���ջ�����
extern uint16_t g_RS485_RxCount;                   // ���ռ�����
extern uint8_t g_RS485_Frame_Flag;                 // ֡������ɱ�־
extern uint8_t g_lastRecvData[RS485_RX_BUF_SIZE]; // ���һ�ν��յ�����
extern uint16_t g_recvLen;                         // ���յ������ݳ���
extern uint8_t rx_buffer[256];  // �����ⲿ����
extern uint16_t rx_count;       // �����ⲿ����
extern uint32_t g_lastRecvTime;  // ����������ʱ�����

/* CRCУ����غ������� */
uint16_t CRC16_Modbus(uint8_t *_pBuf, uint16_t _usLen);  // CRC16У�����
uint8_t RS485_VerifyCRC(uint8_t *_pBuf, uint16_t _usLen); // ��֤�������ݵ�CRC

/* RS485֡��ʽ���� */
#define RS485_ADDR_OFFSET   0    // ��ַƫ��
#define RS485_FUNC_OFFSET   1    // ������ƫ��
#define RS485_DATA_OFFSET   2    // ����ƫ��
#define RS485_CRC_LENGTH    2    // CRC����
#define RS485_MIN_FRAME_LEN 4    // ��С֡����(��ַ+������+CRC)

/* Modbus�����붨�� */
#define MODBUS_FUNC_READ_COILS          0x01  // ����Ȧ
#define MODBUS_FUNC_READ_DISCRETE       0x02  // ����ɢ����
#define MODBUS_FUNC_READ_HOLDING        0x03  // �����ּĴ���
#define MODBUS_FUNC_READ_INPUT          0x04  // ������Ĵ���
#define MODBUS_FUNC_WRITE_COIL          0x05  // д������Ȧ
#define MODBUS_FUNC_WRITE_HOLDING       0x06  // д�������ּĴ���
#define MODBUS_FUNC_WRITE_COILS         0x0F  // д�����Ȧ
#define MODBUS_FUNC_WRITE_HOLDINGS      0x10  // д������ּĴ���

/* Modbus�����붨�� */
#define MODBUS_ERR_NONE                 0x00  // �޴���
#define MODBUS_ERR_FUNC                 0x01  // ���������
#define MODBUS_ERR_ADDR                 0x02  // ��ַ����
#define MODBUS_ERR_VALUE                0x03  // ����ֵ����
#define MODBUS_ERR_ERROR                0x04  // ִ�д���

/* Modbus���������� */
void RS485_ProcessData(uint8_t *_pBuf, uint16_t _usLen);

/* RS485���������� */
void RS485_Handler(void);              // RS485���ݴ�����

/* RS485������غ��� */
void RS485_EchoTest(uint8_t *data, uint16_t len);  // �ػ����Ժ���
void RS485_SendTestData(void);              // ���Ͳ�������
void RS485_DisplayStatus(void);             // ��ʾͨ��״̬

/* RS485��ʾ��غ��� */
void RS485_DisplayData(void);              // ��ʾ���յ�������

#endif /* __RS485_H */

