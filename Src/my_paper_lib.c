#include "my_paper_lib.h"
#include "include.h"

unsigned char g_image_buf[4736];

unsigned char g_re_data[10];

S_Graph_Buf graph_buf;

unsigned char g_re_data2[10];

S_Save_Data temp_data = {.type = data_of_temp};
S_Save_Data voc_data= {.type = data_of_tvoc};
S_Save_Data pm25_data= {.type = data_of_pm25};
S_Save_Data hum_data= {.type = data_of_hum};
S_Save_Data co2_data= {.type = data_of_co2};
S_Save_Data pm10_data= {.type = data_of_pm10};
S_Save_Data hcho= {.type = data_of_hcho};
//S_Save_Data 

char common_change_function(float max,float min,float current)
{
	if(current == max)
		{
			return 95;
		}
	if(max == min)
		{
			return 47;
		}
	char tmp;
	tmp = (char)((95*(current - min))/(max-min))+3;
	return tmp;
}

char pm2_5_change_function(float max,float min,float current)
{
  return 1;
}

char voc_change_function(float max,float min,float current)
{
	switch((short)current)
		{
			case 0:
				return 1;

			case 1:
				return 25;

			case 2:
				return 50;

			case 3:
				return 75;

			default:
				return 25;
		}
}
char pm10_change_function(float max,float min,float current)
{
	return 1;
}
char hum_change_function(float max,float min,float current)
{
	return 1;
}
float min_in_n_buf(float* buf,unsigned short data_num)
{
	float tmp = 10000.0;
	float* p_tmp = buf;
	unsigned short loopx = 0;
	if(data_num == 0)
		{
			tmp = 0.0;
		}
	for(loopx = 0 ;loopx < data_num ; loopx++)
		{
			if(tmp > *(p_tmp))
				{
					tmp = *p_tmp;
				}
			p_tmp++;
		}
	return tmp;
}

float max_in_n_buf(float* buf,unsigned short data_num)
{
	float tmp = 0.0;
	float* p_tmp = buf;
	unsigned short loopx = 0;
	for(loopx = 0 ;loopx < data_num ; loopx++)
		{
			if(tmp < *p_tmp)
				{
					tmp =  *p_tmp;
				}
			p_tmp ++;
		}
	return tmp;
}

unsigned char display_num(unsigned char line_x,unsigned short pixel_y,float num,unsigned char font)
{
	unsigned char* font_num;
	
	if(2 == font)
		{
			font_num = (unsigned char*)gImage_16_10;
		}
	else if(1 == font)
		{
			font_num = (unsigned char*)gImage_8_6;
		}
	else
		{
			return 0XFF;
		}
	unsigned char qian_wei,bai_wei,shi_wei,ge_wei,shifen_wei;
	unsigned char offset = 0;
	short int_num = 0;
	float float_num = 0.0;
	unsigned char is_show_dot = 0;
	
	int_num = (short)num;
	float_num = num - int_num;
	shifen_wei = 10 * float_num;
	
	unsigned char num_num = 0;
	qian_wei = 0;
	bai_wei = 0;
	shi_wei = 0;
	ge_wei = 0;
	if(int_num > 0)
		{
			ge_wei = int_num % 10;
			num_num = 1;
		}

	if(int_num > 9)
		{
			shi_wei = (int_num / 10)%10;
			num_num = 2;
		}

	if(int_num > 99)
		{
			bai_wei = (int_num /100)%10;
			num_num = 3;
		}
	if(int_num > 999)
		{
			qian_wei = (int_num/1000) %10;
			num_num = 4;
		}


	if(shifen_wei)
	{
		if(bai_wei || qian_wei)
			{
				is_show_dot = 0;
			}
		else
			{
				is_show_dot = 1;
			}
	}
	if(is_show_dot)
		{
			if(shi_wei)
				{
					insert_image(line_x,pixel_y+(offset*(font*4 + 2)),font*8,(font*4 + 2),(font_num + (font*(4*font + 2) *shi_wei)));
					offset ++;
				}
			insert_image(line_x,pixel_y+(offset*(font*4 + 2)),font*8,(font*4 + 2),(font_num + (font*(4*font + 2) *ge_wei)));
			offset ++;
			insert_image(line_x,pixel_y+(offset*(font*4 + 2)),font*8,(font*4 + 2),(font_num + (font*(4*font + 2) *10)));
			offset ++;
			insert_image(line_x,pixel_y+(offset*(font*4 + 2)),font*8,(font*4 + 2),(font_num + (font*(4*font + 2) *shifen_wei)));
		}
	else
		{
			if(num_num > 3)
				{
					insert_image(line_x,pixel_y+(offset*(font*4 + 2)),font*8,(font*4 + 2),(font_num + (font*(4*font + 2) *qian_wei)));
					offset ++;
				}
			if(num_num > 2)
				{
					insert_image(line_x,pixel_y+(offset*(font*4 + 2)),font*8,(font*4 + 2),(font_num + (font*(4*font + 2) *bai_wei)));
					offset ++;
				}
			if(num_num > 1)
				{
					insert_image(line_x,pixel_y+(offset*(font*4 + 2)),font*8,(font*4 + 2),(font_num + (font*(4*font + 2) *shi_wei)));
					offset ++;
				}
			insert_image(line_x,pixel_y+(offset*(font*4 + 2)),font*8,(font*4 + 2),(font_num + (font*(4*font + 2) *ge_wei)));
		}
	return offset;
	
}


void display_temp(unsigned char line_x,unsigned short pixel_y,float temp,unsigned char font)
{
	unsigned char* font_num;
	unsigned char signal = 0;
	if(2 == font)
		{
			font_num = (unsigned char*)gImage_16_10;
		}
	else if(1 == font)
		{
			font_num = (unsigned char*)gImage_8_6;
		}
	else
		{
			return;
		}
	
	unsigned char offset;
	if(temp<0)
	{
		insert_image(line_x,pixel_y,font*8,(font*4 + 2),(font_num + (font*(4*font + 2) *14)));			
		signal =1;
		temp = -temp;
	}
	offset = display_num(line_x,  pixel_y+signal*(font*4 + 2), temp,font);
	offset += signal;
	offset ++;
	//insert_image(line_x,pixel_y+(offset*10),16,10,(unsigned char*)gImage_16_10[11]);	
	insert_image(line_x,pixel_y+(offset*(font*4 + 2)),font*8,(font*4 + 2),(font_num + (font*(4*font + 2) *11)));
}

void display_humidity(unsigned char line_x,unsigned short pixel_y,float humidity)
{
	unsigned char offset;
	offset = display_num(line_x,  pixel_y, humidity,2);
	offset ++;
	insert_image(line_x,pixel_y+(offset*10),16,10,(unsigned char*)gImage_16_10[12]);	
}

void display_voc(unsigned char line_x,unsigned short pixel_y,unsigned short voc,unsigned char font)
{	
	unsigned char offset;
	offset = display_num(line_x,pixel_y,voc,font);
	offset ++;
	insert_image(line_x,pixel_y+(offset*(font*4 + 2)),16,15,(unsigned char*)gImage_16_15[3]);		
}

void display_pm25(unsigned char line_x,unsigned short pixel_y,unsigned short pm25,unsigned char font)
{
	unsigned char offset;
	offset = display_num(line_x,pixel_y,pm25,font);
	offset ++;
	insert_image(line_x,pixel_y+(offset*(font*4 + 2)),16,15,(unsigned char*)gImage_16_15[1]);	
}

void display_co2(unsigned char line_x,unsigned short pixel_y,unsigned short co2,unsigned char font)
{
	unsigned char offset;
	offset = display_num(line_x,pixel_y,co2,2);
	offset ++;
	insert_image(line_x,pixel_y+(offset*(font*4 + 2)+1),16,15,(unsigned char*)gImage_16_15[0]);	
}

void display_hcho(unsigned char line_x,unsigned short pixel_y,unsigned short hcho,unsigned char font)
{
	unsigned char offset;
	offset = display_num(line_x,pixel_y,hcho,2);
	offset ++;
	insert_image(line_x,pixel_y+(offset*(font*4 + 2)+1),16,15,(unsigned char*)gImage_16_15[3]);	
}

void display_time(unsigned char line_x,unsigned short pixel_y)
{
	RTC_TimeTypeDef rtc_time;
	unsigned show_time[4] = {0};
	
	HAL_RTC_GetTime(&hrtc,&rtc_time,RTC_FORMAT_BIN);
	if(rtc_time.Hours < 10)
		{		
			show_time[1] = rtc_time.Hours % 10;
			insert_image(line_x,pixel_y,8,6,(unsigned char*)gImage_8_6[show_time[1]]);
			insert_image(line_x,pixel_y+6,8,6,(unsigned char*)gImage_8_6[13]);//:
			show_time[2] = rtc_time.Minutes /10;
			insert_image(line_x,pixel_y+12,8,6,(unsigned char*)gImage_8_6[show_time[2]]);	
			show_time[3] = rtc_time.Minutes %10;
			insert_image(line_x,pixel_y+18,8,6,(unsigned char*)gImage_8_6[show_time[3]]);			
		
		}
	else
		{		
			show_time[0] = rtc_time.Hours / 10;
			insert_image(line_x,pixel_y,8,6,(unsigned char*)gImage_8_6[show_time[0]]);	
			show_time[1] = rtc_time.Hours % 10;
			insert_image(line_x,pixel_y+6,8,6,(unsigned char*)gImage_8_6[show_time[1]]);		
			insert_image(line_x,pixel_y+12,8,6,(unsigned char*)gImage_8_6[13]);
			show_time[2] = rtc_time.Minutes /10;
			insert_image(line_x,pixel_y+18,8,6,(unsigned char*)gImage_8_6[show_time[2]]);	
			show_time[3] = rtc_time.Minutes %10;
			insert_image(line_x,pixel_y+24,8,6,(unsigned char*)gImage_8_6[show_time[3]]);						
				
		}

}

void display_target_time(unsigned char line_x,unsigned short pixel_y,unsigned int time_offset,unsigned char flag)
{
	unsigned int time_seconds = 0;
	RTC_TimeTypeDef rtc_time;
	unsigned show_time[6] = {0};	

	rtc_time.Hours = graph_buf.save_time.Hours;
	rtc_time.Minutes= graph_buf.save_time.Minutes;
	rtc_time.Seconds = graph_buf.save_time.Seconds;
	
	time_seconds += rtc_time.Hours * 60 *60;
	time_seconds += rtc_time.Minutes *60;
	time_seconds += rtc_time.Seconds;
	
	
	if(0 == flag)
		{
			time_seconds = time_seconds - time_offset;
		}
	if(1 == flag)
		{
			time_seconds = time_seconds + time_offset;
		}

	rtc_time.Hours = time_seconds / (3600);
	rtc_time.Minutes = (time_seconds%3600) /60;
	rtc_time.Seconds = time_seconds%60;
	
	if(rtc_time.Hours < 10)
		{		
			show_time[1] = rtc_time.Hours % 10;
			insert_image(line_x,pixel_y,8,6,(unsigned char*)gImage_8_6[show_time[1]]);
			
			insert_image(line_x,pixel_y+6,8,6,(unsigned char*)gImage_8_6[13]);//:
			show_time[2] = rtc_time.Minutes /10;
			insert_image(line_x,pixel_y+12,8,6,(unsigned char*)gImage_8_6[show_time[2]]);	
			show_time[3] = rtc_time.Minutes %10;
			insert_image(line_x,pixel_y+18,8,6,(unsigned char*)gImage_8_6[show_time[3]]);		
			
			insert_image(line_x,pixel_y+24,8,6,(unsigned char*)gImage_8_6[13]);//:
			show_time[4] = rtc_time.Seconds/10;
			insert_image(line_x,pixel_y+30,8,6,(unsigned char*)gImage_8_6[show_time[4]]);	
			show_time[5] = rtc_time.Seconds%10;
			insert_image(line_x,pixel_y+36,8,6,(unsigned char*)gImage_8_6[show_time[5]]);
		
		}
	else
		{		
			show_time[0] = rtc_time.Hours / 10;
			insert_image(line_x,pixel_y,8,6,(unsigned char*)gImage_8_6[show_time[0]]);	
			show_time[1] = rtc_time.Hours % 10;
			insert_image(line_x,pixel_y+6,8,6,(unsigned char*)gImage_8_6[show_time[1]]);		
			insert_image(line_x,pixel_y+12,8,6,(unsigned char*)gImage_8_6[13]);
			show_time[2] = rtc_time.Minutes /10;
			insert_image(line_x,pixel_y+18,8,6,(unsigned char*)gImage_8_6[show_time[2]]);	
			show_time[3] = rtc_time.Minutes %10;
			insert_image(line_x,pixel_y+24,8,6,(unsigned char*)gImage_8_6[show_time[3]]);	
			
			insert_image(line_x,pixel_y+30,8,6,(unsigned char*)gImage_8_6[13]);//:
			show_time[4] = rtc_time.Seconds/10;
			insert_image(line_x,pixel_y+36,8,6,(unsigned char*)gImage_8_6[show_time[4]]);	
			show_time[5] = rtc_time.Seconds%10;
			insert_image(line_x,pixel_y+42,8,6,(unsigned char*)gImage_8_6[show_time[5]]);
				
		}

}

