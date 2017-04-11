#include "pm25.h"

PM_25_DATA pm_25_data_1;
S_Res_Pm25 res_pm2_5;

static void pm25_delay_us(void)
{
	unsigned char tmp ;
	for(tmp = 0;tmp<200;tmp++); 
}

static void pm25_delay_ms(void)
{
	unsigned int tmp;
	for(tmp = 0;tmp<5000;tmp++);
}

void start_iic_1(void)
{
	SDA_1_1;
	SCL_1_1;
	pm25_delay_us();
	SDA_1_0;
	pm25_delay_us();
	SCL_1_0;
	pm25_delay_us();	
	//SDA_2_1;
}

void stop_iic_1(void)
{
	SDA_1_0;
	SCL_1_0;
	pm25_delay_us();
	SCL_1_1;
	pm25_delay_us();
	SDA_1_0;
	pm25_delay_us();
	SDA_1_1;
}

void iic_ack_1(void)
{
	SDA_1_0;
	SCL_1_0;
	pm25_delay_us();
	SCL_1_1;
	pm25_delay_us();
	SCL_1_0;
	
}

void iic_nack_1(void)
{
	SDA_1_1;
	SCL_1_0;
	pm25_delay_us();
	SCL_1_1;
	pm25_delay_us();
	SCL_1_0;
	
}

unsigned char iic_wait_ack_1(void)
{
	SCL_1_0;
	pm25_delay_us();
	SDA_1_1;
	pm25_delay_us();
	SCL_1_1;
	pm25_delay_us();
	while(READ_SDA_1== GPIO_PIN_SET)
		{
			SCL_1_0;
			return 0;
		}
	SCL_1_0;
	return 1;
}

unsigned char iic_1_write_byte(unsigned char data)
{
	unsigned char loop8;
	SCL_1_0;
	pm25_delay_us();
	for(loop8 =0;loop8<8;loop8++)
		{
			if((data & 0X80) == 0X00)
				{SDA_1_0;}
			else
				{SDA_1_1;}
			pm25_delay_us();
			SCL_1_1;
			pm25_delay_us();
			SCL_1_0;
			pm25_delay_us();
			data = (data<<1);
		}
	SCL_1_0;
		return 1;
}

unsigned char iic_1_read_byte(unsigned char ack)
{
	unsigned char loop8;
	GPIO_PinState stat;
	unsigned char read_data;
	(void)ack;
	SDA_1_1;
	for(loop8 = 0;loop8<8;loop8++)
		{
			SCL_1_0;
			pm25_delay_us();
			SCL_1_1;
			pm25_delay_us();
			read_data = (read_data<<1);
			stat = READ_SDA_1;
			if(stat == GPIO_PIN_SET)
				{read_data = read_data|0X01;}
		}
		SCL_1_0;
		pm25_delay_us();
		return read_data;	
}


void get_pm_25_1(void)
{
	unsigned int tmp_data_0;
	start_iic_1();
	iic_1_write_byte(PM_25_ADDR_W);
	iic_wait_ack_1();
	iic_1_write_byte(0X00);
	iic_wait_ack_1();
	//stop_iic_2();
	//delay_5ms();
	start_iic_1();
	iic_1_write_byte(PM_25_ADDR_R);
	iic_wait_ack_1();
	pm_25_data_1.data0[0]= iic_1_read_byte(1);
	iic_ack_1();
	pm_25_data_1.data0[1]= iic_1_read_byte(1);
	iic_ack_1();
	pm_25_data_1.data0[2]= iic_1_read_byte(0);
	iic_ack_1();
	pm_25_data_1.data0[3] = iic_1_read_byte(0);
	iic_nack_1();
	stop_iic_1();
	tmp_data_0 =(int)((int)pm_25_data_1.data0[0] |((int)pm_25_data_1.data0[1] <<8)|((int)pm_25_data_1.data0[2]<<16) |((int)pm_25_data_1.data0[3] <<24) );
	pm_25_data_1.f_data0 = ((float)(tmp_data_0))/1000;
}

void get_pm_25_data8_1(void)
{	
	unsigned int tmp_data_8;
	start_iic_1();
	iic_1_write_byte(PM_25_ADDR_W);
	iic_wait_ack_1();
	iic_1_write_byte(0X08);
	iic_wait_ack_1();
	//stop_iic_2();
	//delay_5ms();
	start_iic_1();
	iic_1_write_byte(PM_25_ADDR_R);
	iic_wait_ack_1();
	pm_25_data_1.data8[0]= iic_1_read_byte(1);
	iic_ack_1();
	pm_25_data_1.data8[1]= iic_1_read_byte(1);
	iic_nack_1();
	stop_iic_1();
	tmp_data_8 =(int)((int)pm_25_data_1.data8[0] |((int)pm_25_data_1.data8[1] <<8));
	pm_25_data_1.i_data8= tmp_data_8;
}
void get_pm_25_data20_1(void)
{
	unsigned int tmp_data_20;
	unsigned char loopx;
	start_iic_1();
	iic_1_write_byte(PM_25_ADDR_W);
	iic_wait_ack_1();
	iic_1_write_byte(0X20);
	iic_wait_ack_1();
	//stop_iic_2();
	//delay_5ms();
	start_iic_1();
	iic_1_write_byte(PM_25_ADDR_R);
	iic_wait_ack_1();
	pm_25_data_1.data20[0]= iic_1_read_byte(1);
	iic_ack_1();
	pm_25_data_1.data20[1]= iic_1_read_byte(1);
	iic_ack_1();
	pm_25_data_1.data20[2]= iic_1_read_byte(0);
	iic_ack_1();
	pm_25_data_1.data20[3] = iic_1_read_byte(0);
	iic_nack_1();
	stop_iic_1();
	tmp_data_20 =(int)((int)pm_25_data_1.data20[0] |((int)pm_25_data_1.data20[1] <<8)|((int)pm_25_data_1.data20[2]<<16) |((int)pm_25_data_1.data20[3] <<24) );
	pm_25_data_1.f_data20 = ((float)(tmp_data_20))/1000;
	
	res_pm2_5.buf1[res_pm2_5.point1++] = pm_25_data_1.f_data20;
	res_pm2_5.res_data1_tmp = 0;
	for(loopx = 0; loopx < 4; loopx ++)
		{
			res_pm2_5.res_data1_tmp += res_pm2_5.buf1[loopx];
		}
	res_pm2_5.avg_data1 = res_pm2_5.res_data1_tmp/4;
	
}

