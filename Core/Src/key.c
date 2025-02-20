#include "key.h"

/* ����״̬��¼ */
static uint8_t key_state[KEY_NUM] = {0};           // ��ǰ״̬
static uint8_t key_last_state[KEY_NUM] = {0};      // ��һ��״̬
static uint32_t key_debounce_time[KEY_NUM] = {0};  // ������ʱ

/**
  * @brief  ������ʼ��
  * @param  None
  * @retval None
  */
void KEY_Init(void)
{
    /* ״̬������ʼ�� */
    for(uint8_t i = 0; i < KEY_NUM; i++)
    {
        key_state[i] = KEY_RELEASED;
        key_last_state[i] = KEY_RELEASED;
        key_debounce_time[i] = 0;
    }
}

/**
  * @brief  ����ɨ��
  * @param  key_id: �������
  * @retval ����״̬��KEY_PRESSED/KEY_RELEASED
  */
uint8_t KEY_Scan(uint8_t key_id)
{
    uint8_t key_current = KEY_RELEASED;
    
    /* ��ȡ��ǰ������ƽ */
    if(key_id == KEY1)
    {
        key_current = (HAL_GPIO_ReadPin(Key1_GPIO_Port, Key1_Pin) == GPIO_PIN_SET) ? 
                      KEY_PRESSED : KEY_RELEASED;
    }
    else if(key_id == KEY2)
    {
        key_current = (HAL_GPIO_ReadPin(Key2_GPIO_Port, Key2_Pin) == GPIO_PIN_SET) ? 
                      KEY_PRESSED : KEY_RELEASED;
    }
    
    /* ״̬�ж������� */
    if(key_current != key_last_state[key_id])
    {
        /* ״̬�����仯����¼ʱ�� */
        key_debounce_time[key_id] = HAL_GetTick();
    }
    else
    {
        /* ״̬�ȶ����ж�����ʱ�� */
        if(HAL_GetTick() - key_debounce_time[key_id] >= KEY_DEBOUNCE_TIME)
        {
            if(key_current != key_state[key_id])  // ״̬ȷʵ�����˱仯
            {
                key_state[key_id] = key_current;
                
                /* ���5ms�ߵ�ƽ���� */
                HAL_GPIO_WritePin(Key_Status_GPIO_Port, Key_Status_Pin, GPIO_PIN_SET);
                HAL_Delay(5);  // ��ʱ5ms
                HAL_GPIO_WritePin(Key_Status_GPIO_Port, Key_Status_Pin, GPIO_PIN_RESET);
            }
        }
    }
    
    key_last_state[key_id] = key_current;
    return key_state[key_id];
}

/**
  * @brief  ����������
  * @param  None
  * @retval None
  */
void KEY_Handler(void)
{
    /* ɨ�����а��� */
    for(uint8_t i = 0; i < KEY_NUM; i++)
    {
        KEY_Scan(i);
    }
}

/**
  * @brief  ��ȡ����״̬
  * @param  key_id: �������
  * @retval ����״̬��KEY_PRESSED/KEY_RELEASED
  */
uint8_t KEY_GetState(uint8_t key_id)
{
    if(key_id < KEY_NUM)
    {
        return key_state[key_id];
    }
    return KEY_RELEASED;
} 

