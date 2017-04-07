#ifndef __MY_PAPER_LIB_H__
#define __MY_PAPER_LIB_H__
#include "my_port.h"

#define xDot 128
#define yDot 296

#define MAX_SAVE_DATA_NUM  128
#define MAX_GRAPH_DRAW_NUM 55

#define BIT(x,n)	((x)&(1<<(n)))

typedef char (*PTRFUN)(float max,float min,float current); 

extern unsigned char g_image_buf[4736];
extern unsigned char g_re_data[10];
extern unsigned char g_re_data2[10];


typedef struct
{
	
	unsigned char draw_point;
	unsigned char write_point:7;
	
	unsigned char focus;
	unsigned char read_focus;
	unsigned char mark_zero;
	unsigned char mark_half;
	unsigned char mark_full;
	unsigned char mark_flag;
	unsigned char start_point;
	unsigned char end_point;
	unsigned char owner;
	unsigned short save_num;	
	float max_data;
	float min_data;
	unsigned char buf[MAX_SAVE_DATA_NUM]; //(7  -> 10  -> 13 -> ......->286 -> 289  )装载数据完成后就是按照时间先后顺序记录的数据
	float data_buf[MAX_SAVE_DATA_NUM];	
}S_Graph_Buf,*P_S_Graph_Buf;
extern S_Graph_Buf graph_buf;


typedef struct
{
	unsigned char point:7;
	unsigned char operat_point:7;
	unsigned char type;
	unsigned char save_num;
	float max_data;
	float min_data;
	PTRFUN f_change;
	float buf[MAX_SAVE_DATA_NUM];	
}S_Save_Data,*P_S_Save_Data;

extern S_Save_Data temp_data;
extern S_Save_Data voc_data;
extern S_Save_Data pm25_data;
extern S_Save_Data hum_data;
extern S_Save_Data co2_data;
extern S_Save_Data pm10_data;





extern const unsigned char Num[10][128]; 



extern const unsigned char gImage_16_10[][20];//数字  符号 中 号
extern const unsigned char gImage_16_38[][76];//汉字
extern const unsigned char gImage_8_16[][16];//图标
extern const unsigned char gImage_8_6[][6];//数字 符号  小 号
extern const unsigned char gImage_16_15[][30];

extern const unsigned char gImage_mem[4736];
extern const unsigned char gImage_graph[4736] ;

float min_in_n_buf(float* buf,unsigned short data_num);
float max_in_n_buf(float* buf,unsigned short data_num);

void set_g_image_buf(unsigned char set_data);
void draw_point(unsigned short x,unsigned short y, unsigned char mod);
void draw_line(short x1,short y1, short x2,short y2,unsigned char color);

void insert_image(unsigned short line_x,unsigned short pixel_y,unsigned short size_x,unsigned short size_y,unsigned char* image);
void draw_image(unsigned short pixel_X,unsigned short piexl_y,unsigned short size_x,unsigned short size_y,unsigned char* image,unsigned char draw_type);//任意位置画图 draw_type:0 反相draw_type:1 正常

void load_image_to_buf(unsigned char * image);
unsigned char display_num(unsigned char line_x,unsigned short pixel_y,float num,unsigned char font);//返回值为当前占用的屏幕长度font字体: 2 16*10  1:8*6
void display_temp(unsigned char line_x,unsigned short pixel_y,float temp,unsigned char font);
void display_humidity(unsigned char line_x,unsigned short pixel_y,float humidity);
void display_voc(unsigned char line_x,unsigned short pixel_y,unsigned short voc,unsigned char font);
void display_pm25(unsigned char line_x,unsigned short pixel_y,unsigned short pm25,unsigned char font);
void display_co2(unsigned char line_x,unsigned short pixel_y,unsigned short co2,unsigned char font);

void display_menu(void);
void refresh_menu(void);

void draw_graph(void);
void display_graph(void);
void clear_all(void);
void set_focus(unsigned char point);
void load_data_to_graph_buf(P_S_Save_Data data);
void push_data(P_S_Save_Data save_data,float data);






char common_change_function(float max,float min,float current);
char pm2_5_change_function(float max,float min,float current);
char voc_change_function(float max,float min,float current);
char pm10_change_function(float max,float min,float current);
char hum_change_function(float max,float min,float current);


void test_function(void);

#endif

