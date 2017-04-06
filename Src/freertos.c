/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/* USER CODE BEGIN Includes */     
#include "include.h"

/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/
osThreadId sys_core_taskHandle;
osThreadId gui_taskHandle;
osThreadId serial_taskHandle;
osThreadId watcher_taskHandle;
osMessageQId serial_queueHandle;
osMessageQId gui_queueHandle;

/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void sys_core_function(void const * argument);
void gui_function(void const * argument);
void serial_function(void const * argument);
void watcher_function(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

/* Hook prototypes */
void vApplicationIdleHook(void);
void vApplicationTickHook(void);

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */

/* USER CODE BEGIN 3 */

/* USER CODE END 3 */

/* Init FreeRTOS */

void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of sys_core_task */
  osThreadDef(sys_core_task, sys_core_function, osPriorityNormal, 0, 500);
  sys_core_taskHandle = osThreadCreate(osThread(sys_core_task), NULL);

  /* definition and creation of gui_task */
  osThreadDef(gui_task, gui_function, osPriorityNormal, 0, 800);
  gui_taskHandle = osThreadCreate(osThread(gui_task), NULL);

  /* definition and creation of serial_task */
  osThreadDef(serial_task, serial_function, osPriorityHigh, 0, 800);
  serial_taskHandle = osThreadCreate(osThread(serial_task), NULL);

  /* definition and creation of watcher_task */
  osThreadDef(watcher_task, watcher_function, osPriorityNormal, 0, 400);
  watcher_taskHandle = osThreadCreate(osThread(watcher_task), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Create the queue(s) */
  /* definition and creation of serial_queue */
  osMessageQDef(serial_queue, 8, uint8_t);
  serial_queueHandle = osMessageCreate(osMessageQ(serial_queue), NULL);

  /* definition and creation of gui_queue */
  osMessageQDef(gui_queue, 8, uint32_t);
  gui_queueHandle = osMessageCreate(osMessageQ(gui_queue), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
}

/* sys_core_function function */
void sys_core_function(void const * argument)
{

  /* USER CODE BEGIN sys_core_function */
  osDelay(30);
  save_task_info();
  start_recv_data();
  osDelay(3000);
  get_tcm300_id();

  /* Infinite loop */
  for(;;)
  {
	osDelay(10);
	while(HAL_UART_Receive_IT(&huart5,pm25_sensor_recv_data.rx_data,1)==HAL_OK);
  }
  /* USER CODE END sys_core_function */
}

/* gui_function function */
void gui_function(void const * argument)
{
  /* USER CODE BEGIN gui_function */
  save_task_info();
  unsigned short num;
  test_function();
  /* Infinite loop */
  for(;;)
  {

  
    osDelay(500);
	temp_data.save_num++;
	temp_data.point++;
	temp_data.save_num = (temp_data.save_num > 127)? 127 : temp_data.save_num;
	temp_data.f_change = common_change_function;

	
	push_data(&co2_data,(float)(co2_sensor_recv_data.reslut));
	
	load_data_to_graph_buf(&co2_data);

	//draw_image(kx++,ky+=2,8,16,(unsigned char*)gImage_8_16[6],kx%2);
	//draw_image(p_x++,p_y-=2,8,16,(unsigned char*)gImage_8_16[6],p_x%2);


	//EPD_Dis_Part(0,xDot-1,0,yDot-1,(unsigned char *)g_image_buf,1);
	
	//graph_buf.save_num = (graph_buf.save_num>127)? 127: graph_buf.save_num;
		
	display_graph();	

	
	if(num > MAX_SAVE_DATA_NUM)
		{
			num = 0;
		}


  }
  /* USER CODE END gui_function */
}

/* serial_function function */
void serial_function(void const * argument)
{
  /* USER CODE BEGIN serial_function */
  save_task_info();
  /* Infinite loop */
  unsigned char serial_value;
  for(;;)
  {
  	xQueueReceive(serial_queueHandle, &serial_value, portMAX_DELAY);
	switch(serial_value)
		{
			case USART1_RECV_DATA:
				pretreatment_uasrt1_data();
				break;
			case USART2_RECV_DATA:
				break;
			case USART3_RECV_DATA:
				break;
			case USART4_RECV_DATA:
				pretreatment_uasrt4_data();
				break;
			case USART5_RECV_DATA:
				break;
			default :
				break;
		}
  }
  /* USER CODE END serial_function */
}

/* watcher_function function */
void watcher_function(void const * argument)
{
  /* USER CODE BEGIN watcher_function */
  save_task_info();
  /* Infinite loop */
  for(;;)
  {
	osDelay(15000);
	get_hcho_value();
	air_info.tvoc_voc = get_tvoc_value();
	
  }
  /* USER CODE END watcher_function */
}

/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