void display_menu(void)
{
	clear_all();
	//unsigned char loopx;
	
	load_image_to_buf((unsigned char*) gImage_mem);	
	insert_image(0,277,8,16,(unsigned char*)gImage_8_16[3]);
	insert_image(0,238,8,16,(unsigned char*)gImage_8_16[6]);
	insert_image(0,258,8,16,(unsigned char*)gImage_8_16[8]);
	
	display_time(0,186);
	
	display_pm25(3,40,(unsigned short)(res_pm2_5.pm2_5),2);//pm25
	display_pm25(3,125,(unsigned short)(res_pm2_5.pm10),2);//pm10
	
	display_voc(6,40,(unsigned short)(air_info.tvoc_voc),2);
	display_hcho(6,125,(unsigned short)(hcho_sensor_recv_data.reslut),2);
	
	display_humidity(9,40,23);
	display_co2(9,125,(unsigned short)(co2_sensor_recv_data.reslut),2);
	display_temp(12,40,-27,2);
	display_temp(12,125,-27,2);
	
	EPD_Dis_Part(0,xDot-1,0,yDot-1,(unsigned char *)g_image_buf,1);
	osDelay(200);
}

void refresh_menu(unsigned char mode)//mode 1 part   mode 0 full
{
	//unsigned char loopx;
	load_image_to_buf((unsigned char*) gImage_mem);	
	insert_image(0,277,8,16,(unsigned char*)gImage_8_16[3]);
	insert_image(0,239,8,16,(unsigned char*)gImage_8_16[6]);
	insert_image(0,258,8,16,(unsigned char*)gImage_8_16[8]);

	display_time(0,186);

	display_pm25(3,40,(unsigned short)(res_pm2_5.pm2_5),2);//pm25
	display_pm25(3,125,(unsigned short)(res_pm2_5.pm10),2);//pm10
	
	display_voc(6,40,(unsigned short)(air_info.tvoc_voc),2);
	display_hcho(6,125,(unsigned short)(hcho_sensor_recv_data.reslut),2);
	
	display_humidity(9,40,23);
	display_co2(9,125,(unsigned short)(co2_sensor_recv_data.reslut),2);
	display_temp(12,40,-27,2);
	display_temp(12,125,-27,2);

	if(1 == mode)
		{
			EPD_Dis_Part(0,xDot-1,0,yDot-1,(unsigned char *)g_image_buf,1);
		}
	if(0 == mode)
		{
			EPD_Dis_Full((unsigned char *)g_image_buf,1);
		}
	osDelay(200);
}



void draw_point(unsigned short x,unsigned short y, unsigned char mod)
{
	unsigned short position;
	unsigned short bit;
	position = x /8;
	bit = x % 8;
	bit = 7 - bit;
	position += y * (xDot/8);
	switch(mod)
		{
			case (3):
				g_image_buf[position] ^= (1 << bit);				
				break;

			case (0):
				g_image_buf[position] &= ~(1 << bit);
				break;

			case (1):
				g_image_buf[position] |= (1 << bit);
				break;
		}
}


void draw_line(short x1,short y1, short x2,short y2,unsigned char color)
{
    short dx,dy,e;
    dx=x2-x1; 
    dy=y2-y1;
    if(dx>=0)
    {
        if(dy >= 0) // dy>=0
        {
            if(dx>=dy) // 1/8 octant
            {
                e=dy-dx/2;
                while(x1<=x2)
                {
                    draw_point(x1,y1,color);
                    if(e>0){y1+=1;e-=dx;}   
                    x1+=1;
                    e+=dy;
                }
            }
            else        // 2/8 octant
            {
                e=dx-dy/2;
                while(y1<=y2)
                {
                    draw_point(x1,y1,color);
                    if(e>0){x1+=1;e-=dy;}   
                    y1+=1;
                    e+=dx;
                }
            }
        }
        else           // dy<0
        {
            dy=-dy;   // dy=abs(dy)
            if(dx>=dy) // 8/8 octant
            {
                e=dy-dx/2;
                while(x1<=x2)
                {
                    draw_point(x1,y1,color);
                    if(e>0){y1-=1;e-=dx;}   
                    x1+=1;
                    e+=dy;
                }
            }
            else        // 7/8 octant
            {
                e=dx-dy/2;
                while(y1>=y2)
                {
                    draw_point(x1,y1,color);
                    if(e>0){x1+=1;e-=dy;}   
                    y1-=1;
                    e+=dx;
                }
            }
        }   
    }
    else //dx<0
    {
        dx=-dx;     //dx=abs(dx)
        if(dy >= 0) // dy>=0
        {
            if(dx>=dy) // 4/8 octant
            {
                e=dy-dx/2;
                while(x1>=x2)
                {
                    draw_point(x1,y1,color);
                    if(e>0){y1+=1;e-=dx;}   
                    x1-=1;
                    e+=dy;
                }
            }
            else        // 3/8 octant
            {
                e=dx-dy/2;
                while(y1<=y2)
                {
                    draw_point(x1,y1,color);
                    if(e>0){x1-=1;e-=dy;}   
                    y1+=1;
                    e+=dx;
                }
            }
        }
        else           // dy<0
        {
            dy=-dy;   // dy=abs(dy)
            if(dx>=dy) // 5/8 octant
            {
                e=dy-dx/2;
                while(x1>=x2)
                {
                    draw_point(x1,y1,color);
                    if(e>0){y1-=1;e-=dx;}   
                    x1-=1;
                    e+=dy;
                }
            }
            else        // 6/8 octant
            {
                e=dx-dy/2;
                while(y1>=y2)
                {
                    draw_point(x1,y1,color);
                    if(e>0){x1-=1;e-=dy;}   
                    y1-=1;
                    e+=dx;
                }
            }
        }   
    }
}

void clear_all(void)
{
	EPD_init_Full();
	HAL_Delay(200);
	set_g_image_buf(0xff);
	EPD_Dis_Full((unsigned char *)&g_image_buf,1);  //all white
	HAL_Delay(2500);
	EPD_init_Part();
	HAL_Delay(500);
}

void load_image_to_buf(unsigned char * image)
{
	unsigned short loopx = 0;
	while(loopx < 4736)
		{
			g_image_buf[loopx] = image[loopx];
			loopx ++ ;
		}
}

void set_g_image_buf(unsigned char set_data)
{
	unsigned short loopx = 0;
	unsigned short buf_num = sizeof(g_image_buf);
	while(loopx < buf_num)
		{
			g_image_buf[loopx ++] = set_data;
		}
}

/**********************************
//

***********************************/
void insert_image(unsigned short line_x,unsigned short pixel_y,unsigned short size_x,unsigned short size_y,unsigned char* image)
{
	unsigned short tmp_size_x,tmp_line_x;
	tmp_size_x = size_x / 8;
	tmp_line_x = line_x ;//* 8;
	unsigned short loop_outside,loop_inside;
	unsigned short position;
	for(loop_outside = 0 ; loop_outside < size_y; loop_outside++)
		{
			for(loop_inside = 0 ; loop_inside < (tmp_size_x ) ; loop_inside ++)
				{
					position = (pixel_y + loop_outside) * (xDot/8) + (tmp_line_x+loop_inside);
					g_image_buf[position] = image[loop_outside*tmp_size_x + loop_inside];
				}
		
		}
}

void draw_image(unsigned short pixel_X,unsigned short piexl_y,unsigned short size_x,unsigned short size_y,unsigned char* image,unsigned char draw_type)
{	
	unsigned short tmp_size_x;
	tmp_size_x = size_x / 8;
	unsigned short loop_inside,loop_outside,loop8;	
		for(loop_outside = 0 ; loop_outside < size_y; loop_outside++)
		{
			for(loop_inside = 0 ; loop_inside < (tmp_size_x) ; loop_inside ++)
				{
					for(loop8 = 0 ; loop8 < 8 ; loop8 ++)
						{
							if(1 == draw_type)
								{
									if(0 == BIT(image[loop_outside*tmp_size_x + loop_inside],loop8))//��loop8λΪҪ��ʾ�ĵ�
										{
											draw_point((pixel_X + loop_inside*8+loop8),(piexl_y + loop_outside),0);//��ʾ�����
										}
								}
							if(0 == draw_type)
								{
									if(BIT(image[loop_outside*tmp_size_x + loop_inside],loop8))
										{
											draw_point((pixel_X + loop_inside*8+loop8),(piexl_y + loop_outside),0);//��ʾ�����
										}
								}
						}
				}
		
		}
	
}


