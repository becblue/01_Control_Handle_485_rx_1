/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "rtc.h"
#include "usart.h"
#include "gpio.h"
#include "sys_manager.h"
#include <stdio.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
extern UART_HandleTypeDef huart1;
uint8_t rxBuffer[UART_RX_BUFFER_SIZE];
volatile uint8_t rxIndex = 0;
volatile ReceiveState rxState = WAITING_HEADER;
uint8_t dataLength = 0;
uint8_t dataIndex = 0;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_RTC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
extern UART_HandleTypeDef huart1;

void USART1_Init(void)
{
    // USART1 GPIO����
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    // ʹ��GPIOAʱ��
    __HAL_RCC_GPIOA_CLK_ENABLE();
    // ʹ��USART1ʱ��
    __HAL_RCC_USART1_CLK_ENABLE();
    
    // ����PA9 (USART1_TX)
    GPIO_InitStruct.Pin = USART1_TX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(USART1_TX_GPIO_Port, &GPIO_InitStruct);
    
    // ����PA10 (USART1_RX)
    GPIO_InitStruct.Pin = USART1_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(USART1_RX_GPIO_Port, &GPIO_InitStruct);
    
    // USART1����
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    
    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        Error_Handler();
    }
    
    // ȷ���ڳ�ʼ����ɺ����������ж�
    HAL_UART_Receive_IT(&huart1, &rxBuffer[0], 1);
    
    // ȷ��USART1�ж�������
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
}

// UART�����жϻص�����
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        printf("Received byte[%d]: 0x%02X\r\n", rxIndex, rxBuffer[rxIndex]);
        
        switch (rxState)
        {
            case WAITING_HEADER:
                if (rxBuffer[rxIndex] == FRAME_HEADER)
                {
                    printf("Header found at index %d\r\n", rxIndex);
                    rxState = RECEIVING_LENGTH;
                    rxIndex = 0;  // ��������
                }
                break;
                
            case RECEIVING_LENGTH:
                dataLength = rxBuffer[rxIndex];
                printf("Length received: %d at index %d\r\n", dataLength, rxIndex);
                if (dataLength > 0 && dataLength <= UART_RX_BUFFER_SIZE - 3)
                {
                    rxState = RECEIVING_DATA;
                    dataIndex = 0;
                }
                else
                {
                    printf("Invalid length, resetting to wait for header\r\n");
                    rxState = WAITING_HEADER;
                }
                break;
                
            case RECEIVING_DATA:
                printf("Data[%d]: 0x%02X\r\n", dataIndex, rxBuffer[rxIndex]);
                dataIndex++;
                if (dataIndex >= dataLength)
                {
                    rxState = WAITING_TAIL;
                }
                break;
                
            case WAITING_TAIL:
                if (rxBuffer[rxIndex] == FRAME_TAIL)
                {
                    printf("Frame complete\r\n");
                    // ��������֡����
                    ProcessReceivedFrame(&rxBuffer[2], dataLength);
                }
                rxState = WAITING_HEADER;
                break;
        }
        
        rxIndex = (rxIndex + 1) % UART_RX_BUFFER_SIZE;
        HAL_UART_Receive_IT(&huart1, &rxBuffer[rxIndex], 1);
    }
}

void ProcessReceivedFrame(uint8_t* data, uint8_t length)
{
    // ��ӵ��Դ�ӡ
    printf("Processing frame: ");
    for(uint8_t i = 0; i < length; i++)
    {
        printf("0x%02X ", data[i]);
    }
    printf("\r\n");

    // ��鹦���������
    if(length >= 2 && data[0] == 0x01)  // ������Ϊ0x01
    {
        if(data[1] == 0x01)  // ��KEY1
        {
            printf("Setting KEY1 ON\r\n");
            // ������Ӵ�KEY1�ľ������
            Set_Comm_Status(COMM_STATUS_OK);  // ����ͨ��״̬
        }
        else if(data[1] == 0x00)  // �ر�KEY1
        {
            printf("Setting KEY1 OFF\r\n");
            // ������ӹر�KEY1�ľ������
            Set_Comm_Status(COMM_STATUS_OK);  // ����ͨ��״̬
        }
    }
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();
    
    /* Configure the system clock */
    SystemClock_Config();
    
    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_I2C1_Init();
    MX_USART1_UART_Init();
    MX_USART3_UART_Init();
    MX_RTC_Init();
    
    printf("\r\n\r\n[System] Starting up...\r\n");
    printf("[System] UART1 initialized and ready for receiving data\r\n");
    printf("[System] Please send data...\r\n");
    
    /* Initialize system */
    if(Sys_Init() != HAL_OK)
    {
        printf("[System] Initialization failed!\r\n");
        Error_Handler();
    }
    printf("[System] Initialization complete\r\n");
    
    printf("\r\n[System] Testing UART1 protocol...\r\n");
    printf("[System] Device address: 0x%02X\r\n", DEVICE_ADDR);
    printf("[System] Available functions:\r\n");
    printf("  - Read Status (0x%02X)\r\n", FUNC_READ_STATUS);
    printf("  - Set LED     (0x%02X)\r\n", FUNC_SET_LED);
    printf("  - Get Time    (0x%02X)\r\n", FUNC_GET_TIME);
    printf("  - Set Time    (0x%02X)\r\n", FUNC_SET_TIME);
    
    /* Infinite loop */
    while (1)
    {
        Sys_Run();
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_2);  // ��תLED2��ʾϵͳ����
        HAL_Delay(500);  // 500ms��˸
    }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    /* ����LED���� */
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* ����USART3���ţ����ʹ�ã� */
    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

static void MX_USART1_UART_Init(void)
{
    // ���������Զ���ĳ�ʼ������
    USART1_Init();
}

static void MX_I2C1_Init(void)
{
    // ������Ҫ����CubeMX���ɵ�I2C��ʼ������
    // ʾ����
    hi2c1.Instance = I2C1;
    hi2c1.Init.ClockSpeed = 100000;
    hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c1) != HAL_OK)
    {
        Error_Handler();
    }
}

static void MX_RTC_Init(void)
{
    RTC_TimeTypeDef sTime = {0};
    RTC_DateTypeDef sDate = {0};

    hrtc.Instance = RTC;
    hrtc.Init.AsynchPrediv = 0x7F;   // ���������첽Ԥ��Ƶ
    if (HAL_RTC_Init(&hrtc) != HAL_OK)
    {
        Error_Handler();
    }

    // ���ó�ʼʱ�䣨STM32F1ר�ø�ʽ��
    sTime.Hours = 0;
    sTime.Minutes = 0;
    sTime.Seconds = 0;
    if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
    {
        Error_Handler();
    }

    // ���ó�ʼ���ڣ�STM32F1ר�ø�ʽ��
    sDate.WeekDay = RTC_WEEKDAY_MONDAY;
    sDate.Month = RTC_MONTH_JANUARY;
    sDate.Date = 1;
    sDate.Year = 24;
    if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
    {
        Error_Handler();
    }
}

static void MX_USART3_UART_Init(void)
{
    // ������Ҫ����CubeMX���ɵ�USART3��ʼ������
    // ʾ����
    huart3.Instance = USART3;
    huart3.Init.BaudRate = 115200;
    huart3.Init.WordLength = UART_WORDLENGTH_8B;
    huart3.Init.StopBits = UART_STOPBITS_1;
    huart3.Init.Parity = UART_PARITY_NONE;
    huart3.Init.Mode = UART_MODE_TX_RX;
    huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart3) != HAL_OK)
    {
        Error_Handler();
    }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
