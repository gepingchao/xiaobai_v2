#ifndef __MY_USART_H__
#define __MY_USART_H__

#include "include.h"


extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim6;

extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;


#define USART1_RECV_BUFF_SIZE 300
#define USART1_SEND_BUFF_SIZE 256
#define USART2_RECV_BUFF_SIZE 300
#define USART2_SEND_BUFF_SIZE 256
#define USART3_RECV_BUFF_SIZE 300
#define USART3_SEND_BUFF_SIZE 256
#define USART4_RECV_BUFF_SIZE 300
#define USART4_SEND_BUFF_SIZE 256
#define USART5_RECV_BUFF_SIZE 300
#define USART5_SEND_BUFF_SIZE 256

#define USART1_RECV_DATA 1
#define USART2_RECV_DATA 2
#define USART3_RECV_DATA 3
#define USART4_RECV_DATA 4
#define USART5_RECV_DATA 5

typedef struct
{
	unsigned short data_len;
	unsigned short dma_cndtr;
	unsigned char is_dma_tx_sending;
	unsigned char rx_data[USART1_RECV_BUFF_SIZE];
	unsigned char tx_data[USART1_SEND_BUFF_SIZE];
	unsigned int reslut;
	unsigned int mask;
	unsigned char recv_ok;	
	unsigned short point;
}S_Usart1_Recv_Data,*P_S_Usart1_Recv_Data;


typedef struct
{
	unsigned short data_len;
	unsigned short dma_cndtr;
	unsigned char is_dma_tx_sending;
	unsigned char rx_data[USART2_RECV_BUFF_SIZE];
	unsigned char tx_data[USART2_SEND_BUFF_SIZE];
	
	unsigned char recv_ok;	
	unsigned short point;
}S_Usart2_Recv_Data,*P_S_Usart2_Recv_Data;


typedef struct
{
	unsigned short data_len;
	unsigned short dma_cndtr;
	unsigned char is_dma_tx_sending;
	unsigned char rx_data[USART3_RECV_BUFF_SIZE];
	unsigned char tx_data[USART3_SEND_BUFF_SIZE];
	
	unsigned char recv_ok;	
	unsigned short point;
}S_Usart3_Recv_Data,*P_S_Usart3_Recv_Data;


typedef struct
{
	unsigned short data_len;
	unsigned short dma_cndtr;
	unsigned char is_dma_tx_sending;
	unsigned char rx_data[USART4_RECV_BUFF_SIZE];
	unsigned char tx_data[USART4_SEND_BUFF_SIZE];
	
	unsigned char reslut;
	unsigned char mask;
	
	unsigned char recv_ok;	
	unsigned short point;
}S_Usart4_Recv_Data,*P_S_Usart4_Recv_Data;


typedef struct
{
	unsigned short data_len;
	unsigned short dma_cndtr;
	unsigned char is_dma_tx_sending;
	unsigned char rx_data[USART5_RECV_BUFF_SIZE];
	unsigned char tx_data[USART5_SEND_BUFF_SIZE];
	
	unsigned char recv_ok;	
	unsigned short point;
}S_Usart5_Recv_Data,*P_S_Usart5_Recv_Data;


extern S_Usart1_Recv_Data co2_sensor_recv_data;
extern S_Usart2_Recv_Data tcm300_recv_data;
extern S_Usart3_Recv_Data wifi_recv_data;
extern S_Usart4_Recv_Data hcho_sensor_recv_data;
extern S_Usart5_Recv_Data pm25_sensor_recv_data;

void start_recv_data(void);
void UsartReceive_IDLE(UART_HandleTypeDef *huart)  ;
void get_tcm300_id(void);
void pretreatment_uasrt1_data(void);
void get_hcho_value(void);
void pretreatment_uasrt4_data(void);

#endif