void draw_graph()
{
	unsigned char base;
	unsigned draw_num = 0;
	graph_buf.mark_flag = 0;
	base = (xDot -19);
	
	graph_buf.draw_point = graph_buf.start_point;//��ʼ��Ҫ��ʾ�ĵ�һ�����ݵ�λ��
	
	if(graph_buf.save_num < MAX_GRAPH_DRAW_NUM)//��Ļ������ʾ��������
		{
			graph_buf.draw_point= 0;
			graph_buf.mark_full= 0;
			graph_buf.mark_flag |= 1;
			if(graph_buf.save_num > MAX_GRAPH_DRAW_NUM/2)
				{
					graph_buf.mark_half = MAX_GRAPH_DRAW_NUM/2;
					graph_buf.mark_flag |= (1<<1);
				}
		}
	else
		{
			if(graph_buf.start_point > (graph_buf.save_num - MAX_GRAPH_DRAW_NUM))
				{
					graph_buf.draw_point = graph_buf.save_num - MAX_GRAPH_DRAW_NUM;
				}
			graph_buf.mark_full = graph_buf.draw_point;
			graph_buf.mark_half = graph_buf.draw_point + MAX_GRAPH_DRAW_NUM/2;
			graph_buf.mark_zero= graph_buf.draw_point + MAX_GRAPH_DRAW_NUM;
			graph_buf.mark_flag |= 1;
			graph_buf.mark_flag |= (1<<1);
			graph_buf.mark_flag |= (1<<2);
		}

	
	/*if((graph_buf.save_num >  MAX_GRAPH_DRAW_NUM) && ( graph_buf.start_point > (graph_buf.save_num- MAX_GRAPH_DRAW_NUM)))//��Ļ��ʾ����¼������
		{
			graph_buf.draw_point= graph_buf.save_num - MAX_GRAPH_DRAW_NUM;
			graph_buf.mark_full = graph_buf.start_point;
			graph_buf.mark_half = graph_buf.start_point + MAX_GRAPH_DRAW_NUM/2;
			graph_buf.mark_zero= graph_buf.start_point + MAX_GRAPH_DRAW_NUM;
			graph_buf.mark_flag |= 1;
			graph_buf.mark_flag |= (1<<1);
			graph_buf.mark_flag |= (1<<2);
		}
	if((graph_buf.save_num >  MAX_GRAPH_DRAW_NUM) &&((graph_buf.save_num - graph_buf.start_point) < MAX_GRAPH_DRAW_NUM))
		{
			graph_buf.draw_point= graph_buf.save_num - MAX_GRAPH_DRAW_NUM;
			graph_buf.mark_full = graph_buf.draw_point;
			graph_buf.mark_half = graph_buf.draw_point + MAX_GRAPH_DRAW_NUM/2;
			graph_buf.mark_zero= graph_buf.draw_point + MAX_GRAPH_DRAW_NUM;
			graph_buf.mark_flag |= 1;
			graph_buf.mark_flag |= (1<<1);
			graph_buf.mark_flag |= (1<<2);
			
		}*/

	graph_buf.focus = graph_buf.read_focus;//���ù��
	
	if(graph_buf.focus < graph_buf.draw_point)
		{
			graph_buf.focus = graph_buf.draw_point;
		}
	if((graph_buf.save_num < MAX_GRAPH_DRAW_NUM)&&(graph_buf.focus > graph_buf.save_num))
		{
			graph_buf.focus = graph_buf.save_num;
		}
	if((graph_buf.save_num > MAX_GRAPH_DRAW_NUM)&&(graph_buf.focus > (graph_buf.draw_point + MAX_GRAPH_DRAW_NUM)))
		{
			graph_buf.focus = graph_buf.draw_point +MAX_GRAPH_DRAW_NUM;
		}
	graph_buf.focus = graph_buf.focus - graph_buf.draw_point;
	set_focus(graph_buf.focus);
	
	
	unsigned short loopx;
	for(loopx = (7 + MAX_GRAPH_DRAW_NUM*3) ; loopx > 7 ; loopx -=3)
		{
			if(draw_num > graph_buf.save_num)
				{
					break;
				}
			if(draw_num > MAX_GRAPH_DRAW_NUM)
				{
					break;
				}
			
			draw_line((base - graph_buf.buf[graph_buf.draw_point]),loopx,base,loopx,0);	
			draw_line((base - graph_buf.buf[graph_buf.draw_point]),loopx -1,base,loopx -1,0);
			graph_buf.draw_point ++;
			//draw_line((base - graph_buf.buf[start_point +offset ]),loopx+2,base,loopx+2,0);
			draw_num ++ ;
		}

		display_num(15,166,(graph_buf.mark_full),1);
		if(((graph_buf.mark_flag)&(1<<1)))
			{
				display_num(15,(5 + (MAX_GRAPH_DRAW_NUM / 2)*3),( graph_buf.mark_half),1);
			}
		if(((graph_buf.mark_flag)&(1<<2)))
			{
				display_num(15,5,( graph_buf.mark_zero),1);
			}
	/*
	display_num(15,5,(MAX_SAVE_DATA_NUM - graph_buf.start_point),1);

	if((graph_buf.start_point + (MAX_GRAPH_DRAW_NUM / 2)) < MAX_SAVE_DATA_NUM)
		{
			display_num(15,(5 + (MAX_GRAPH_DRAW_NUM / 2)*3),(MAX_SAVE_DATA_NUM - graph_buf.start_point - MAX_GRAPH_DRAW_NUM/2),1);
		}
	
	if((graph_buf.start_point + MAX_GRAPH_DRAW_NUM) < MAX_SAVE_DATA_NUM)
		{
			display_num(15,166,(MAX_SAVE_DATA_NUM - graph_buf.start_point - MAX_GRAPH_DRAW_NUM),1);
		}*/		

}

void display_graph(void)
{
	switch(machine_info.cur_display_data)
		{
			case 1:									
			load_data_to_graph_buf(&co2_data);
			break;
			case 2:									
			load_data_to_graph_buf(&pm25_data);
			break;
			case 3:									
			load_data_to_graph_buf(&pm10_data);
			break;
			case 4:									
			load_data_to_graph_buf(&voc_data);
			break;
			case 5:									
			load_data_to_graph_buf(&hcho);
			break;
			case 6:									
			load_data_to_graph_buf(&temp_data);			
			break;
			case 7:									
			load_data_to_graph_buf(&hum_data);
			break;
		}
	//load_data_to_graph_buf(data);
	
	load_image_to_buf((unsigned char*) gImage_graph);	
	draw_graph();
	switch(graph_buf.owner)
		{
			case data_of_pm25:
				insert_image(1,250,16,38,(unsigned char*)gImage_16_38[0]);	
				display_pm25(4,180+38,graph_buf.max_data,2);
				display_pm25(7,180+38,graph_buf.min_data,2);	
				display_pm25(10,180+38,graph_buf.data_buf[(graph_buf.start_point + graph_buf.focus)],2);			
				break;
			case data_of_pm10:
				insert_image(1,250,16,38,(unsigned char*)gImage_16_38[8]);	
				display_pm25(4,180+38,graph_buf.max_data,2);
				display_pm25(7,180+38,graph_buf.min_data,2);	
				display_pm25(10,180+38,graph_buf.data_buf[(graph_buf.start_point + graph_buf.focus)],2);			
				break;
			case data_of_tvoc:
				insert_image(1,250,16,38,(unsigned char*)gImage_16_38[1]);	
				display_voc(4,180+38,graph_buf.max_data,2);
				display_voc(7,180+38,graph_buf.min_data,2);	
				display_voc(10,180+38,graph_buf.data_buf[(graph_buf.start_point + graph_buf.focus)],2);			
				break;
			case data_of_co2:
				insert_image(1,250,16,38,(unsigned char*)gImage_16_38[9]);	
				display_co2(4,180+38,graph_buf.max_data,2);
				display_co2(7,180+38,graph_buf.min_data,2);	
				display_co2(10,180+38,graph_buf.data_buf[(graph_buf.start_point + graph_buf.focus)],2);
				break;
			case data_of_temp:
				insert_image(1,250,16,38,(unsigned char*)gImage_16_38[3]);	
				display_temp(4,180+38,graph_buf.max_data,2);
				display_temp(7,180+38,graph_buf.min_data,2);	
				display_temp(10,180+38,graph_buf.data_buf[(graph_buf.start_point + graph_buf.focus)],2);			
				break;
			case data_of_hum:
				insert_image(1,250,16,38,(unsigned char*)gImage_16_38[2]);
				display_humidity(4,180+38,graph_buf.max_data);
				display_humidity(7,180+38,graph_buf.min_data);	
				display_humidity(10,180+38,graph_buf.data_buf[(graph_buf.start_point + graph_buf.focus)]);				
				break;
			case data_of_hcho:
				insert_image(1,250,16,38,(unsigned char*)gImage_16_38[7]);	
				display_hcho(4,180+38,graph_buf.max_data,2);
				display_hcho(7,180+38,graph_buf.min_data,2);	
				display_hcho(10,180+38,graph_buf.data_buf[(graph_buf.start_point + graph_buf.focus)],2);			
				break;
		}
	
	insert_image(4,180,16,38,(unsigned char*)gImage_16_38[5]);
	//display_temp(3,180+38,graph_buf.max_data,1);
	//display_co2(4,180+38,graph_buf.max_data,2);
	
	insert_image(7,180,16,38,(unsigned char*)gImage_16_38[4]);
	//display_temp(6,180+38,graph_buf.min_data,2);
	//display_co2(7,180+38,graph_buf.min_data,2);
	
	insert_image(10,180,16,38,(unsigned char*)gImage_16_38[6]);
	//display_temp(9,180+38,graph_buf.data_buf[(graph_buf.start_point + graph_buf.focus)],1);//��ʾ����ֵ
	//display_co2(10,180+38,graph_buf.data_buf[(graph_buf.start_point + graph_buf.focus)],2);

	insert_image(13,180,16,38,(unsigned char*)gImage_16_38[6]);
	display_target_time(14,180+38,(graph_buf.start_point + graph_buf.focus)*(graph_buf.time_step),0);
	
	//insert_image(14,1,16,10,(unsigned char*)gImage_16_10[16]);
	//insert_image(14,20,8,6,(unsigned char*)gImage_8_6[16]);
	//display_num(14,166,0,1);
	//display_num(14,6,30,1);

	EPD_Dis_Part(0,xDot-1,0,yDot-1,(unsigned char *)g_image_buf,1);
	osDelay(200);
}

void set_focus(unsigned char point)
{
	unsigned position;
	position = (170 - 3*point);
	insert_image(14,position,8,6,(unsigned char*)gImage_8_6[16]);
}


void load_data_to_graph_buf(P_S_Save_Data data)
{
	data->max_data = max_in_n_buf(data->buf,data->save_num);
	data->min_data = min_in_n_buf(data->buf,data->save_num);
	
	unsigned char loopx = data->save_num;
	data->operat_point = data->point;
	graph_buf.save_num = data->save_num;
	graph_buf.max_data = data->max_data;
	graph_buf.min_data = data->min_data;
	graph_buf.owner = data->type;
	for(loopx = 0 ; loopx < (data->save_num) ; loopx ++)
		{
			//graph_buf.buf[loopx] = data->buf[data->operat_point];
			graph_buf.buf[loopx] = data->f_change(data->max_data,data->min_data,data->buf[data->operat_point]);
			graph_buf.data_buf[loopx] = data->buf[data->operat_point];
			data->operat_point -- ;
		}
	HAL_RTC_GetTime(&hrtc,&(graph_buf.save_time),RTC_FORMAT_BIN);

}


void push_data(P_S_Save_Data save_data,float data)
{
	save_data->buf[save_data->point++] = data;
	save_data->save_num ++;
	save_data->save_num = (save_data->save_num > 127)? 127 : save_data->save_num;
	save_data->f_change = common_change_function;
}



void test_function(void)
{
	/*short p_x,p_y,num,x,y,kx,ky;
	unsigned int rand;
	unsigned short k= 0;
	x=27;
	y=91;
	kx=1;
	ky=1;
	p_x = 38;
	p_y = 225;
	num = 0;
	rand = 0X20009000;
	for (x = 0 ; x < 128 ; x ++)
		{
			temp_data.buf[x] = (*((unsigned short*)(rand++)))%1000;
			//temp_data.buf[x] =( x * 4)%1000;
		}*/

	display_menu();
	HAL_Delay(3000);
	
	//clear_all();
	
	graph_buf.read_focus = 1;
	graph_buf.start_point = 0;

}


void set_cursor(unsigned char direction)
{
	if(1 == direction)//�����ƶ�
		{
			if(graph_buf.read_focus > 1)
				{
					graph_buf.read_focus --;
				}
			if(graph_buf.read_focus < graph_buf.start_point)
				{
					graph_buf.start_point = graph_buf.read_focus;
				}
		
		}
	if(0 == direction)//�����ƶ�
		{
			if(graph_buf.read_focus < graph_buf.save_num -1)
				{
					graph_buf.read_focus ++;				
				}
			if(graph_buf.read_focus >( graph_buf.start_point + MAX_GRAPH_DRAW_NUM))
				{
					graph_buf.start_point = graph_buf.read_focus -MAX_GRAPH_DRAW_NUM ;
				}
		}
	display_graph();
}



