#include"iic.h"
unsigned char tem_buf[3];
unsigned char hum_buf[3];
unsigned char tvoc_value[9] = {0};
void delay_5us(void)
{
	unsigned short tmp ;
	for(tmp = 0;tmp<70;tmp++);
}

void tvoc_delay_pluse(void)
{
	unsigned short tmp ;
	for(tmp = 0;tmp<100;tmp++);
}

void delay_5ms(void)
{
	unsigned int tmp;
	for(tmp = 0;tmp<5000;tmp++);
}

void start_iic(void)
{
	sda1;
	scl1;
	delay_5us();
	sda0;
	delay_5us();
	scl0;
	delay_5us();	
	sda1;
}

void stop_iic(void)
{
	sda0;
	scl0;
	delay_5us();
	scl1;
	delay_5us();
	sda0;
	delay_5us();
	sda1;
}

void iic_ack(void)
{
	sda0;
	scl0;
	delay_5us();
	scl1;
	delay_5us();
	scl0;
	
}

void iic_nack(void)
{
	sda1;
	scl0;
	delay_5us();
	scl1;
	delay_5us();
	scl0;
	
}

unsigned char iic_wait_ack(void)
{
	scl0;
	delay_5us();
	sda1;
	delay_5us();
	scl1;
	delay_5us();
	while(READ_SDA == GPIO_PIN_SET)
		{
			scl0;
			return 0;
		}
	scl0;
	return 1;
}


unsigned char iic_write_byte(unsigned char data)
{
	unsigned char loop8;
	scl0;
	delay_5us();
	for(loop8 =0;loop8<8;loop8++)
		{
			if((data & 0X80) == 0X00)
				{sda0;}
			else
				{sda1;}
			delay_5us();
			scl1;
			delay_5us();
			scl0;
			delay_5us();
			data = (data<<1);
		}
	scl0;
		return 1;
}

unsigned char iic_read_byte(unsigned char ack)
{
	unsigned char loop8;
	GPIO_PinState stat;
	unsigned char read_data;
	(void)ack;
	sda1;
	for(loop8 = 0;loop8<8;loop8++)
		{
			scl0;
			delay_5us();
			scl1;
			delay_5us();
			read_data = (read_data<<1);
			stat = READ_SDA;
			if(stat == GPIO_PIN_SET)
				{read_data = read_data|0X01;}
		}
		scl0;
		delay_5us();
		return read_data;	
}

unsigned char SHT2x_CheckCRCSub(unsigned char vData[],unsigned char vDataLong, unsigned char vCheckSum)
{
	unsigned char  vCRC_Data=0X00;
        unsigned char  vLoop;
        unsigned char  vTemp;

        for(vLoop=0X00;vLoop<vDataLong;++vLoop)
        {
        	vCRC_Data^=vData[vLoop];

                for (vTemp=0X08;vTemp>0;--vTemp)
                {
                	if (vCRC_Data&0X80) 
                	{
                		vCRC_Data=(vCRC_Data<<1)^POLYNOMIAL;
                	}
                        else
                        {
                        	vCRC_Data=(vCRC_Data<<1);
                        }
                }
        }

        if(vCRC_Data!=vCheckSum)
        {       return 0X00;                     }
        else
        {       return 0XFF;                     }
}

unsigned char SHT_soft_reset(void)
{
	start_iic();
	iic_write_byte(SHT_ADDR_W);

	iic_wait_ack();
		/*{HAL_UART_Transmit(&huart1,su_buf,2,10);}
	else
		{HAL_UART_Transmit(&huart1,fail_buf,1,10);}*/

	iic_write_byte(cSOFT_RESET);

	iic_wait_ack();
	/*if{HAL_UART_Transmit(&huart1,su_buf,3,10);}
	else
	{HAL_UART_Transmit(&huart1,fail_buf,2,10);}*/

	stop_iic();
	return 1;
}

unsigned char set_SHT_mode(void)
{
	unsigned char reg_data= 0x55;
	//unsigned check_sum;
	unsigned char res = 0;
	start_iic();
	iic_write_byte(SHT_ADDR_W);
	iic_wait_ack();

	iic_write_byte(cUSER_REG_R);
	iic_wait_ack();

	start_iic();
	iic_write_byte(SHT_ADDR_R)	;
	iic_wait_ack();
	
	reg_data = iic_read_byte(ACK);
	iic_nack();
	
	//check_sum = iic_read_byte(NACK);
	//if(SHT2x_CheckCRCSub(&reg_data,0X01,check_sum))
		//{
		//	res = 1;
		//}	
	//stop_iic();
	
	//res = 0;
	reg_data =(reg_data&0X7E)|cSHT2x_RES_10_13BIT;

	start_iic();
	iic_write_byte(SHT_ADDR_W);
	iic_wait_ack();
	
	iic_write_byte(cUSER_REG_W);	
	iic_wait_ack();
	
	iic_write_byte(reg_data);
	iic_wait_ack();
	
	stop_iic();

	delay_5ms();
	reg_data= 0x55;

	/*start_iic();
	iic_write_byte(SHT_ADDR_W);
	iic_wait_ack();

	iic_write_byte(cUSER_REG_R);
	iic_wait_ack();

	start_iic();
	iic_write_byte(SHT_ADDR_R)	;
	iic_wait_ack();
	
	reg_data= iic_read_byte(ACK);
	iic_nack();*/
	
	return res;		
}

