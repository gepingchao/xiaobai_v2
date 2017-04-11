#ifndef __PM25_H__
#define __PM25_H__
#include "include.h"

#define PM_25_ADDR_W 0X66
#define PM_25_ADDR_R 0X67

#define ACK 0
#define NACK 1

typedef struct
{
unsigned char data0[4];
unsigned char data8[2];
unsigned char data20[4];
int i_data8;
float f_data0;
float f_data20;
}PM_25_DATA;
extern PM_25_DATA pm_25_data_1;

typedef struct
{
	unsigned char point1:2;
	unsigned char point2:2;
	unsigned char point3:2;
	float buf1[4];
	float buf2[4];
	float buf3[4];
	float res_data1_tmp;
	float res_data2_tmp;
	float res_data3_tmp;
	float avg_data1;
	float avg_data2;
	float avg_data3;
	
}S_Res_Pm25,*P_S_Res_Pm25;
extern S_Res_Pm25 res_pm2_5;


#define SCL_1_1  HAL_GPIO_WritePin(PM_25_SCLK_GPIO_Port,PM_25_SCLK_Pin,GPIO_PIN_SET)
#define SCL_1_0  HAL_GPIO_WritePin(PM_25_SCLK_GPIO_Port,PM_25_SCLK_Pin,GPIO_PIN_RESET)

#define SDA_1_1  HAL_GPIO_WritePin(PM_25_SDA_GPIO_Port,PM_25_SDA_Pin,GPIO_PIN_SET)
#define SDA_1_0  HAL_GPIO_WritePin(PM_25_SDA_GPIO_Port,PM_25_SDA_Pin,GPIO_PIN_RESET)

#define READ_SDA_1 HAL_GPIO_ReadPin(PM_25_SDA_GPIO_Port,PM_25_SDA_Pin)
#define READ_SCL_1 HAL_GPIO_ReadPin(PM_25_SCLK_GPIO_Port,PM_25_SCLK_Pin)


#endif