const unsigned char gImage_16_10[][20] = {
	{ /* 0X81,0X01,0X0A,0X00,0X10,0X00, 0 0 */
0XFF,0XFF,0XF0,0X1F,0XE7,0XCF,0XDF,0XF7,0XDF,0XF7,0XDF,0XF7,0XCF,0XEF,0XE0,0X1F,
0XFF,0XFF,0XFF,0XFF,},

{ /* 0X81,0X01,0X0A,0X00,0X10,0X00, 1 1*/
0XFF,0XFF,0XF7,0XF7,0XEF,0XF7,0XEF,0XF7,0XE0,0X07,0XDF,0XF7,0XFF,0XF7,0XFF,0XF7,
0XFF,0XFF,0XFF,0XFF,},

{ /* 0X81,0X01,0X0A,0X00,0X10,0X00, 2 2*/
0XFF,0XFF,0XFF,0XF7,0XDF,0XE7,0XDF,0XD7,0XDF,0X37,0XDE,0X77,0XCC,0XF7,0XE1,0XF7,
0XFF,0XFF,0XFF,0XFF,},

{ /* 0X81,0X01,0X0A,0X00,0X10,0X00,3 3 */
0XFF,0XFF,0XFF,0XEF,0XDF,0XF7,0XDE,0XF7,0XDC,0XF7,0XDD,0XF7,0XE3,0X0F,0XF7,0X1F,
0XFF,0XFF,0XFF,0XFF,},

{ /* 0X81,0X01,0X0A,0X00,0X10,0X00,4 4 */
0XFF,0XFF,0XFF,0XBF,0XFF,0X3F,0XFC,0XBF,0XF9,0XBF,0XE7,0XBF,0XCF,0X9F,0XC0,0X07,
0XFF,0XBF,0XFF,0XBF,},

{ /* 0X81,0X01,0X0A,0X00,0X10,0X00,5  5*/
0XFF,0XFF,0XFF,0XEF,0XC0,0XF7,0XDC,0XF7,0XDE,0XF7,0XDE,0XF7,0XDE,0X4F,0XFF,0X1F,
0XFF,0XFF,0XFF,0XFF,},

{ /* 0X81,0X01,0X0A,0X00,0X10,0X00, 6 6*/
0XFF,0XFF,0XF8,0X1F,0XE2,0X4F,0XEE,0XF7,0XDE,0XF7,0XDE,0XF7,0XDE,0XE7,0XDF,0X0F,
0XFF,0XBF,0XFF,0XFF,},

{ /* 0X81,0X01,0X0A,0X00,0X10,0X00, 7 7*/
0XFF,0XFF,0XFF,0XFF,0XDF,0XFF,0XDF,0XFF,0XDF,0XC7,0XDF,0X0F,0XDC,0X7F,0XC1,0XFF,
0XCF,0XFF,0XFF,0XFF,},

{ /* 0X81,0X01,0X0A,0X00,0X10,0X00, 8 8*/
0XFF,0XFF,0XE7,0X8F,0XE1,0X67,0XDC,0XF7,0XDE,0XF7,0XDC,0XF7,0XC9,0X67,0XE3,0X0F,
0XFF,0XFF,0XFF,0XFF,},

{ /* 0X81,0X01,0X0A,0X00,0X10,0X00, 9  9 */
0XFF,0XFF,0XE1,0XFF,0XCC,0XF7,0XDE,0X77,0XDE,0X77,0XDE,0X67,0XEC,0XCF,0XF0,0X3F,
0XFF,0XFF,0XFF,0XFF,},
 {/* 0X81,0X01,0X0A,0X00,0X10,0X00, 10 .  */
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF3,0XFF,0XF3,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,},

{ /* 0X81,0X01,0X0A,0X00,0X10,0X00, 11  ��*/
0XF7,0XFF,0XEB,0XFF,0XF7,0XFF,0XF8,0X0F,0XF3,0XE7,0XE7,0XF3,0XEF,0XFB,0XEF,0XF7,
0XF7,0XEF,0XFF,0XFF,},

{ /* 0X81,0X01,0X0A,0X00,0X10,0X00, 12  % */
0XFF,0XFF,0XE1,0XFF,0XDE,0XF7,0XE1,0XCF,0XFF,0X1F,0XF8,0XFF,0XE7,0X0F,0XDE,0XF7,
0XFF,0X0F,0XFF,0XFF,},

{ /* 0X81,0X01,0X0A,0X00,0X10,0X00, 13  : */
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF3,0XCF,0XF3,0XCF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,},

{ /* 0X81,0X01,0X0A,0X00,0X10,0X00,  14  - */
0XFF,0XFF,0XFF,0XFF,0XFE,0X7F,0XFE,0X7F,0XFE,0X7F,0XFE,0X7F,0XFE,0X7F,0XFE,0X7F,
0XFF,0XFF,0XFF,0XFF,},

{ /* 0X81,0X01,0X0A,0X00,0X10,0X00, 15  + */
0XFF,0XFF,0XFE,0X7F,0XFE,0X7F,0XFE,0X7F,0XE0,0X07,0XE0,0X07,0XFE,0X7F,0XFE,0X7F,
0XFE,0X7F,0XFF,0XFF,},

{ /* 0X81,0X01,0X0A,0X00,0X10,0X00,  16 ��*/
0XFF,0XFF,0XFB,0XFF,0XF3,0XFF,0XE0,0X03,0XC0,0X03,0XC0,0X03,0XE0,0X03,0XF3,0XFF,
0XFB,0XFF,0XFF,0XFF,},

{ /* 0X81,0X01,0X0A,0X00,0X10,0X00, 17 ��*/
0XFF,0XFF,0XFF,0XDF,0XFF,0XCF,0XC0,0X07,0XC0,0X03,0XC0,0X03,0XC0,0X07,0XFF,0XCF,
0XFF,0XDF,0XFF,0XFF,},

{ /* 0X81,0X01,0X0A,0X00,0X10,0X00, 18 �� */
0XFE,0X7F,0XE0,0X01,0X85,0XF9,0XFD,0XE3,0XFC,0X1F,0X81,0XFF,0XFC,0X01,0X9D,0XFD,
0XC5,0XE1,0XFF,0XFF,},

{ /* 0X81,0X01,0X0A,0X00,0X10,0X00,19 �� */
0XFF,0XFF,0XE0,0X01,0XED,0X7D,0XAD,0X23,0XCD,0X0F,0XED,0X63,0XED,0X4D,0XE0,0X1D,
0XFF,0XBD,0XFF,0XFF,},

{ /* 0X81,0X01,0X0A,0X00,0X10,0X00, 20  ��*/
0XFF,0XFF,0XF8,0X3F,0XFB,0X7F,0XFB,0X7F,0X80,0X01,0XFB,0X7F,0XFB,0X7F,0XFB,0X7F,
0XF8,0X3F,0XFF,0XFF,},

{ /* 0X81,0X01,0X0A,0X00,0X10,0X00, 21  ��*/
0XFF,0XFB,0XED,0X63,0XED,0X4D,0X8D,0X1D,0XEC,0X5D,0XE1,0X5D,0XED,0X41,0X8D,0X5D,
0XED,0X5D,0XED,0X7D,}


};

const unsigned char gImage_8_16[][16] = {
{ /* 0X81,0X01,0X10,0X00,0X08,0X00,   0    ����0 */
0XFF,0X81,0X7E,0X7E,0X7E,0X7E,0X7E,0X7E,0X7E,0X7E,0X7E,0X7E,0X3C,0X81,0XE7,0XFF,
},

{ /* 0X81,0X01,0X10,0X00,0X08,0X00, 1  ����3*/
0XFF,0X81,0X00,0X00,0X7E,0X7E,0X7E,0X7E,0X7E,0X7E,0X7E,0X7E,0X3C,0X81,0XE7,0XFF,
},

{ /* 0X81,0X01,0X10,0X00,0X08,0X00, 2 ����5*/
0XFF,0X81,0X00,0X00,0X00,0X00,0X00,0X00,0X7E,0X7E,0X7E,0X7E,0X3C,0X81,0XE7,0XFF,
},

{ /* 0X81,0X01,0X10,0X00,0X08,0X00, 3 ����7*/
0XFF,0X81,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X7E,0X3C,0X81,0XE7,0XFF,
},

{ /* 0X81,0X01,0X10,0X00,0X08,0X00, 4 ����10 */
0XFF,0X81,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X81,0XE7,0XFF,
},


{ /* 0X81,0X01,0X10,0X00,0X08,0X00,  5 ���� charge*/
0XFF,0X81,0X7E,0X7A,0X7A,0X72,0X52,0X42,0X4A,0X4E,0X5E,0X5E,0X3C,0X81,0XE7,0XFF,
},

{ /* 0X81,0X01,0X10,0X00,0X08,0X00, 6  wifi ������*/
0XFF,0XE7,0XE7,0XFF,0XDB,0XDB,0XE7,0XBD,0X99,0X42,0X66,0XBD,0X99,0XC3,0XFF,0XFF,
},

{ /* 0X81,0X01,0X10,0X00,0X08,0X00, 7 wifi δ���� */
0X7E,0X7E,0XBD,0XBD,0XDB,0XDB,0XE7,0X00,0X00,0XE7,0XDB,0XDB,0XBD,0XBD,0X7E,0X7E,
},

{ /* 0X81,0X01,0X10,0X00,0X08,0X00,  8 enocean  ���� */
0XBF,0XDF,0XEF,0X80,0XEF,0XDF,0XBF,0XE7,0XE7,0XFF,0XDB,0XE7,0XBD,0XDB,0XE7,0XFF,
},

{ /* 0X81,0X01,0X10,0X00,0X08,0X00, 9 enocean error*/
0XBF,0XDF,0XEF,0X80,0XEF,0XDF,0XBF,0XFF,0XDD,0XC9,0XE3,0XF7,0XE3,0XC9,0XDD,0XFF,
}

};

const unsigned char gImage_8_6[][6] = {
{ /* 0X81,0X01,0X06,0X00,0X08,0X00, 0 0*/
0XFF,0X81,0X7E,0X7E,0X81,0XFF,},

{ /* 0X81,0X01,0X06,0X00,0X08,0X00,1 1 */
0XFE,0XBE,0X00,0XFE,0XFE,0XFF,},
	
{ /* 0X81,0X01,0X06,0X00,0X08,0X00, 2 2*/
0XFF,0XBC,0X7A,0X76,0X8E,0XFF,},

{ /* 0X81,0X01,0X06,0X00,0X08,0X00,  3 3*/
0XFF,0XBE,0X6E,0X6E,0X91,0XFF,},

 {/* 0X81,0X01,0X06,0X00,0X08,0X00, 4  4*/
0XE3,0XDB,0XBB,0X00,0XFB,0XFF,},


{ /* 0X81,0X01,0X06,0X00,0X08,0X00, 5 5*/
0XFF,0X0E,0X6E,0X6E,0X71,0XFF,},

{ /* 0X81,0X01,0X06,0X00,0X08,0X00, 6 6*/
0X81,0X2E,0X6E,0X6E,0XF1,0XFF,},

{ /* 0X81,0X01,0X06,0X00,0X08,0X00, 7 7*/
0XFF,0X7F,0X78,0X63,0X1F,0XFF,},

{ /* 0X81,0X01,0X06,0X00,0X08,0X00, 8 8 */
0XFF,0X91,0X6E,0X6E,0X91,0XFF,},

{ /* 0X81,0X01,0X06,0X00,0X08,0X00, 9 9*/
0XFF,0X8E,0X76,0X76,0X81,0XFF,},

{ /* 0X81,0X01,0X06,0X00,0X08,0X00, 10  . */
0XFF,0XFF,0XFC,0XFC,0XFF,0XFF,},

{ /* 0X81,0X01,0X06,0X00,0X08,0X00, 11  ��*/
0X9F,0XFF,0XC1,0XBE,0XDD,0XFF,},
	
{ /* 0X81,0X01,0X06,0X00,0X08,0X00, 12 %*/
0XFF,0XCC,0XFB,0XF7,0X8C,0XFF,},
	
{ /* 0X81,0X01,0X06,0X00,0X08,0X00, 13 : */
0XFF,0XFF,0XD7,0XD7,0XFF,0XFF,},

{ /* 0X81,0X01,0X06,0X00,0X08,0X00, 14  - */
0XFF,0XEF,0XEF,0XEF,0XEF,0XFF,},

{ /* 0X81,0X01,0X06,0X00,0X08,0X00, 15 +*/
0XEF,0XEF,0X83,0XEF,0XEF,0XFF,},

{ /* 0X81,0X01,0X06,0X00,0X08,0X00, 16 ��*/
0XDF,0X9F,0X00,0X9F,0XDF,0XFF,},

{ /* 0X81,0X01,0X06,0X00,0X08,0X00, 17 ��*/
0XFB,0XF9,0X00,0XF9,0XFB,0XFF,},

{ /* 0X81,0X01,0X06,0X00,0X08,0X00, 18 ��*/
0XEF,0XC7,0X83,0XEF,0XEF,0XEF,},

{ /* 0X81,0X01,0X06,0X00,0X08,0X00, 19 ��*/
0XEF,0XEF,0XEF,0X83,0XC7,0XEF,}



};

