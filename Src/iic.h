#ifndef __iic_h__
#define __iic_h__
#include "include.h"

#define POLYNOMIAL  0X131

#define SHT_ADDR_W 128
#define SHT_ADDR_R 129

#define TVOC_ADDR_W	0XB4
#define TVOC_ADDR_R 		0XB5

#define ACK 0
#define NACK 1

#define scl1  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_SET)
#define scl0  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_RESET)

#define TVOC_SCL_1	HAL_GPIO_WritePin(TVOC_SCLK_GPIO_Port,TVOC_SCLK_Pin,GPIO_PIN_SET)
#define TVOC_SCL_0	HAL_GPIO_WritePin(TVOC_SCLK_GPIO_Port,TVOC_SCLK_Pin,GPIO_PIN_RESET)

#define sda1  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET)
#define sda0  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET)

#define TVOC_SDA_1	HAL_GPIO_WritePin(TVOC_SDA_GPIO_Port,TVOC_SDA_Pin,GPIO_PIN_SET)
#define TVOC_SDA_0	HAL_GPIO_WritePin(TVOC_SDA_GPIO_Port,TVOC_SDA_Pin,GPIO_PIN_RESET)


#define READ_SDA HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_7)
#define READ_SCL HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_6)

#define READ_TVOC_SDA 	HAL_GPIO_ReadPin(TVOC_SDA_GPIO_Port,TVOC_SDA_Pin)
#define READ_TVOC_SCL	HAL_GPIO_ReadPin(TVOC_SCLK_GPIO_Port,TVOC_SCLK_Pin)

#define     cPOLYNOMIAL  0X131

typedef enum{
             cTRIG_T_MEASUREMENT_HM=0XE3,       // command trig. Temperature meas. hold master
             cTRIG_RH_MEASUREMENT_HM=0XE5,      // command trig. humidity meas. hold master
             cTRIG_T_MEASUREMENT_POLL=0XF3,     // command trig. Temperature meas. no hold master
             cTRIG_RH_MEASUREMENT_POLL=0XF5,    // command trig. humidity meas. no hold master
             cUSER_REG_W=0XE6,                  // command writing user register
             cUSER_REG_R=0XE7,                  // command reading user register
             cSOFT_RESET=0XFE                   // command soft reset
            }cEtSHT2xCommand;

typedef enum{
             cSHT2x_RES_12_14BIT=0X00,          // RH=12bit, T=14bit
             cSHT2x_RES_8_12BIT=0X01,           // RH= 8bit, T=12bit
             cSHT2x_RES_10_13BIT=0X80,          // RH=10bit, T=13bit
             cSHT2x_RES_11_11BIT=0X81,          // RH=11bit, T=11bit
             cSHT2x_RES_MASK=0X81               // Mask for res. bits (7,0) in user reg.
            }cEtSHT2xResolution;

typedef enum{
             cSHT2x_EOB_ON=0X40,                // end of battery
             cSHT2x_EOB_MASK=0X40               // Mask for EOB bit(6) in user reg.
            }cEtSHT2xEob;

typedef enum{
             cSHT2x_HEATER_ON=0X04,             // heater on
             cSHT2x_HEATER_OFF=0X00,            // heater off
             cSHT2x_HEATER_MASK=0X04            // Mask for Heater bit(2) in user reg.
            }cEtSHT2xHeater;

// measurement signal selection
typedef enum{
             cHUMIDITY,
             cTEMP
            }cEtSHT2xMeasureType;

extern unsigned char tem_buf[3];
extern unsigned char hum_buf[3];

typedef struct
{
	unsigned char point;
	unsigned tem_buf[10];
	unsigned hum_buf[10];
}tem_hum_buf_def;
extern tem_hum_buf_def tem_hum_buf1;

void delay_5us(void);
void delay_5ms(void);
void start_iic(void);
void stop_iic(void);
unsigned char iic_write_byte(unsigned char data);
unsigned char iic_read_byte(unsigned char ack);
unsigned char SHT2x_CheckCRCSub(unsigned char vData[],unsigned char vDataLong, unsigned char vCheckSum);
unsigned char SHT_soft_reset(void);
unsigned char set_SHT_mode(void);
unsigned char start_measure(void);
unsigned int measure_tmp(void);
unsigned int get_tem(void);
unsigned int get_hum(void);
void init_sht20(void);

void save_tem_hum_buf(unsigned int tem,unsigned int hum);

//////////////////////////////////////////tvocsensor

unsigned int get_tvoc_value(void);



#endif