unsigned int get_tem(void)
{
	unsigned int data;
	//unsigned char tmp;
	//unsigned char checksum;
	start_iic();
	iic_write_byte(SHT_ADDR_W);
	iic_wait_ack();

	iic_write_byte(cTRIG_T_MEASUREMENT_POLL);
	iic_wait_ack();
/*
	start_iic();
	iic_write_byte(SHT_ADDR_R);
	iic_wait_ack();
	scl1;
	delay_5us();
	while(READ_SCL ==GPIO_PIN_RESET);
*/
	//HAL_Delay(80);
	osDelay(80);

	start_iic();
	iic_write_byte(SHT_ADDR_R);
	iic_wait_ack();
	
	tem_buf[0] = iic_read_byte(0);
	iic_ack();

	tem_buf[1] = iic_read_byte(0);
	iic_ack();

	tem_buf[2] = iic_read_byte(0);
	iic_nack();
	stop_iic();
	if(SHT2x_CheckCRCSub(tem_buf,2,tem_buf[2]))
		{			
			data = (unsigned int)tem_buf[0] <<8;	
			data += (unsigned int)tem_buf[1];
		}
	return data;
	
}

unsigned int get_hum(void)
{
	unsigned int data;
	//unsigned char tmp;
	//unsigned char checksum;
	start_iic();
	iic_write_byte(SHT_ADDR_W);
	iic_wait_ack();

	iic_write_byte(cTRIG_RH_MEASUREMENT_POLL);
	iic_wait_ack();

/*	start_iic();
	iic_write_byte(SHT_ADDR_R);
	iic_wait_ack();
	scl1;
	delay_5us();
	while(READ_SCL ==GPIO_PIN_RESET);*/

	//HAL_Delay(80);
	osDelay(80);//这个80ms是给它做转换的时间 不等不行
	
	start_iic();
	iic_write_byte(SHT_ADDR_R);
	iic_wait_ack();
	
	hum_buf[0] = iic_read_byte(0);;
	iic_ack();

	hum_buf[1] = iic_read_byte(0);
	iic_ack();

	hum_buf[2] = iic_read_byte(0);
	iic_nack();
	stop_iic();

	if(SHT2x_CheckCRCSub(hum_buf,2,hum_buf[2]))
	{			
		data = (unsigned int)hum_buf[0] <<8;	
		data += (unsigned int)hum_buf[1];
	}
	
	return data;
}

unsigned char start_measure(void)
{
	unsigned char res;
	start_iic();

	iic_write_byte(SHT_ADDR_W);
	delay_5us();
		if(iic_write_byte(cTRIG_T_MEASUREMENT_POLL))
			{
				res = 1;
			}
	
	return res;
}

unsigned int measure_tmp(void)
{
	unsigned char loop8;
	unsigned int temperature;
	unsigned char data[2] = {0,0};
	unsigned char checksum;
	unsigned res = 0;
//	float masure_temperature;
	
        for(loop8=0X00;loop8<0X08;loop8++)
        {
        	start_iic();
        	if(iic_write_byte(SHT_ADDR_R)==0X00)
        	{
        		res=0X00;
        		break;
        	}

                delay_5ms();
        }
		
	data[0] = iic_read_byte(ACK);
	data[1] = iic_read_byte(ACK);
	checksum = iic_read_byte(NACK);
	stop_iic();
	res = SHT2x_CheckCRCSub(data,2,checksum);
	if(res)
		{
			temperature = ((unsigned int)data[0]<<8) + (unsigned int)data[1];
			return temperature;
		}
	return 0;
}

///////////////////////////////////////////////////////////////////TVOCSENSOR

void start_tvoc_iic(void)
{
	TVOC_SDA_1;
	TVOC_SCL_1;
	tvoc_delay_pluse();
	TVOC_SDA_0;
	tvoc_delay_pluse();
	TVOC_SCL_0;
	tvoc_delay_pluse();	
	//TVOC_SDA_1;
}


