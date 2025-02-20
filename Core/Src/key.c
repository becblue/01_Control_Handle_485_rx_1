#include "key.h"

/* 按键状态记录 */
static uint8_t key_state[KEY_NUM] = {0};           // 当前状态
static uint8_t key_last_state[KEY_NUM] = {0};      // 上一次状态
static uint32_t key_debounce_time[KEY_NUM] = {0};  // 消抖计时

/**
  * @brief  按键初始化
  * @param  None
  * @retval None
  */
void KEY_Init(void)
{
    /* 状态变量初始化 */
    for(uint8_t i = 0; i < KEY_NUM; i++)
    {
        key_state[i] = KEY_RELEASED;
        key_last_state[i] = KEY_RELEASED;
        key_debounce_time[i] = 0;
    }
}

/**
  * @brief  按键扫描
  * @param  key_id: 按键编号
  * @retval 按键状态：KEY_PRESSED/KEY_RELEASED
  */
uint8_t KEY_Scan(uint8_t key_id)
{
    uint8_t key_current = KEY_RELEASED;
    
    /* 读取当前按键电平 */
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
    
    /* 状态判断与消抖 */
    if(key_current != key_last_state[key_id])
    {
        /* 状态发生变化，记录时间 */
        key_debounce_time[key_id] = HAL_GetTick();
    }
    else
    {
        /* 状态稳定，判断消抖时间 */
        if(HAL_GetTick() - key_debounce_time[key_id] >= KEY_DEBOUNCE_TIME)
        {
            if(key_current != key_state[key_id])  // 状态确实发生了变化
            {
                key_state[key_id] = key_current;
                
                /* 输出5ms高电平脉冲 */
                HAL_GPIO_WritePin(Key_Status_GPIO_Port, Key_Status_Pin, GPIO_PIN_SET);
                HAL_Delay(5);  // 延时5ms
                HAL_GPIO_WritePin(Key_Status_GPIO_Port, Key_Status_Pin, GPIO_PIN_RESET);
            }
        }
    }
    
    key_last_state[key_id] = key_current;
    return key_state[key_id];
}

/**
  * @brief  按键处理函数
  * @param  None
  * @retval None
  */
void KEY_Handler(void)
{
    /* 扫描所有按键 */
    for(uint8_t i = 0; i < KEY_NUM; i++)
    {
        KEY_Scan(i);
    }
}

/**
  * @brief  获取按键状态
  * @param  key_id: 按键编号
  * @retval 按键状态：KEY_PRESSED/KEY_RELEASED
  */
uint8_t KEY_GetState(uint8_t key_id)
{
    if(key_id < KEY_NUM)
    {
        return key_state[key_id];
    }
    return KEY_RELEASED;
} 

