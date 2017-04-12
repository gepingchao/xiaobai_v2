/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "crc.h"
#include "dma.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "include.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
extern osMessageQId serial_queueHandle;
extern osMessageQId gui_queueHandle;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);
void MX_FREERTOS_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_RTC_Init();
  MX_CRC_Init();
  MX_UART4_Init();
  MX_UART5_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_TIM6_Init();
  MX_TIM2_Init();

  /* USER CODE BEGIN 2 */
  POWER_ON;
  RELEASE_WIFI_RESET;
  RELEASE_SMART_LINK;
  //SET_SMART_LINK;
  co2_sensor_recv_data.mask = 900;
  HAL_Delay(300); 
  HAL_UART_Transmit(&huart1,"K 2",3,800);
  HAL_Delay(300);
  HAL_UART_Transmit(&huart1,"M 12358",7,800);
  HAL_Delay(300);
  HAL_UART_Transmit(&huart1,"X 450",5,800);
  HAL_Delay(300);
  while(HAL_UART_Receive_IT(&huart5,pm25_sensor_recv_data.rx_data,1)==HAL_OK);
  /* USER CODE END 2 */

  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();

  /* Start scheduler */
  osKernelStart();
  
  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

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

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM7 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
/* USER CODE BEGIN Callback 0 */
//S_Usart1_Recv_Data co2_sensor_recv_data;
//S_Usart2_Recv_Data tcm300_recv_data;
//S_Usart3_Recv_Data wifi_recv_data;
//S_Usart4_Recv_Data hcho_sensor_recv_data;
//S_Usart5_Recv_Data pm25_sensor_recv_data;

	unsigned char uasrt_msg = 0;
	BaseType_t xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;	
	if (htim->Instance == TIM2) {
  	HAL_TIM_Base_Stop_IT(&htim2);
	if(co2_sensor_recv_data.dma_cndtr ==  huart1.hdmarx->Instance->CNDTR)
		{			
		        HAL_UART_DMAStop(&huart1);   
			co2_sensor_recv_data.data_len = USART1_RECV_BUFF_SIZE-co2_sensor_recv_data.dma_cndtr;
			HAL_UART_Receive_DMA(&huart1, co2_sensor_recv_data.rx_data, USART1_RECV_BUFF_SIZE);  
			uasrt_msg = USART1_RECV_DATA;
			co2_sensor_recv_data.recv_ok = 1;
			xQueueSendFromISR(serial_queueHandle,&uasrt_msg,&xHigherPriorityTaskWoken);
		}else{
			HAL_TIM_Base_Start_IT(&htim2);
			co2_sensor_recv_data.dma_cndtr =  huart1.hdmarx->Instance->CNDTR;
		}
  	}
	
	if (htim->Instance == TIM3) {
  	HAL_TIM_Base_Stop_IT(&htim3);
	if(tcm300_recv_data.dma_cndtr ==  huart2.hdmarx->Instance->CNDTR)
		{			
		        HAL_UART_DMAStop(&huart2);   
			tcm300_recv_data.data_len = USART2_RECV_BUFF_SIZE-tcm300_recv_data.dma_cndtr;
			HAL_UART_Receive_DMA(&huart2, tcm300_recv_data.rx_data, USART2_RECV_BUFF_SIZE);  
			uasrt_msg = USART2_RECV_DATA;
			tcm300_recv_data.recv_ok = 1;
			xQueueSendFromISR(serial_queueHandle,&uasrt_msg,&xHigherPriorityTaskWoken);
		}else{
			HAL_TIM_Base_Start_IT(&htim3);
			tcm300_recv_data.dma_cndtr =  huart2.hdmarx->Instance->CNDTR;
		}
  	}
	
	if (htim->Instance == TIM4) {
  	HAL_TIM_Base_Stop_IT(&htim4);
	if(wifi_recv_data.dma_cndtr ==  huart3.hdmarx->Instance->CNDTR)
		{			
		        HAL_UART_DMAStop(&huart3);   
			wifi_recv_data.data_len = USART3_RECV_BUFF_SIZE-wifi_recv_data.dma_cndtr;
			HAL_UART_Receive_DMA(&huart3, wifi_recv_data.rx_data, USART3_RECV_BUFF_SIZE);  
			uasrt_msg = USART3_RECV_DATA;
			wifi_recv_data.recv_ok = 1;
			xQueueSendFromISR(serial_queueHandle,&uasrt_msg,&xHigherPriorityTaskWoken);
		}else{
			HAL_TIM_Base_Start_IT(&htim4);
			wifi_recv_data.dma_cndtr =  huart3.hdmarx->Instance->CNDTR;
		}
  	}
	
	if (htim->Instance == TIM5) {
  	HAL_TIM_Base_Stop_IT(&htim5);
	if(hcho_sensor_recv_data.dma_cndtr ==  huart4.hdmarx->Instance->CNDTR)
		{			
		        HAL_UART_DMAStop(&huart4);   
			hcho_sensor_recv_data.data_len = USART4_RECV_BUFF_SIZE-hcho_sensor_recv_data.dma_cndtr;
			HAL_UART_Receive_DMA(&huart4, hcho_sensor_recv_data.rx_data, USART4_RECV_BUFF_SIZE);  
			uasrt_msg = USART4_RECV_DATA;
			hcho_sensor_recv_data.recv_ok = 1;
			xQueueSendFromISR(serial_queueHandle,&uasrt_msg,&xHigherPriorityTaskWoken);
		}else{
			HAL_TIM_Base_Start_IT(&htim5);
			hcho_sensor_recv_data.dma_cndtr =  huart4.hdmarx->Instance->CNDTR;
		}
  	}
/* USER CODE END Callback 0 */
  if (htim->Instance == TIM7) {
    HAL_IncTick();
  }
/* USER CODE BEGIN Callback 1 */

/* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