void stop_tvoc_iic(void)
{
	TVOC_SDA_0;
	TVOC_SCL_0;
	tvoc_delay_pluse();
	tvoc_delay_pluse();
	TVOC_SCL_1;
	tvoc_delay_pluse();
	tvoc_delay_pluse();
	TVOC_SDA_0;
	tvoc_delay_pluse();
	TVOC_SDA_1;
}

void iic_tvoc_ack(void)
{
	TVOC_SDA_0;
	TVOC_SCL_0;
	tvoc_delay_pluse();
	TVOC_SCL_1;
	tvoc_delay_pluse();
	tvoc_delay_pluse();
	TVOC_SCL_0;
	
}

void iic_tvoc_nack(void)
{
	TVOC_SDA_1;
	TVOC_SCL_0;
	tvoc_delay_pluse();
	TVOC_SCL_1;
	tvoc_delay_pluse();
	tvoc_delay_pluse();
	TVOC_SCL_0;
	
}

unsigned char iic_tvoc_wait_ack(void)
{
	TVOC_SCL_0;
	tvoc_delay_pluse();
	TVOC_SDA_1;
	tvoc_delay_pluse();
	TVOC_SCL_1;
	tvoc_delay_pluse();
	tvoc_delay_pluse();
	while(READ_TVOC_SDA== GPIO_PIN_SET)
		{
			TVOC_SCL_0;
			return 0;
		}
	TVOC_SCL_0;
	return 1;
}


unsigned char iic_tvoc_write_byte(unsigned char data)
{
	unsigned char loop8;
	TVOC_SCL_0;
	tvoc_delay_pluse();
	for(loop8 =0;loop8<8;loop8++)
		{
			if((data & 0X80) == 0X00)
				{TVOC_SDA_0;}
			else
				{TVOC_SDA_1;}
			tvoc_delay_pluse();
			TVOC_SCL_1;
			tvoc_delay_pluse();
			tvoc_delay_pluse();
			TVOC_SCL_0;
			tvoc_delay_pluse();
			data = (data<<1);
		}
	TVOC_SCL_0;
		return 1;
}

unsigned char iic_tvoc_read_byte(unsigned char ack)
{
	unsigned char loop8;
	GPIO_PinState stat;
	unsigned char read_data;
	(void)ack;
	TVOC_SDA_1;
	for(loop8 = 0;loop8<8;loop8++)
		{
			TVOC_SCL_0;
			tvoc_delay_pluse();
			TVOC_SCL_1;
			tvoc_delay_pluse();
			read_data = (read_data<<1);
			stat = READ_TVOC_SDA;
			if(stat == GPIO_PIN_SET)
				{read_data = read_data|0X01;}
		}
		TVOC_SCL_0;
		tvoc_delay_pluse();
		return read_data;	
}

unsigned int get_tvoc_value(void)
{
	unsigned int tvoc_co2 = 0;
	unsigned int tvoc_voc = 0;
	start_tvoc_iic();
	tvoc_delay_pluse();
	iic_tvoc_write_byte(TVOC_ADDR_R);
	
	if( 0 == iic_tvoc_wait_ack())
		{
			return 0;
		}
	delay_5ms();
	
	tvoc_value[0] = iic_tvoc_read_byte(ACK);
	iic_tvoc_ack();
	
	tvoc_value[1] = iic_tvoc_read_byte(ACK);
	iic_tvoc_ack();
	
	tvoc_value[2] = iic_tvoc_read_byte(ACK);
	iic_tvoc_ack();
	
	tvoc_value[3] = iic_tvoc_read_byte(ACK);
	iic_tvoc_ack();
	
	tvoc_value[4] = iic_tvoc_read_byte(ACK);
	iic_tvoc_ack();
	
	tvoc_value[5] = iic_tvoc_read_byte(ACK);
	iic_tvoc_ack();
	
	tvoc_value[6] = iic_tvoc_read_byte(ACK);
	iic_tvoc_ack();
	
	tvoc_value[7] = iic_tvoc_read_byte(ACK);
	iic_tvoc_ack();
	
	tvoc_value[8] = iic_tvoc_read_byte(ACK);	
	iic_tvoc_nack();

	stop_tvoc_iic();
	
	tvoc_co2 = tvoc_value[0];
	tvoc_co2 <<= 8;
	tvoc_co2 += tvoc_value[1];
	
	tvoc_voc = tvoc_value[7];
	tvoc_voc <<= 8;
	tvoc_voc += tvoc_value[8];
	if( 0 == tvoc_value[2])
		{
			air_info.tvoc_co2 = tvoc_co2;
			air_info.tvoc_voc = tvoc_voc;
		}
	return tvoc_voc;
}

