#ifndef __TCM300_H__
#define __TCM300_H__
#include "include.h"

#define cOperateCode_A   0X50
#define cOperateCode_C   0X70

#define cOperateCode_B   0X10
#define cOperateCode_D   0X30

#define HIGHT_POR_SWITCH_TIME 1


 
void read_tcm300_id(void);
void task_deal_tcm300(void);
void send_data_to_tcm300(unsigned char len);
void set_send_gateway_head_code(unsigned char data_len);
void set_send_gateway_command_code(unsigned char manufacturer_id,unsigned char master_command,unsigned char command);
void set_send_gateway_end_code(unsigned char data_len);
void save_gateway_ID(unsigned char * pgateway_ID);

void Deal_RPS_RadioSub(void);
void Deal_4BS_RadioSub(void);

unsigned char find_tcm_ID(P_S_Save_Learned_Device_Data P_DATA );

unsigned char find_wifi_send_tcm_ID(P_S_Save_Learned_Device_Data P_DATA );

#endif