const unsigned char gImage_16_15[][30] = {
	
{ /* 0X81,0X01,0X0F,0X00,0X10,0X00, 0  ppm */
0XE0,0X01,0XEF,0X7F,0XF0,0XFF,0XFF,0XFF,0XE0,0X01,0XEF,0X7F,0XF0,0XFF,0XFF,0XFF,
0XE0,0X01,0XF1,0XFF,0XFE,0X3F,0XFF,0XC7,0XFE,0X3F,0XF1,0XFF,0XE0,0X01,},

{ /* 0X81,0X01,0X0F,0X00,0X10,0X00, 1  ug */
0XFF,0XFF,0XFF,0XFF,0XC0,0X7F,0XFF,0XBF,0XFF,0XBF,0XFF,0X7F,0XC0,0X1F,0XFF,0XFF,
0XF0,0X3B,0XEF,0XDD,0XEF,0XDD,0XEF,0XBD,0XF0,0X03,0XFF,0XFF,0XFF,0XFF,},

{ /* 0X81,0X01,0X0F,0X00,0X10,0X00, 2 ppb*/
0XFF,0XFF,0XE0,0X01,0XEF,0X7F,0XF0,0XFF,0XFF,0XFF,0XFF,0XFF,0XE0,0X01,0XEF,0X7F,
0XF0,0XFF,0XFF,0XFF,0XFF,0XFF,0XE0,0X01,0XFF,0XBD,0XFF,0XC3,0XFF,0XFF,},

{ /* 0X81,0X01,0X0F,0X00,0X10,0X00, 3 mg */
0XFF,0XFF,0X80,0X1F,0XDF,0XFF,0XDF,0XFF,0XE0,0X1F,0XDF,0XFF,0XDF,0XFF,0XE0,0X1F,
0XFF,0XFF,0XE0,0X3B,0XDF,0XDD,0XDF,0XDD,0XDF,0XBD,0XE0,0X03,0XFF,0XFF,},

};


const unsigned char gImage_16_38[][76] = {

{ /* 0X81,0X01,0X26,0X00,0X10,0X00,  0  pm25  */
0XFF,0XFF,0X80,0X03,0XBF,0X7F,0XBF,0X7F,0XBF,0X7F,0XCC,0XFF,0XF3,0XFF,0XFF,0XFF,
0X80,0X03,0X8F,0XFF,0XF1,0XFF,0XFE,0X1F,0XFF,0XE7,0XFE,0X1F,0XF1,0XFF,0X8F,0XFF,
0X80,0X03,0XFF,0XFF,0XFF,0XFF,0XFF,0XF3,0XCF,0XF3,0XBF,0X83,0XBE,0X33,0XC0,0XF3,
0XE7,0XF3,0XFF,0XFF,0XFF,0XF3,0XFF,0XF3,0XFF,0XEF,0X81,0XF3,0XBD,0XF3,0XBD,0XF3,
0XBE,0X6F,0XFF,0X9F,0XFF,0XFF,0XF3,0XCF,0XF3,0XCF,0XFF,0XFF,},

{ /* 0X81,0X01,0X26,0X00,0X10,0X00, 1  voc   */
0XFF,0XFF,0XFF,0XFF,0X9F,0XFF,0X81,0XFF,0XF8,0X0F,0XFF,0XC1,0XFF,0XF9,0XFF,0XC1,
0XF8,0X0F,0X81,0XFF,0X9F,0XFF,0XFF,0XFF,0XFF,0XFF,0XF8,0X0F,0XE7,0XF3,0XDF,0XFD,
0XDF,0XFD,0XDF,0XFD,0XDF,0XFD,0XDF,0XFD,0XE7,0XF3,0XF8,0X0F,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XF8,0X07,0XE7,0XF3,0XDF,0XFD,0XDF,0XFD,0XDF,0XFD,0XDF,0XFD,0XDF,0XFD,
0XE7,0XF3,0XFF,0XFF,0XFF,0XFF,0XF9,0XE7,0XF9,0XE7,0XFF,0XFF,},

{ /* 0X81,0X01,0X26,0X00,0X10,0X00, 2  ʪ��*/
0XFF,0XFF,0XFB,0XFF,0XB9,0XF1,0XCC,0X87,0XEE,0XBF,0XFF,0XFD,0XC1,0X9D,0XD5,0XE5,
0XD5,0XFD,0XD5,0X01,0XD5,0XFD,0XD5,0XFD,0XD5,0X01,0XD5,0XF5,0XD5,0XE5,0XC1,0X9D,
0XFF,0XFD,0XFF,0XFD,0XFF,0XF1,0XE0,0X07,0XEF,0XFD,0XED,0XDD,0XED,0XDD,0XE8,0X4B,
0XED,0X53,0XAD,0X5B,0XCD,0X5B,0XED,0X53,0XE8,0X4B,0XED,0XCD,0XED,0XDD,0XED,0XFD,
0XEF,0XFF,0XFF,0XFF,0XF9,0XE7,0XF9,0XE7,0XFF,0XFF,0XFF,0XFF,},

{ /* 0X81,0X01,0X26,0X00,0X10,0X00, 3  �¶�*/
0XFF,0XFF,0XFB,0XFE,0XB9,0XF8,0XCC,0X83,0XEE,0XBD,0XFF,0XFD,0XFF,0X81,0XC0,0XBD,
0XD5,0XBD,0XD5,0X81,0XD5,0XBD,0XD5,0XBD,0XD5,0X81,0XD5,0XBD,0XC0,0XBD,0XFF,0X81,
0XFF,0XFD,0XFF,0XFD,0XFF,0XFD,0XFF,0XF1,0XE0,0X07,0XEF,0XFD,0XED,0XDD,0XED,0XDD,
0XE8,0X4B,0XED,0X53,0XAD,0X5B,0XCD,0X5B,0XED,0X53,0XE8,0X4B,0XED,0XCD,0XED,0XDD,
0XED,0XFD,0XEF,0XFF,0XFF,0XFF,0XF9,0XE7,0XF9,0XE7,0XFF,0XFF,},

{ /* 0X81,0X01,0X26,0X00,0X10,0X00,  4 ��С */
0XFF,0XFF,0XFE,0XFF,0XFE,0XFB,0XFE,0X3B,0XC0,0X83,0XD4,0XDF,0XD4,0XD7,0XD4,0XD7,
0XD4,0X05,0XD4,0XE1,0XD4,0XF5,0XD4,0XBB,0XD4,0X9B,0XD4,0XA7,0XD4,0XB7,0XC0,0XAB,
0XFE,0X9B,0XFE,0XBD,0XFE,0XFF,0XFF,0XFF,0XFF,0XDF,0XFF,0X3F,0XFC,0X7F,0XF9,0XFD,
0XFF,0XFD,0XFF,0XFD,0XFF,0XFD,0XC0,0X03,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFB,0XFF,
0XFC,0XFF,0XFE,0X3F,0XFF,0X9F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,},

{ /* 0X81,0X01,0X26,0X00,0X10,0X00,5 ��� */
0XFF,0XFF,0XFE,0XFF,0XFE,0XFB,0XFE,0X3B,0XC0,0X83,0XD4,0XDF,0XD4,0XD7,0XD4,0XD7,
0XD4,0X05,0XD4,0XE1,0XD4,0XF5,0XD4,0XBB,0XD4,0X9B,0XD4,0XA7,0XD4,0XB7,0XC0,0XAB,
0XFE,0X9B,0XFE,0XBD,0XFE,0XFF,0XFF,0XFF,0XFB,0XF9,0XFB,0XFB,0XFB,0XF3,0XFB,0XF7,
0XFB,0XE7,0XFB,0XCF,0XFB,0X9F,0XC0,0X3F,0XC0,0X7F,0XFB,0X3F,0XFB,0X8F,0XFB,0XE7,
0XFB,0XF7,0XFB,0XFB,0XFB,0XFB,0XFB,0XFD,0XFF,0XFD,0XFF,0XFF,},

 { /* 0X81,0X01,0X26,0X00,0X10,0X00, 6   ��*/
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF8,0X0F,0XF8,0X0F,0XF8,0X0F,0XF8,0X0F,0XF8,0X0F,
0XF8,0X0F,0XF8,0X0F,0XF8,0X0F,0XF8,0X0F,0XF8,0X0F,0XF8,0X0F,0XF8,0X0F,0XF8,0X0F,
0XF8,0X0F,0XF8,0X0F,0XC0,0X01,0XC0,0X01,0XE0,0X03,0XE0,0X03,0XE0,0X03,0XF0,0X07,
0XF0,0X07,0XF0,0X0F,0XF8,0X0F,0XF8,0X0F,0XFC,0X1F,0XFC,0X1F,0XFC,0X1F,0XFE,0X3F,
0XFE,0X3F,0XFF,0X7F,0XFF,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,},

{ /* 0X81,0X01,0X26,0X00,0X10,0X00, 7  ��ȩ*/
0XFF,0XFF,0X80,0X1F,0XBB,0XBF,0XBB,0XBF,0XBB,0XBF,0XBB,0XBF,0XBB,0XBF,0X80,0X01,
0XBB,0XBF,0XBB,0XBF,0XBB,0XBF,0XBB,0XBF,0XBB,0XBF,0X80,0X1F,0XFF,0XFF,0XFF,0XFF,
0XB0,0X01,0XB7,0XAB,0XB7,0X2B,0X80,0X6B,0XB7,0XEB,0X80,0X6B,0XB7,0X6B,0XB0,0X01,
0XDE,0XFB,0X0D,0XDB,0XDA,0XDB,0XD6,0XDB,0XCE,0X03,0XD6,0XDB,0X0A,0XDB,0XDD,0XFB,
0XDE,0XFB,0XFF,0XFF,0XFF,0XFF,0XF3,0XCF,0XF3,0XCF,0XFF,0XFF,},

{ /* 0X81,0X01,0X26,0X00,0X10,0X00,  8  pm10 */
0XFF,0XFF,0XC0,0X03,0XDF,0X7F,0XDF,0X7F,0XCF,0X7F,0XE0,0XFF,0XFF,0XFF,0XFF,0XFF,
0XC0,0X03,0XC7,0XFF,0XF8,0XFF,0XFF,0X0F,0XFF,0XF3,0XFF,0X0F,0XF8,0XFF,0XC7,0XFF,
0XC0,0X03,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XEF,0XFB,0XCF,0XFB,0XC0,0X03,0XFF,0XFB,
0XFF,0XFB,0XFF,0XFF,0XFF,0XFF,0XE0,0X07,0XCF,0XF3,0XDF,0XFB,0XCF,0XF3,0XE0,0X07,
0XFF,0XFF,0XFF,0XFF,0XF9,0XE7,0XF9,0XE7,0XFF,0XFF,0XFF,0XFF,},

{ /* 0X81,0X01,0X26,0X00,0X10,0X00, 9 co2 */
0XFF,0XFF,0XFC,0X1F,0XF0,0X0F,0XE3,0XC7,0XE7,0XF3,0XCF,0XF3,0XCF,0XF3,0XCF,0XF3,
0XCF,0XF3,0XCF,0XF3,0XC7,0XE3,0XE7,0XE7,0XFF,0XFF,0XFC,0X3F,0XF0,0X0F,0XE3,0XC7,
0XE7,0XE7,0XCF,0XF3,0XCF,0XF3,0XCF,0XF3,0XCF,0XF3,0XCF,0XF3,0XE7,0XE7,0XE3,0XC7,
0XF0,0X1F,0XFC,0X3F,0XFF,0XFF,0XFF,0X7B,0XFE,0X73,0XFE,0XEB,0XFE,0XDB,0XFF,0X3B,
0XFF,0XFF,0XFF,0XFF,0XF9,0XE7,0XF9,0XE7,0XFF,0XFF,0XFF,0XFF,},

};


