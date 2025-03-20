#include "led.h"

// LED配置结构体
typedef struct {
    GPIO_TypeDef* port;    // LED端口
    uint16_t pin;          // LED引脚
    LED_State state;       // LED当前状态
    uint32_t timer;        // LED计时器
    uint8_t toggle;        // LED翻转状态
} LED_Config;

// LED配置数组
static LED_Config leds[] = {
    {GPIOA, GPIO_PIN_1, LED_OFF, 0, 0},  // LED_R
    {GPIOA, GPIO_PIN_2, LED_OFF, 0, 0},  // LED_G
    {GPIOA, GPIO_PIN_3, LED_OFF, 0, 0}   // LED_B
};

// LED闪烁参数定义
#define LED_FAST_BLINK_INTERVAL  100   // 快闪间隔(ms)
#define LED_SLOW_BLINK_INTERVAL  500   // 慢闪间隔(ms)
#define LED_SINGLE_BLINK_TIME    50    // 单闪时间(ms)

// LED初始化
void LED_Init(void)
{
    // LED引脚已在CubeMX中配置为推挽输出
    // 初始化时关闭所有LED
    for(uint8_t i = 0; i < sizeof(leds)/sizeof(leds[0]); i++) {
        HAL_GPIO_WritePin(leds[i].port, leds[i].pin, GPIO_PIN_SET);
        leds[i].state = LED_OFF;
        leds[i].timer = 0;
        leds[i].toggle = 0;
    }
}

// 设置LED状态
void LED_SetState(uint8_t led, LED_State state)
{
    if(led < sizeof(leds)/sizeof(leds[0])) {
        leds[led].state = state;
        leds[led].timer = HAL_GetTick();
        
        // 如果设置为常亮或常灭，直接执行
        if(state == LED_ON) {
            HAL_GPIO_WritePin(leds[led].port, leds[led].pin, GPIO_PIN_RESET);
        }
        else if(state == LED_OFF) {
            HAL_GPIO_WritePin(leds[led].port, leds[led].pin, GPIO_PIN_SET);
        }
    }
}

// LED处理函数（在主循环中调用）
void LED_Process(void)
{
    uint32_t current_time = HAL_GetTick();
    
    for(uint8_t i = 0; i < sizeof(leds)/sizeof(leds[0]); i++) {
        switch(leds[i].state) {
            case LED_FAST_BLINK:
                if(current_time - leds[i].timer >= LED_FAST_BLINK_INTERVAL) {
                    leds[i].toggle = !leds[i].toggle;
                    HAL_GPIO_WritePin(leds[i].port, leds[i].pin, leds[i].toggle ? GPIO_PIN_RESET : GPIO_PIN_SET);
                    leds[i].timer = current_time;
                }
                break;
                
            case LED_SLOW_BLINK:
                if(current_time - leds[i].timer >= LED_SLOW_BLINK_INTERVAL) {
                    leds[i].toggle = !leds[i].toggle;
                    HAL_GPIO_WritePin(leds[i].port, leds[i].pin, leds[i].toggle ? GPIO_PIN_RESET : GPIO_PIN_SET);
                    leds[i].timer = current_time;
                }
                break;
                
            case LED_SINGLE_BLINK:
                if(current_time - leds[i].timer >= LED_SINGLE_BLINK_TIME) {
                    HAL_GPIO_WritePin(leds[i].port, leds[i].pin, GPIO_PIN_SET);
                    leds[i].state = LED_OFF;
                }
                break;
                
            default:
                break;
        }
    }
}

// LED单闪
void LED_BlinkOnce(uint8_t led)
{
    if(led < sizeof(leds)/sizeof(leds[0])) {
        leds[led].state = LED_SINGLE_BLINK;
        leds[led].timer = HAL_GetTick();
        HAL_GPIO_WritePin(leds[led].port, leds[led].pin, GPIO_PIN_RESET);
    }
} 

