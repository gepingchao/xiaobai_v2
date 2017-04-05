#include "my_usart.h"

S_Usart1_Recv_Data co2_sensor_recv_data;
S_Usart2_Recv_Data tcm300_recv_data;
S_Usart3_Recv_Data wifi_recv_data;
S_Usart4_Recv_Data hcho_sensor_recv_data;
S_Usart5_Recv_Data pm25_sensor_recv_data;

void UsartReceive_IDLE(UART_HandleTypeDef *huart)  
{  
	if(huart == &huart1)
	  	{
		    if((__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) != RESET))  
		    {   
		        __HAL_UART_CLEAR_IDLEFLAG(huart);  
			co2_sensor_recv_data.dma_cndtr = huart1.hdmarx->Instance->CNDTR; 
			HAL_TIM_Base_Start_IT(&htim2);
		    }  
	  	}
	if(huart == &huart2)
	  	{
		    if((__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) != RESET))  
		    {   
		        __HAL_UART_CLEAR_IDLEFLAG(huart);  
			tcm300_recv_data.dma_cndtr = huart1.hdmarx->Instance->CNDTR; 
			HAL_TIM_Base_Start_IT(&htim3);
		    }  
	  	}
	if(huart == &huart3)
	  	{
		    if((__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) != RESET))  
		    {   
		        __HAL_UART_CLEAR_IDLEFLAG(huart);  
			wifi_recv_data.dma_cndtr = huart1.hdmarx->Instance->CNDTR; 
			HAL_TIM_Base_Start_IT(&htim4);
		    }  
	  	}
	if(huart == &huart4)
	  	{
		    if((__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) != RESET))  
		    {   
		        __HAL_UART_CLEAR_IDLEFLAG(huart);  
			hcho_sensor_recv_data.dma_cndtr = huart1.hdmarx->Instance->CNDTR; 
			HAL_TIM_Base_Start_IT(&htim5);
		    }  
	  	}
	
}

void start_recv_data(void)
{
	HAL_UART_Receive_DMA(&huart1, co2_sensor_recv_data.rx_data, USART1_RECV_BUFF_SIZE);  
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);	

	HAL_UART_Receive_DMA(&huart2, tcm300_recv_data.rx_data, USART2_RECV_BUFF_SIZE);  
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);	

	HAL_UART_Receive_DMA(&huart3, wifi_recv_data.rx_data, USART3_RECV_BUFF_SIZE);  
	__HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);	

	HAL_UART_Receive_DMA(&huart4, hcho_sensor_recv_data.rx_data, USART4_RECV_BUFF_SIZE);  
	__HAL_UART_ENABLE_IT(&huart4, UART_IT_IDLE);	
}


void get_tcm300_id(void)
{
	unsigned char send_data[8];
	send_data[0]= 0X55;
	send_data[1]= 0X00;
	send_data[2]= 0X01;
	send_data[3]= 0X00;
	send_data[4]= 0X05;
	send_data[5]= 0X70;
	send_data[6]= 0X03;
	send_data[7]= 0X09;
	HAL_UART_Transmit(&huart2,send_data,8,800);
}


void pretreatment_uasrt1_data(void)
{
	unsigned loopx;
	unsigned int res = 0;
	loopx = co2_sensor_recv_data.data_len;
	while(loopx --)
		{
			if(co2_sensor_recv_data.rx_data[loopx] == 'Z')
				{
					break;
				}
		}
	res += ( co2_sensor_recv_data.rx_data[loopx +2]-'0')*10000;
	res += ( co2_sensor_recv_data.rx_data[loopx +3]-'0')*1000;
	res += ( co2_sensor_recv_data.rx_data[loopx +4]-'0')*100;
	res += ( co2_sensor_recv_data.rx_data[loopx +5]-'0')*10;
	res += ( co2_sensor_recv_data.rx_data[loopx +6]-'0');
	co2_sensor_recv_data.reslut = res - co2_sensor_recv_data.mask;
}

void get_hcho_value(void)
{
	unsigned char cmd[8];
	cmd[0] = 0X01;
	cmd[1] = 0X03;
	cmd[2] = 0X00;
	cmd[3] = 0X00;
	cmd[4] = 0X00;
	cmd[5] = 0X01;
	cmd[6] = 0X84;
	cmd[7] = 0X0A;
	HAL_UART_Transmit(&huart4,cmd,8,800);
}

