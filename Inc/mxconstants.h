/**
  ******************************************************************************
  * File Name          : mxconstants.h
  * Description        : This file contains the common defines of the application
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MXCONSTANT_H
#define __MXCONSTANT_H
  /* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define FAN_Pin GPIO_PIN_8
#define FAN_GPIO_Port GPIOE
#define SPK_Pin GPIO_PIN_9
#define SPK_GPIO_Port GPIOE
#define PM_25_SCLK_Pin GPIO_PIN_10
#define PM_25_SCLK_GPIO_Port GPIOB
#define PM_25_SDA_Pin GPIO_PIN_11
#define PM_25_SDA_GPIO_Port GPIOB
#define WIFI_RESET_Pin GPIO_PIN_12
#define WIFI_RESET_GPIO_Port GPIOB
#define WIFI_NREADY_Pin GPIO_PIN_13
#define WIFI_NREADY_GPIO_Port GPIOB
#define WIFI_SMART_LINK_Pin GPIO_PIN_14
#define WIFI_SMART_LINK_GPIO_Port GPIOB
#define WIFI_N_LINK_Pin GPIO_PIN_15
#define WIFI_N_LINK_GPIO_Port GPIOB
#define POWER_Pin GPIO_PIN_7
#define POWER_GPIO_Port GPIOC
#define CO2_AMBIENT_Pin GPIO_PIN_11
#define CO2_AMBIENT_GPIO_Port GPIOA
#define CO2_ZERO_Pin GPIO_PIN_12
#define CO2_ZERO_GPIO_Port GPIOA
#define TVOC_SCLK_Pin GPIO_PIN_0
#define TVOC_SCLK_GPIO_Port GPIOD
#define TVOC_SDA_Pin GPIO_PIN_1
#define TVOC_SDA_GPIO_Port GPIOD
#define PAPER_MOSI_Pin GPIO_PIN_3
#define PAPER_MOSI_GPIO_Port GPIOD
#define PAPER_CLK_Pin GPIO_PIN_4
#define PAPER_CLK_GPIO_Port GPIOD
#define PAPER_CS_Pin GPIO_PIN_5
#define PAPER_CS_GPIO_Port GPIOD
#define PAPER_DC_Pin GPIO_PIN_6
#define PAPER_DC_GPIO_Port GPIOD
#define PAPER_RST_Pin GPIO_PIN_7
#define PAPER_RST_GPIO_Port GPIOD
#define PAPER_BUSY_Pin GPIO_PIN_3
#define PAPER_BUSY_GPIO_Port GPIOB
#define PAPER_BS_Pin GPIO_PIN_4
#define PAPER_BS_GPIO_Port GPIOB
#define TH_SCLK_Pin GPIO_PIN_6
#define TH_SCLK_GPIO_Port GPIOB
#define TH_SDA_Pin GPIO_PIN_7
#define TH_SDA_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MXCONSTANT_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
