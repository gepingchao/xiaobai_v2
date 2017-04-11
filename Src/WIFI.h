#ifndef __WIFI_H__
#define __WIFI_H__
#include "Include.h"
#include "cmsis_os.h"

extern UART_HandleTypeDef huart3;
#define WIFI_COM		huart3

typedef struct
{
	unsigned char type;
	unsigned char flag;
	unsigned char data[300];
	unsigned char point;
	unsigned char data_len;
	unsigned char recv_data_num;

	unsigned char tx_data[200];
	
	unsigned char head_buf[5];
	unsigned char head_point;
	unsigned char head_ok;
	
	unsigned char recv_ok;
	
	unsigned char enable_rxdata;
	unsigned char vBigRxMax;
	unsigned char at_ctrl_model;
}recv_data_def;
extern recv_data_def data_from_wifi;


#define OK 1
#define FULL  2
#define FAIL 3
#define  	A_T_FIELD_START	 0X38
#define 	A_T_FIELD_STOP	 0X00

#define WARNING		0XEE

#define READ_NREADY HAL_GPIO_ReadPin(WIFI_NREADY_GPIO_Port,WIFI_NREADY_Pin)
#define READ_NLINK HAL_GPIO_ReadPin(WIFI_N_LINK_GPIO_Port,WIFI_N_LINK_Pin)

#define PRESS_SMART_LINK HAL_GPIO_WritePin(WIFI_SMART_LINK_GPIO_Port,WIFI_SMART_LINK_Pin,GPIO_PIN_RESET)
#define RELEASE_SMART_LINK HAL_GPIO_WritePin(WIFI_SMART_LINK_GPIO_Port,WIFI_SMART_LINK_Pin,GPIO_PIN_SET)
#define SET_SMART_LINK {PRESS_SMART_LINK;HAL_Delay(1000);RELEASE_SMART_LINK;}
#define RESET_SMART_LINK {PRESS_SMART_LINK;HAL_Delay(5000);RELEASE_SMART_LINK;}

#define PRESS_WIFI_RESET HAL_GPIO_WritePin(WIFI_RESET_GPIO_Port,WIFI_RESET_Pin,GPIO_PIN_RESET)
#define RELEASE_WIFI_RESET HAL_GPIO_WritePin(WIFI_RESET_GPIO_Port,WIFI_RESET_Pin,GPIO_PIN_SET)
#define RESET_WIFI {PRESS_WIFI_RESET;HAL_Delay(20);RELEASE_WIFI_RESET;}

void mem_copy(char* from,char* to,unsigned char cnt);
unsigned char string_length(char* string);

void A_T_Field(void);
void upload_warn_id_by_wifi(unsigned char warn_id);
void upload_data_by_wifi(unsigned char comand,unsigned char *data,unsigned char length);
void wifi_expand_command(void);
void A_T_Command(char* command);
void Quit_A_T_Field(void);
unsigned char A_T_Set_SockA_Server(unsigned char mode,unsigned char ip1,unsigned char ip2,unsigned char ip3,unsigned char ip4,unsigned int port);
unsigned char A_T_Set_SockB_Server(unsigned char mode,unsigned char ip1,unsigned char ip2,unsigned char ip3,unsigned char ip4,unsigned int port);
void wifi_set_server(void);


#endif

