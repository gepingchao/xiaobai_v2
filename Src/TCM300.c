#include"TCM300.h"

//tcm300_recv_data
//machine_info

recv_data_def data_from_stm300;
extern UART_HandleTypeDef huart2;


void read_tcm300_id(void)
{
	tcm300_recv_data.tx_data[0]= 0X55;
	tcm300_recv_data.tx_data[1]= 0X00;
	tcm300_recv_data.tx_data[2]= 0X01;
	tcm300_recv_data.tx_data[3]= 0X00;
	tcm300_recv_data.tx_data[4]= 0X05;
	tcm300_recv_data.tx_data[5]= 0X70;
	tcm300_recv_data.tx_data[6]= 0X03;
	tcm300_recv_data.tx_data[7]= 0X09;
	//send_data_to_gateway_by_msc(0x68,0x88,"hello",5);
	send_data_to_tcm300(8);	
	HAL_Delay(100);
		{
			if(1)//tcm300_recv_data.recv_ok == 1)
				{
					tcm300_recv_data.recv_ok = 0;
					if(VerifyCRC8Sub(&tcm300_recv_data.rx_data[6],(tcm300_recv_data.rx_data[2]+tcm300_recv_data.rx_data[3])) == 0XFF)
						{
							if(tcm300_recv_data.rx_data[4] == 0X02)
								{
									machine_info.tcm300id[0] = tcm300_recv_data.rx_data[15];
									machine_info.tcm300id[1] = tcm300_recv_data.rx_data[16];
									machine_info.tcm300id[2] = tcm300_recv_data.rx_data[17];
									machine_info.tcm300id[3] = tcm300_recv_data.rx_data[18];
								}
						}
					return;
				}
		}
}


void send_data_to_tcm300(unsigned char len)
{	
	HAL_UART_Transmit(&TCM300_COM,tcm300_recv_data.tx_data,len,800);
}

void set_send_gateway_head_code(unsigned char data_len)
{
	tcm300_recv_data.tx_data[0] = 0X55;
	tcm300_recv_data.tx_data[1] = 0x00;
	tcm300_recv_data.tx_data[2] = 0X0E + data_len;
	tcm300_recv_data.tx_data[3] = 0X06;
	tcm300_recv_data.tx_data[4] = 0X01;
	SetCRC8Sub(&tcm300_recv_data.tx_data[1],4);	
}

void set_send_gateway_command_code(unsigned char manufacturer_id,unsigned char master_command,unsigned char command)
{
	tcm300_recv_data.tx_data[6] = 0XD1;
	tcm300_recv_data.tx_data[7] = manufacturer_id;	 ///
	tcm300_recv_data.tx_data[8] = master_command;	///
	tcm300_recv_data.tx_data[9] = command;
}



void set_send_gateway_end_code(unsigned char data_len)
{
	unsigned char vlong;
	//tcm300_recv_data.tx_data[data_len + 0X0F] = id.TCM300_ID[0];
	//tcm300_recv_data.tx_data[data_len + 0X10] = id.TCM300_ID[1];
	//tcm300_recv_data.tx_data[data_len + 0X11] = id.TCM300_ID[2];
	//tcm300_recv_data.tx_data[data_len + 0X12] = id.TCM300_ID[3];
	tcm300_recv_data.tx_data[data_len + 0X13] = 0X00;
	tcm300_recv_data.tx_data[data_len + 0X14] = 0X03;
	
	tcm300_recv_data.tx_data[data_len + 0X15] = 0XFF;
	tcm300_recv_data.tx_data[data_len + 0X16] = 0XFF;
	tcm300_recv_data.tx_data[data_len + 0X17] = 0XFF;
	tcm300_recv_data.tx_data[data_len + 0X18] = 0XFF;
	tcm300_recv_data.tx_data[data_len + 0X19] = 0XFF;
	vlong = data_len+0X14;
	 SetCRC8Sub(&tcm300_recv_data.tx_data[0X06],vlong);
}