const unsigned char gImage_mem[4736] ={ /* 0X81,0X01,0X28,0X01,0X80,0X00, */
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF7,0XFD,0XFF,0XFF,
0XFF,0XFF,0XFF,0X80,0X03,0XFF,0X3F,0XFF,0XFF,0XF7,0XFF,0XFF,0X73,0XF1,0XFF,0XFF,
0XFF,0XFF,0XFF,0XBF,0X7F,0XFF,0X03,0XFF,0XFF,0X73,0XE3,0XFF,0X99,0X07,0XFF,0XFF,
0XFF,0XFF,0XFF,0XBF,0X7F,0XFF,0XF0,0X1F,0XFF,0X99,0X0F,0XFF,0XDD,0X7B,0XFF,0XFF,
0XFF,0XFF,0XFF,0XBF,0X7F,0XFF,0XFF,0X83,0XFF,0XDD,0X7F,0XFF,0XFF,0XFB,0XFF,0XFF,
0XFF,0XFF,0XFF,0XCC,0XFF,0XFF,0XFF,0XF3,0XFF,0XFF,0XFB,0XFF,0XFF,0X03,0XFF,0XFF,
0XFF,0XFF,0XFF,0XF3,0XFF,0XFF,0XFF,0X83,0XFF,0X83,0X3B,0XFF,0X81,0X7B,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF0,0X1F,0XFF,0XAB,0XCB,0XFF,0XAB,0X7B,0XFF,0XFF,
0XFF,0XFF,0XFF,0X80,0X03,0XFF,0X03,0XFF,0XFF,0XAB,0XFB,0XFF,0XAB,0X03,0XFF,0XFF,
0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0X3F,0XFF,0XFF,0XAA,0X03,0XFF,0XAB,0X7B,0XFF,0XFF,
0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XAB,0XFB,0XFF,0XAB,0X7B,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFE,0X1F,0XFF,0XFF,0XFF,0XFF,0XAB,0XFB,0XFF,0XAB,0X03,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XE7,0XFF,0XF0,0X1F,0XFF,0XAA,0X03,0XFF,0XAB,0X7B,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFE,0X1F,0XFF,0XCF,0XE7,0XFF,0XAB,0XEB,0XFF,0X81,0X7B,0XFF,0XFF,
0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XBF,0XFB,0XFF,0XAB,0XCB,0XFF,0XFF,0X03,0XFF,0XFF,
0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XBF,0XFB,0XFF,0X83,0X3B,0XFF,0XFF,0XFB,0XFF,0XFF,
0XFF,0XFF,0XFF,0X80,0X03,0XFF,0XBF,0XFB,0XFF,0XFF,0XFB,0XFF,0XFF,0XFB,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XBF,0XFB,0XFF,0XFF,0XFB,0XFF,0XFF,0XFB,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XBF,0XFB,0XFF,0XFF,0XE3,0XFF,0XFF,0XE3,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XF3,0XFF,0XCF,0XE7,0XFF,0XC0,0X0F,0XFF,0XC0,0X0F,0XFF,0XFF,
0XFF,0XFF,0XFF,0XCF,0XF3,0XFF,0XF0,0X1F,0XFF,0XDF,0XFB,0XFF,0XDF,0XFB,0XFF,0XFF,
0XFF,0XFF,0XFF,0XBF,0X8B,0XFF,0XFF,0XFF,0XFF,0XDB,0XBB,0XFF,0XDB,0XBB,0XFF,0XFF,
0XFF,0XFF,0XFF,0XBE,0X3B,0XFF,0XFF,0XFF,0XFF,0XDB,0XBB,0XFF,0XDB,0XBB,0XFF,0XFF,
0XFF,0XFF,0XFF,0XC0,0XFB,0XFF,0XFF,0XFF,0XFF,0XD0,0X97,0XFF,0XD0,0X97,0XFF,0XFF,
0XFF,0XFF,0XFF,0XE7,0XF3,0XFF,0XF0,0X0F,0XFF,0XDA,0XA7,0XFF,0XDA,0XA7,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XCF,0XE7,0XFF,0X5A,0XB7,0XFF,0X5A,0XB7,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFB,0XFF,0XBF,0XFB,0XFF,0X9A,0XB7,0XFF,0X9A,0XB7,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XBF,0XFB,0XFF,0XDA,0XA7,0XFF,0XDA,0XA7,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XE7,0XFF,0XBF,0XFB,0XFF,0XD0,0X97,0XFF,0XD0,0X97,0XFF,0XFF,
0XFF,0XFF,0XFF,0X81,0XFB,0XFF,0XBF,0XFB,0XFF,0XDB,0X9B,0XFF,0XDB,0X9B,0XFF,0XFF,
0XFF,0XFF,0XFF,0XBD,0XFB,0XFF,0XBF,0XFB,0XFF,0XDB,0XBB,0XFF,0XDB,0XBB,0XFF,0XFF,
0XFF,0XFF,0XFF,0XBD,0XFB,0XFF,0XCF,0XE7,0XFF,0XDB,0XFB,0XFF,0XDB,0XFB,0XFF,0XFF,
0XFF,0XFF,0XFF,0XBE,0X67,0XFF,0XFF,0XFF,0XFF,0XDF,0XFF,0XFF,0XDF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0X9F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF3,0XCF,0XFF,0XF3,0XCF,0XFF,0XF3,0XCF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XF3,0XE7,0XFF,0XF3,0XCF,0XFF,0XF3,0XCF,0XFF,0XF3,0XCF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XF3,0XE7,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0X6F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XE3,0X6F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XEC,0X6F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XE6,0X4F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XEB,0X5F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XAB,0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XC0,0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XEF,0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XEF,0X5F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XEF,0X4F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XE3,0X6F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0X6F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XC0,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XDB,0XBF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XDB,0XBF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XC0,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XEF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XE9,0XEF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XEE,0XEF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XEF,0XEF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0X80,0X0F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XEF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XEF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFD,0XEF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XCD,0XEF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XDA,0XEF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XDA,0XEF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XD6,0XEF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0X9E,0XEF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0X9E,0X0F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XD6,0XEF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XDA,0XEF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XDA,0XEF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XCC,0XEF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XEF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFD,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0X00,0X3F,0XFF,0XF8,0X3F,0XFF,0XDA,0XBF,0XFF,0XFF,
0XFF,0XC5,0XFF,0XFF,0XF3,0XFF,0X77,0X7F,0XFF,0XE0,0X1F,0XFF,0XBA,0XBF,0XFF,0XFF,
0XFF,0X95,0XFF,0XDF,0XE7,0XFF,0X77,0X7F,0XFF,0XC7,0X8F,0XFF,0X3A,0XBF,0XFF,0XFF,
0XFF,0XD5,0XFF,0XDF,0X9F,0XFF,0X77,0X7F,0XFF,0XCF,0XE7,0XFF,0X9A,0X9F,0XFF,0XFF,
0XFF,0XD5,0XFF,0XDC,0X7F,0XFF,0X77,0X7F,0XFF,0X9F,0XE7,0XFF,0XA2,0XAF,0XFF,0XFF,
0XFF,0XD5,0XFF,0X03,0XFB,0XFF,0X77,0X7F,0XFF,0X9F,0XE7,0XFF,0XBA,0XB7,0XFF,0XFF,
0XFF,0XD5,0XFF,0XDF,0XFB,0XFF,0X00,0X03,0XFF,0X9F,0XE7,0XFF,0XBA,0XBB,0XFF,0XFF,
0XFF,0XD5,0XFF,0XDC,0XF7,0XFF,0X77,0X7F,0XFF,0X9F,0XE7,0XFF,0XE0,0XBB,0XFF,0XFF,
0XFF,0XD4,0X1F,0XDF,0X6F,0XFF,0X77,0X7F,0XFF,0X9F,0XE7,0XFF,0XDA,0XBB,0XFF,0XFF,
0XFF,0XD7,0XEF,0XDF,0X1F,0XFF,0X77,0X7F,0XFF,0X8F,0XC7,0XFF,0X3A,0XBB,0XFF,0XFF,
0XFF,0XFF,0X9F,0XC0,0X7F,0XFF,0X77,0X7F,0XFF,0XCF,0XCF,0XFF,0X9A,0X03,0XFF,0XFF,
0XFF,0XEE,0X3F,0XDF,0X3F,0XFF,0X77,0X7F,0XFF,0XFF,0XFF,0XFF,0XA2,0XBF,0XFF,0XFF,
0XFF,0XE8,0XFF,0XDF,0X5F,0XFF,0X00,0X3F,0XFF,0XF8,0X7F,0XFF,0XBA,0XBF,0XFF,0XFF,
0XFF,0X80,0X0F,0XDE,0XEF,0XFF,0XFF,0XFF,0XFF,0XE0,0X1F,0XFF,0XBA,0XBF,0XFF,0XFF,
0XFF,0XEC,0XFF,0XDD,0XF3,0XFF,0XFF,0XFF,0XFF,0XC7,0X8F,0XFF,0XBA,0XBF,0XFF,0XFF,
0XFF,0XEA,0X6F,0XDF,0XFB,0XFF,0X60,0X03,0XFF,0XCF,0XCF,0XFF,0XFE,0XF7,0XFF,0XFF,
0XFF,0XF3,0X2F,0XFF,0XFB,0XFF,0X6F,0X57,0XFF,0X9F,0XE7,0XFF,0XF7,0XF7,0XFF,0XFF,
0XFF,0XEB,0XAF,0XFF,0XF7,0XFF,0X6E,0X57,0XFF,0X9F,0XE7,0XFF,0XE6,0XEF,0XFF,0XFF,
0XFF,0XDA,0X2F,0XF7,0X77,0XFF,0X00,0XD7,0XFF,0X9F,0XE7,0XFF,0XD8,0XDF,0XFF,0XFF,
0XFF,0XBB,0XAF,0XE7,0X77,0XFF,0X6F,0XD7,0XFF,0X9F,0XE7,0XFF,0X3A,0XDF,0XFF,0XFF,
0XFF,0XDB,0X8F,0XCF,0X77,0XFF,0X00,0XD7,0XFF,0X9F,0XE7,0XFF,0XF6,0XB7,0XFF,0XFF,
0XFF,0XEA,0X6F,0X9F,0X77,0XFF,0X6E,0XD7,0XFF,0XCF,0XCF,0XFF,0XAE,0XEF,0XFF,0XFF,
0XFF,0XF3,0XEF,0XFF,0X77,0XFF,0X60,0X03,0XFF,0XC7,0X8F,0XFF,0XBF,0X9F,0XFF,0XFF,
0XFF,0XB7,0XFF,0XFB,0X77,0XFF,0XBD,0XF7,0XFF,0XE0,0X3F,0XFF,0X80,0X77,0XFF,0XFF,
0XFF,0X92,0X0F,0X02,0X07,0XFE,0X1B,0XB7,0XFF,0XF8,0X7F,0XFF,0XB9,0XF7,0XFF,0XFF,
0XFF,0XDA,0XEF,0XFF,0X77,0XFF,0XB5,0XB7,0XFF,0XFF,0XFF,0XFF,0XBE,0XEF,0XFF,0XFF,
0XFF,0X80,0X5F,0XFF,0X77,0XFF,0XAD,0XB7,0XFF,0XFE,0XF7,0XFF,0XBF,0X5F,0XFF,0XFF,
0XFF,0XBF,0XBF,0XBF,0X77,0XFF,0X9C,0X07,0XFF,0XFC,0XE7,0XFF,0XB3,0XBF,0XFF,0XFF,
0XFF,0XA0,0X3F,0X9F,0X77,0XFF,0XAD,0XB7,0XFF,0XFD,0XD7,0XFF,0X8B,0X5F,0XFF,0XFF,
0XFF,0XBF,0XBF,0XCF,0X77,0XFE,0X15,0XB7,0XFF,0XFD,0XB7,0XFF,0XFA,0XEF,0XFF,0XFF,
0XFF,0X80,0X5F,0XE7,0X77,0XFF,0XBB,0XF7,0XFF,0XFE,0X77,0XFF,0XF9,0XF7,0XFF,0XFF,
0XFF,0XFF,0XEF,0XF3,0XF7,0XFF,0XBD,0XF7,0XFF,0XFF,0XFF,0XFF,0XFF,0XF7,0XFF,0XFF,
0XFF,0XC0,0X6F,0XFF,0XF7,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XEF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF3,0XCF,0XFF,0XF3,0XCF,0XFF,0XFF,
0XFF,0X80,0X0F,0XF3,0XCF,0XFF,0XE7,0X9F,0XFF,0XF3,0XCF,0XFF,0XF3,0XCF,0XFF,0XFF,
0XFF,0XFE,0XFF,0XF3,0XCF,0XFF,0XE7,0X9F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XF9,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0X80,0X0F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0X9F,0XEF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XCF,0XEF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XF1,0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XBC,0X9F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0X8F,0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XEE,0XBF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XF9,0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XC7,0XCF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XEF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XF8,0X00,0X00,0X7F,0XE0,0X00,0X0E,0X3C,0X1F,0XCF,0XE7,0X9E,0X00,0X00,0X1F,
0XFF,0XF0,0X00,0X00,0X7F,0XE0,0X00,0X0E,0X3C,0X0F,0XCF,0XC7,0X8E,0X00,0X00,0X0F,
0XFF,0XF0,0X00,0X00,0X7F,0XE0,0X00,0X0E,0X3C,0X0F,0XCF,0XC7,0X8E,0X00,0X00,0X0F,
0XFF,0XF0,0XFF,0XFC,0X7F,0X80,0X0F,0XE2,0X00,0X68,0X00,0X99,0X0E,0X3F,0XFF,0X0F,
0XFF,0XF1,0XFF,0XFC,0X7E,0X00,0X0F,0XF0,0X00,0X70,0X00,0X3C,0X0E,0X3F,0XFF,0X8F,
0XFF,0XF1,0XFF,0XFC,0X7E,0X00,0X0F,0XF0,0X00,0X70,0X00,0X3C,0X0E,0X3F,0XFF,0X8F,
0XFF,0XF1,0XFF,0XFC,0X7E,0X08,0X0F,0XF8,0X40,0X70,0X00,0X3C,0X0E,0X3F,0XFF,0X8F,
0XFF,0XF0,0XE0,0X1C,0X7E,0X3C,0X0F,0XFF,0XC0,0X7F,0XFF,0XF8,0X0E,0X38,0X07,0X8F,
0XFF,0XF1,0XE0,0X1C,0X7F,0X38,0X0F,0XFF,0XE0,0X7F,0XFF,0XFC,0X0E,0X38,0X07,0X8F,
0XFF,0XF1,0XE0,0X1C,0X71,0XE0,0X00,0X71,0XFC,0X0E,0X0E,0X3C,0X7E,0X38,0X07,0X8F,
0XFF,0XF1,0XE0,0X1C,0X71,0XE0,0X00,0X71,0XFC,0X0E,0X0E,0X3C,0X7E,0X38,0X07,0X8F,
0XFF,0XF1,0XE0,0X1C,0X71,0XE0,0X00,0X71,0XFC,0X0E,0X0E,0X38,0XFE,0X38,0X07,0X8F,
0XFF,0XF1,0XE0,0X1C,0X7F,0XFF,0XF3,0XFE,0X03,0X8F,0XCE,0X07,0XFE,0X38,0X07,0X8F,
0XFF,0XF1,0XE0,0X1C,0X7F,0XFF,0XF3,0XFE,0X03,0X8F,0XCE,0X07,0XFE,0X38,0X07,0X8F,
0XFF,0XF1,0XE0,0X1C,0X7F,0XFF,0XF3,0XFE,0X07,0X8F,0XCE,0X07,0XFE,0X38,0X07,0X8F,
0XFF,0XF1,0XFF,0XFC,0X7F,0XE0,0X00,0X7E,0X3F,0XF0,0X0F,0XFF,0XFE,0X3F,0XFF,0X8F,
0XFF,0XF1,0XFF,0XFC,0X7F,0XE0,0X00,0X7E,0X3F,0XF0,0X0F,0XFF,0XFE,0X3F,0XFF,0X8F,
0XFF,0XF1,0XFF,0XFC,0X7F,0XC0,0X00,0X7E,0X3F,0XF0,0X0F,0XFF,0XFE,0X3F,0XFF,0X8F,
0XFF,0XF0,0X00,0X00,0X71,0XE3,0X8C,0X71,0XC3,0X8E,0X31,0XC7,0X8E,0X00,0X00,0X0F,
0XFF,0XF0,0X00,0X00,0X71,0XE3,0X8C,0X71,0XE3,0X8E,0X31,0XC7,0X8E,0X00,0X00,0X0F,
0XFF,0XF0,0X00,0X00,0X71,0XC3,0X8C,0X71,0XC7,0X8E,0X31,0XC7,0X8E,0X00,0X00,0X0F,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0X0C,0X7F,0XFF,0XF0,0X3E,0X38,0X7F,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0X0C,0X7F,0XFF,0XF0,0X3E,0X3C,0X7F,0XFF,0XFF,0XFF,
0XFF,0XF8,0X3C,0X00,0X7F,0XE0,0X0C,0X7E,0X3C,0X7E,0X01,0XC0,0X71,0XFE,0X3F,0XFF,
0XFF,0XF0,0X1C,0X00,0X7F,0XE0,0X0C,0X7E,0X3C,0X7E,0X01,0XC0,0X71,0XFE,0X3F,0XFF,
0XFF,0XF0,0X1C,0X00,0X7F,0XE0,0X0C,0X7E,0X3C,0X7E,0X01,0XC0,0X71,0XFE,0X3F,0XFF,
0XFF,0XF0,0X1F,0X03,0XFF,0X38,0X7F,0XF9,0XC0,0X71,0XFF,0XC0,0X4F,0XC1,0XFC,0X1F,
0XFF,0XF0,0X1F,0X83,0XFE,0X3C,0X7F,0XF1,0XC0,0X71,0XFF,0XC0,0X0F,0XC1,0XF8,0X0F,
0XFF,0XF0,0X1F,0X83,0XFE,0X3C,0X7F,0XF1,0XC0,0X71,0XFF,0XC0,0X0F,0XC1,0XF8,0X0F,
0XFF,0XF0,0XBF,0X83,0XBE,0X18,0X7F,0XF0,0XC2,0XFB,0XFF,0XC3,0X0F,0X5B,0XE8,0X0F,
0XFF,0XF1,0XFF,0X9C,0X0E,0X00,0X7F,0XF0,0X03,0XFF,0XFF,0XC7,0X8E,0X3F,0XC0,0X0F,
0XFF,0XF1,0XFF,0X9C,0X0E,0X00,0X7F,0XF0,0X03,0XFF,0XFF,0XC7,0X8E,0X3F,0XC0,0X0F,
0XFF,0XF1,0XFF,0X9C,0X06,0X00,0XFF,0XF0,0X07,0XFF,0XFF,0XC7,0X8E,0X3F,0XE0,0X0F,
0XFF,0XF1,0XFC,0X63,0X81,0XFF,0XF3,0XFE,0X3C,0X7E,0X0E,0X3F,0X8F,0XC0,0X38,0X0F,
0XFF,0XF1,0XFC,0X63,0X81,0XFF,0XF3,0XFE,0X3C,0X7E,0X0E,0X3F,0X8F,0XC0,0X38,0X0F,
0XFF,0XF1,0XFC,0X63,0X81,0XFF,0XF3,0XFE,0X3C,0X7E,0X0E,0X1F,0X0F,0XC0,0X38,0X1F,
0XFF,0XF1,0XE0,0X1C,0X71,0XC3,0X8C,0X01,0XC3,0XFE,0X0E,0X00,0X01,0XC7,0XC7,0XFF,
0XFF,0XF1,0XE0,0X1C,0X71,0XC3,0X8C,0X01,0XC3,0XFE,0X0E,0X00,0X01,0XC7,0XC7,0XFF,
0XFF,0XF1,0XE0,0X1C,0X71,0XE7,0X8C,0X01,0XC3,0XFE,0X0E,0X00,0X01,0XC7,0XC7,0XFF,
0XFF,0XF0,0X00,0X7F,0XF1,0XFF,0X83,0XFF,0XE7,0X81,0XC0,0X03,0XF0,0X38,0X00,0X7F,
0XFF,0XF0,0X00,0X7F,0XF1,0XFF,0X83,0XFF,0XFF,0XFF,0XC0,0X07,0XF0,0X38,0X00,0XFF,
0XFF,0XFF,0XFC,0X7C,0X7E,0X3C,0X73,0XFF,0XFF,0XFF,0XC0,0X00,0X7F,0XFF,0XC0,0X0F,
0XFF,0XFF,0XFC,0X7C,0X7F,0X1C,0X73,0XFF,0X9D,0XFF,0XE0,0X00,0X7F,0XFF,0XC0,0X0F,
0XFF,0XFF,0XFC,0X7C,0X7E,0X1C,0X73,0XFF,0X6C,0XFF,0XE0,0X00,0X7F,0XFF,0XC0,0X0F,
0XFF,0XFF,0XFF,0XE3,0X8F,0XE0,0X7F,0XFF,0X6E,0XFF,0XE1,0XC3,0X80,0X07,0XFF,0X0F,
0XFF,0XFF,0XFF,0XE3,0X8F,0XE0,0X7F,0XFF,0X76,0XFF,0XE1,0XC7,0X80,0X07,0XFF,0X8F,
0XFF,0XFF,0XFF,0XE3,0X8F,0XE0,0X7F,0XFF,0X76,0XFF,0XE1,0XC7,0X80,0X07,0XFF,0X8F,
0XFF,0XF9,0XE3,0X9C,0X01,0XFF,0X8F,0XFF,0XF9,0XFF,0XE1,0XFF,0XF1,0XF9,0XC0,0X7F,
0XFF,0XF1,0XE3,0X9C,0X01,0XFF,0X8F,0XFF,0XFF,0XFF,0XE1,0XFF,0XF1,0XF9,0XC0,0XFF,
0XFF,0XF1,0XE3,0X9C,0X01,0XFF,0X8F,0XFF,0X00,0XFF,0XE1,0XFF,0XF1,0XF9,0XC0,0X7F,
0XFF,0XFF,0X1F,0X83,0X80,0X3F,0X87,0XFF,0X6E,0XFF,0XF0,0X07,0X9F,0XC6,0X38,0X1F,
0XFF,0XFE,0X1F,0X83,0X80,0X3F,0X83,0XFF,0X6E,0XFF,0XF0,0X07,0X8F,0XC6,0X38,0X0F,
0XFF,0XFE,0X1F,0X83,0X80,0X3F,0X83,0XFF,0X6E,0XFF,0XF0,0X07,0X8F,0XC6,0X38,0X0F,
0XFF,0XF9,0XE7,0XC0,0X00,0XE7,0X83,0XFF,0X7E,0XFF,0XEE,0X07,0X8F,0XC7,0XF8,0X7F,
0XFF,0XF1,0XE3,0XE0,0X01,0XE3,0X83,0XFF,0XC3,0XFF,0XEE,0X07,0X8F,0XC7,0XF8,0X7F,
0XFF,0XFB,0XC3,0XF0,0X81,0XE7,0X83,0XFF,0XBD,0XFF,0XEE,0X03,0X1F,0XC7,0XF8,0XFF,
0XFF,0XFE,0X00,0X1F,0XFE,0X1F,0XF3,0XFF,0X7E,0XFF,0XF1,0XC0,0X7E,0X38,0X07,0XFF,
0XFF,0XFE,0X00,0X1F,0XFE,0X3F,0XF3,0XFF,0X7E,0XFF,0XF1,0XC0,0X7E,0X38,0X07,0XFF,
0XFF,0XFE,0X00,0X1F,0XFE,0X1F,0XF3,0XFF,0X7E,0XFF,0XF1,0XC0,0XFE,0X38,0X07,0XFF,
0XFF,0XFE,0X1C,0X60,0X7E,0X03,0X8F,0XFF,0XBD,0XFF,0XFF,0XC3,0X8F,0XFE,0X07,0X8F,
0XFF,0XFF,0X1C,0X60,0X7E,0X03,0X8F,0XFF,0XFF,0XFF,0XFF,0XC7,0X8F,0XFE,0X07,0X8F,
0XFF,0XFE,0X1C,0X60,0X7E,0X07,0X8F,0XFF,0XC3,0XFF,0XFF,0XC7,0X8F,0XFE,0X07,0X8F,
0XFF,0XF0,0X1F,0X83,0XFE,0X1F,0XFF,0XFF,0XBD,0XFF,0XE0,0X3F,0XF1,0XC7,0XFF,0X8F,
0XFF,0XF0,0X1F,0X83,0XFE,0X3F,0XFF,0XFF,0X7E,0XFF,0XE0,0X3F,0XF1,0XC7,0XFF,0X8F,
0XFF,0XF0,0X3F,0X83,0XFE,0X1F,0XFF,0XFF,0X7E,0XFF,0XE0,0X1F,0XF1,0XC7,0XFF,0X8F,
0XFF,0XFF,0XFC,0X00,0X70,0X3C,0X0F,0XFF,0X7E,0XFF,0XFE,0X07,0X81,0XC7,0XC0,0X0F,
0XFF,0XFF,0XFC,0X00,0X70,0X3C,0X0F,0XFF,0XBD,0XFF,0XFE,0X07,0X81,0XC7,0XC0,0X0F,
0XFF,0XFF,0XFC,0X00,0X70,0X1C,0X0F,0XFF,0XC3,0XFF,0XFE,0X07,0X81,0XC7,0XC0,0X0F,
0XFF,0XFE,0X3F,0X9F,0X8F,0XE3,0XF3,0XFF,0XFF,0XFF,0XF0,0X3F,0X8F,0XF8,0X38,0X0F,
0XFF,0XFF,0X1F,0X9F,0X8F,0XC3,0XF3,0X8F,0XFC,0XFF,0XF0,0X3F,0X8F,0XF8,0X38,0X0F,
0XFF,0XFE,0X1F,0X9C,0X01,0XE0,0X7F,0X80,0X3C,0X70,0X30,0X00,0X70,0X01,0XF8,0X7F,
0XFF,0XFF,0X1F,0X9C,0X01,0XC0,0X7F,0X80,0X3C,0X70,0X30,0X00,0X70,0X01,0XF8,0X7F,
0XFF,0XFE,0X1F,0X9C,0X01,0XE0,0X7F,0X80,0X3C,0X70,0X30,0X00,0X70,0X01,0XF8,0X7F,
0XFF,0XFF,0XFC,0X63,0XF0,0X18,0X00,0X0F,0XFF,0XFF,0XCF,0XF8,0X7F,0XF8,0X00,0X1F,
0XFF,0XFF,0XFC,0X63,0XF0,0X3C,0X00,0X0F,0XFF,0XFF,0XCF,0XF8,0X7F,0XF8,0X00,0X0F,
0XFF,0XFF,0XFC,0X63,0XF0,0X3C,0X00,0X0F,0XFF,0XFF,0XCF,0XFC,0X7F,0XF8,0X00,0X0F,
0XFF,0XF8,0X00,0X00,0X70,0XFF,0XFC,0X0F,0X07,0X9E,0X3F,0XE0,0X71,0XC7,0XFF,0X0F,
0XFF,0XF0,0X00,0X00,0X71,0XFF,0XFC,0X0E,0X03,0X8E,0X3F,0XC0,0X71,0XC7,0XFF,0X8F,
0XFF,0XF0,0X00,0X00,0X71,0XFF,0XFC,0X0E,0X03,0X8E,0X3F,0XC0,0X71,0XC7,0XFF,0X0F,
0XFF,0XF0,0X00,0X01,0XF1,0XFF,0XE3,0X8F,0XFF,0X8F,0X79,0XFA,0XFF,0XFF,0XFF,0XDF,
0XFF,0XF0,0X00,0X03,0XF1,0XFC,0X03,0X8F,0XFF,0X8F,0XF1,0XFF,0XFF,0XFF,0XF8,0X7F,
0XFF,0XF0,0X00,0X03,0XF1,0XFC,0X03,0X8F,0XFF,0X8F,0XF1,0XFF,0XFF,0XFF,0XF8,0XFF,
0XFF,0XF8,0X00,0X03,0XF9,0XFC,0X03,0XCF,0XFF,0X8F,0XF1,0XFF,0XFF,0XFF,0XF8,0X7F,
0XFF,0XFF,0XE0,0X00,0X7E,0X03,0X8C,0X7E,0X03,0X8E,0X0E,0X00,0X00,0X00,0X00,0X0F,
0XFF,0XFF,0XE0,0X00,0X7E,0X03,0X8C,0X7E,0X03,0X8E,0X0E,0X00,0X00,0X00,0X00,0X0F,
0XFF,0XFF,0XE0,0X00,0X7F,0X07,0X8C,0X7E,0X03,0X8E,0X0E,0X00,0X00,0X00,0X00,0X0F,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0X70,0X03,0XF0,0X30,0X00,0X0F,0XF8,0X3F,0X8F,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0X70,0X03,0XF0,0X30,0X00,0X0F,0XF8,0X3F,0X8F,
0XFF,0XF0,0X00,0X00,0X7F,0XE3,0X80,0X70,0X00,0X00,0X00,0X3C,0X0E,0X39,0XFF,0X8F,
0XFF,0XF0,0X00,0X00,0X7F,0XE3,0X80,0X70,0X00,0X00,0X00,0X38,0X0E,0X39,0XFF,0X8F,
0XFF,0XF0,0X00,0X00,0X7F,0XE3,0X80,0X70,0X00,0X00,0X00,0X38,0X0E,0X39,0XFF,0X0F,
0XFF,0XF1,0XFF,0XFC,0X71,0XFC,0X00,0X7F,0XFC,0X0E,0X30,0X3C,0X0F,0XF8,0X38,0XFF,
0XFF,0XF1,0XFF,0XFC,0X71,0XFC,0X00,0X7F,0XFC,0X0E,0X30,0X3C,0X0F,0XF8,0X38,0X7F,
0XFF,0XF1,0XFF,0XFC,0X71,0XFC,0X00,0X7F,0XFC,0X0E,0X30,0X3C,0X0F,0XF8,0X38,0XFF,
0XFF,0XF1,0XE0,0X1C,0X71,0XE0,0X7F,0XFE,0X3C,0X0E,0X01,0XC7,0X80,0X01,0XC0,0X7F,
0XFF,0XF1,0XE0,0X1C,0X71,0XE0,0X7F,0XFE,0X3C,0X0E,0X01,0XC7,0X80,0X01,0XC0,0XFF,
0XFF,0XF1,0XE0,0X1C,0X71,0XE0,0X7F,0XFE,0X3C,0X0E,0X01,0XC3,0X80,0X01,0XC0,0X7F,
0XFF,0XF1,0XE0,0X1C,0X70,0X03,0X83,0XF0,0X07,0XFE,0X3F,0XC7,0X8F,0XF9,0XF8,0X7F,
0XFF,0XF1,0XE0,0X1C,0X70,0X03,0X83,0XF0,0X03,0XFE,0X3F,0XC7,0X8F,0XF9,0XF8,0XFF,
0XFF,0XF1,0XE0,0X1C,0X70,0X03,0X83,0XF0,0X03,0XFE,0X3F,0XC3,0X8F,0XF9,0XF8,0X7F,
0XFF,0XF1,0XE0,0X1C,0X7E,0X1F,0X8C,0X70,0X00,0XF0,0X30,0X3F,0X8E,0X06,0X07,0XFF,
0XFF,0XF1,0XE0,0X1C,0X7E,0X3F,0X8C,0X70,0X00,0X70,0X30,0X3F,0X8E,0X06,0X07,0XFF,
0XFF,0XF1,0XFF,0XFC,0X7A,0X1F,0XF3,0X8F,0XC3,0X8E,0X3F,0XFF,0X80,0X38,0X1F,0X9F,
0XFF,0XF1,0XFF,0XFC,0X70,0X3F,0XF3,0X8F,0XE3,0X8E,0X3F,0XFF,0X80,0X38,0X3F,0X8F,
0XFF,0XF1,0XFF,0XFC,0X70,0X3F,0XF3,0X8F,0XC3,0X8E,0X3F,0XFF,0X80,0X38,0X3F,0X8F,
0XFF,0XF0,0XFF,0XFC,0X70,0X1F,0XE1,0X9F,0XC3,0X94,0X1F,0XFF,0X60,0X3C,0X1F,0X8F,
0XFF,0XF0,0X00,0X00,0X70,0X00,0X00,0X7E,0X00,0X70,0X00,0X00,0X70,0X3E,0X07,0X8F,
0XFF,0XF0,0X00,0X00,0X70,0X00,0X00,0X7E,0X00,0X70,0X00,0X00,0X70,0X3E,0X07,0X8F,
0XFF,0XF8,0X00,0X00,0X78,0X00,0X00,0X7F,0X00,0XF0,0X00,0X00,0XF0,0X3E,0X07,0X9F,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
};









const unsigned char gImage_graph[4736] = { /* 0X81,0X01,0X28,0X01,0X80,0X00, */
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X03,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,
0X03,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,
0X03,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X7B,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0X7B,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0X7B,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XFF,0XFF,
0X7B,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0X7B,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0X7B,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0X7B,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X40,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,
0X7B,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X5E,
0X7B,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XDE,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X03,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,
0X03,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,
0X03,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
};













