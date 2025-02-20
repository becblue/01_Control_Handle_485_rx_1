#include "led.h"

/* LED状态记录 */
static uint8_t led_state[LED_NUM] = {LED_OFF, LED_OFF, LED_OFF};

/**
  * @brief  LED初始化
  * @param  None
  * @retval None
  */
void LED_Init(void)
{
    /* 初始化所有LED为熄灭状态 */
    HAL_GPIO_WritePin(Led_1_GPIO_Port, Led_1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(Led_2_GPIO_Port, Led_2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(Led_1A3_GPIO_Port, Led_1A3_Pin, GPIO_PIN_SET);
    
    /* 初始化状态变量 */
    for(uint8_t i = 0; i < LED_NUM; i++)
    {
        led_state[i] = LED_OFF;
    }
}

/**
  * @brief  设置LED状态
  * @param  led_id: LED编号
  * @param  state: LED状态(LED_ON/LED_OFF)
  * @retval None
  */
void LED_SetState(uint8_t led_id, uint8_t state)
{
    if(led_id < LED_NUM)
    {
        led_state[led_id] = state;
        GPIO_PinState pin_state = (state == LED_ON) ? GPIO_PIN_RESET : GPIO_PIN_SET;
        
        switch(led_id)
        {
            case LED_R:
                HAL_GPIO_WritePin(Led_1_GPIO_Port, Led_1_Pin, pin_state);
                break;
            
            case LED_G:
                HAL_GPIO_WritePin(Led_2_GPIO_Port, Led_2_Pin, pin_state);
                break;
            
            case LED_B:
                HAL_GPIO_WritePin(Led_1A3_GPIO_Port, Led_1A3_Pin, pin_state);
                break;
            
            default:
                break;
        }
    }
}

/**
  * @brief  获取LED状态
  * @param  led_id: LED编号
  * @retval LED状态(LED_ON/LED_OFF)
  */
uint8_t LED_GetState(uint8_t led_id)
{
    if(led_id < LED_NUM)
    {
        return led_state[led_id];
    }
    return LED_OFF;
}

/**
  * @brief  LED状态翻转
  * @param  led_id: LED编号
  * @retval None
  */
void LED_Toggle(uint8_t led_id)
{
    if(led_id < LED_NUM)
    {
        led_state[led_id] = !led_state[led_id];
        
        switch(led_id)
        {
            case LED_R:
                HAL_GPIO_TogglePin(Led_1_GPIO_Port, Led_1_Pin);
                break;
            
            case LED_G:
                HAL_GPIO_TogglePin(Led_2_GPIO_Port, Led_2_Pin);
                break;
            
            case LED_B:
                HAL_GPIO_TogglePin(Led_1A3_GPIO_Port, Led_1A3_Pin);
                break;
            
            default:
                break;
        }
    }
} 