void save_gateway_ID(unsigned char * pgateway_ID)
{
	/*id.GATE_WAY_ID[3] = *pgateway_ID;
	pgateway_ID++;
	id.GATE_WAY_ID[2] = *pgateway_ID;
	pgateway_ID++;
	id.GATE_WAY_ID[1] = *pgateway_ID;
	pgateway_ID++;
	id.GATE_WAY_ID[0] = *pgateway_ID;*/
	
}

void task_deal_tcm300(void)
{
	tcm300_recv_data.recv_ok = 0;
	if(VerifyCRC8Sub(&tcm300_recv_data.rx_data[6],(tcm300_recv_data.rx_data[2]+tcm300_recv_data.rx_data[3])) == 0XFF)
		{
			if(tcm300_recv_data.rx_data[4] == 1)
				{
					switch(tcm300_recv_data.rx_data[6])
						{
							case 0XF6:
						             Deal_RPS_RadioSub();
						             break;

						        case 0XA5:
						             //Deal_4BS_RadioSub();
						             break;

						        case 0XC5:
						             //Deal_SYSEX_RadioSub();
						             break;

						        case 0XD1:
	                       					//Deal_Gateway_RadioSub();
						             break;

	                	                        default:
	                	                             break;
						}
				}
			if(tcm300_recv_data.rx_data[3] == 7)
				{
				}
			
		}
}

/*
unsigned char find_tcm_ID(P_S_Save_Learned_Device_Data P_DATA )
{
	unsigned loop8,loop4;
	for(loop8 = 0;loop8<8;loop8++)
		{
			for(loop4 = 0;loop4<4;loop4++)
				{
					if(P_DATA->device[loop8].tcm300_ID[loop4] == id.TARGAT_ID[loop4])
						{
							if(loop4 == 3)
								{
									if(P_DATA->device[loop8].tcm300_ID[4] == 0X55) //YOUXIAO
										{
											P_DATA->find_point = loop8;
											return 1;
										}
								}
							continue;
						}
					else
						{
							break;
						}
				}
		}
	return 0;
}

unsigned char find_wifi_send_tcm_ID(P_S_Save_Learned_Device_Data P_DATA )
{
	unsigned loop8,loop4;
	for(loop8 = 0;loop8<8;loop8++)
		{
			for(loop4 = 0;loop4<4;loop4++)
				{
					if(P_DATA->device[loop8].tcm300_ID[loop4] == id.SET_LEARN_CLEAR_ID[loop4])
						{
							if(loop4 == 3)
								{
									if(P_DATA->device[loop8].tcm300_ID[4] == 0X55) //YOUXIAO
										{
											P_DATA->find_point = loop8;
											return 1;
										}
								}
							continue;
						}
					else
						{
							break;
						}
				}
		}
	return 0;
}

*/


void Deal_RPS_RadioSub(void)
{
	unsigned char command;
	command = tcm300_recv_data.rx_data[7];
	//id.TARGAT_ID[0] = tcm300_recv_data.rx_data[7];
	//id.TARGAT_ID[1] = tcm300_recv_data.rx_data[8];
	//id.TARGAT_ID[2] = tcm300_recv_data.rx_data[9];
	//id.TARGAT_ID[3] = tcm300_recv_data.rx_data[10];
	if((command == cOperateCode_A)||(command == cOperateCode_B)||(command == cOperateCode_C)||(command == cOperateCode_D))
		{			
			//按下开关就要清除传感器恢复之前状态标志位					
			
			switch(command)
				{
					case cOperateCode_A:
						if(graph_buf.menu_choose)
							{
								set_cursor(1);
							}
						break;
						
					case cOperateCode_B:								
						if(graph_buf.menu_choose)
							{
								set_cursor(0);
							}
						break;
						
					case cOperateCode_C:	
						graph_buf.menu_choose = !graph_buf.menu_choose;
						graph_buf.nop_refresh = 1;
						if(graph_buf.menu_choose)
							{
								clear_all();
								display_graph();	
							}
						else
							{
								clear_all();
								refresh_menu(1);
							}

						break;
						
					case cOperateCode_D:
						break;
					
				}
		}
}




