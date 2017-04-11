#ifndef __MY_BSP_H__
#define __MY_BSP_H__

#include "gpio.h"
#include "my_port.h"


typedef struct
{
	unsigned int co2;
	unsigned int tvoc_co2;
	unsigned int tvoc_voc;
	unsigned int hcho;
	unsigned int pm25;
	unsigned int pm10;
	unsigned int temp;
	unsigned int hum;
}S_Air_Info,*P_S_Air_Info;
extern S_Air_Info air_info;


typedef struct
{
	unsigned char tcm300id[4];
	unsigned char is_wifi_not_linked;
	unsigned char is_wifi_not_ready;
}S_Machine_Info,*P_S_Machine_Info;
extern S_Machine_Info machine_info;

#define POWER_ON	HAL_GPIO_WritePin(POWER_GPIO_Port,POWER_Pin,GPIO_PIN_SET)
#define POWER_OFF	HAL_GPIO_WritePin(POWER_GPIO_Port,POWER_Pin,GPIO_PIN_RESET)

#define EPD_W21_MOSI_0	HAL_GPIO_WritePin(PAPER_MOSI_GPIO_Port,PAPER_MOSI_Pin,GPIO_PIN_RESET)
#define EPD_W21_MOSI_1	HAL_GPIO_WritePin(PAPER_MOSI_GPIO_Port,PAPER_MOSI_Pin,GPIO_PIN_SET)

#define EPD_W21_CLK_0	HAL_GPIO_WritePin(PAPER_CLK_GPIO_Port,PAPER_CLK_Pin,GPIO_PIN_RESET)
#define EPD_W21_CLK_1	HAL_GPIO_WritePin(PAPER_CLK_GPIO_Port,PAPER_CLK_Pin,GPIO_PIN_SET)

#define EPD_W21_CS_0	HAL_GPIO_WritePin(PAPER_CS_GPIO_Port,PAPER_CS_Pin,GPIO_PIN_RESET)
#define EPD_W21_CS_1	HAL_GPIO_WritePin(PAPER_CS_GPIO_Port,PAPER_CS_Pin,GPIO_PIN_SET)

#define EPD_W21_DC_0	HAL_GPIO_WritePin(PAPER_DC_GPIO_Port,PAPER_DC_Pin,GPIO_PIN_RESET)
#define EPD_W21_DC_1	HAL_GPIO_WritePin(PAPER_DC_GPIO_Port,PAPER_DC_Pin,GPIO_PIN_SET)

#define EPD_W21_RST_0	HAL_GPIO_WritePin(PAPER_RST_GPIO_Port,PAPER_RST_Pin,GPIO_PIN_RESET)
#define EPD_W21_RST_1	HAL_GPIO_WritePin(PAPER_RST_GPIO_Port,PAPER_RST_Pin,GPIO_PIN_SET)

#define EPD_W21_BS_0	HAL_GPIO_WritePin(PAPER_BS_GPIO_Port,PAPER_BS_Pin,GPIO_PIN_RESET)

#define EPD_W21_BUSY_LEVEL 0






#endif

