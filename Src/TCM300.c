#include"TCM300.h"

extern recv_data_def data_from_stm300;
extern time_def time;
extern UART_HandleTypeDef huart2;
extern ID_def id;

extern S_Save_Learned_Device_Data Enocean_Switch;
extern S_Save_Learned_Device_Data Wireless_Body_Sensor; 
extern S_Save_Learned_Device_Data Wireless_Windows_Sensor;

//extern  air_stat_def air;
extern S_Air_Stat_Def air_fliter;

extern key_data_def rtos_key;
void read_tcm300_id(void)
{
	data_from_stm300.tx_data[0]= 0X55;
	data_from_stm300.tx_data[1]= 0X00;
	data_from_stm300.tx_data[2]= 0X01;
	data_from_stm300.tx_data[3]= 0X00;
	data_from_stm300.tx_data[4]= 0X05;
	data_from_stm300.tx_data[5]= 0X70;
	data_from_stm300.tx_data[6]= 0X03;
	data_from_stm300.tx_data[7]= 0X09;
	id.read_tcm300_id_flag = 1;
	//send_data_to_gateway_by_msc(0x68,0x88,"hello",5);
	send_data_to_tcm300(8);	
	HAL_Delay(100);
//	while(1)
		{
			if(data_from_stm300.recv_ok == 1)
				{
					data_from_stm300.recv_ok = 0;
					if(VerifyCRC8Sub(&data_from_stm300.data[5],(data_from_stm300.data[1]+data_from_stm300.data[2])) == 0XFF)
						{
							if(data_from_stm300.data[3] == 0X02)
								{
									id.TCM300_ID[0] = data_from_stm300.data[14];
									id.TCM300_ID[1] = data_from_stm300.data[15];
									id.TCM300_ID[2] = data_from_stm300.data[16];
									id.TCM300_ID[3] = data_from_stm300.data[17];
									if((id.TCM300_ID[0] == 0x00)&&(id.TCM300_ID[1] == 0x00)&&(id.TCM300_ID[2] == 0x00)&&(id.TCM300_ID[3] == 0x00))
										{
											id.read_tcm300_id_flag = 0;
											return;
										}
									else
										{
											id.read_tcm300_id_flag = 0;
											return;
										}
								}
						}
					id.read_tcm300_id_flag = 0;
					return;
				}
		}
	id.read_tcm300_id_flag = 0;
	//send_data_to_gateway_by_msc(0x68,0x88,"hello",5);
}


void send_data_to_tcm300(unsigned char len)
{	
	HAL_UART_Transmit(&huart2,data_from_stm300.tx_data,len,800);
}

void set_send_gateway_head_code(unsigned char data_len)
{
	data_from_stm300.tx_data[0] = 0X55;
	data_from_stm300.tx_data[1] = 0x00;
	data_from_stm300.tx_data[2] = 0X0E + data_len;
	data_from_stm300.tx_data[3] = 0X06;
	data_from_stm300.tx_data[4] = 0X01;
	SetCRC8Sub(&data_from_stm300.tx_data[1],4);	
}

void set_send_gateway_command_code(unsigned char manufacturer_id,unsigned char master_command,unsigned char command)
{
	data_from_stm300.tx_data[6] = 0XD1;
	data_from_stm300.tx_data[7] = manufacturer_id;	 ///
	data_from_stm300.tx_data[8] = master_command;	///
	data_from_stm300.tx_data[9] = command;
}


void set_send_gateway_ID_code(void)
{
	data_from_stm300.data[10] = 20; //////
	data_from_stm300.data[11] = id.GATE_WAY_ID[3];
	data_from_stm300.data[12] = id.GATE_WAY_ID[2];
	data_from_stm300.data[13] = id.GATE_WAY_ID[1];
	data_from_stm300.data[14] = id.GATE_WAY_ID[0];
}

void set_send_gateway_end_code(unsigned char data_len)
{
	unsigned char vlong;
	data_from_stm300.tx_data[data_len + 0X0F] = id.TCM300_ID[0];
	data_from_stm300.tx_data[data_len + 0X10] = id.TCM300_ID[1];
	data_from_stm300.tx_data[data_len + 0X11] = id.TCM300_ID[2];
	data_from_stm300.tx_data[data_len + 0X12] = id.TCM300_ID[3];
	data_from_stm300.tx_data[data_len + 0X13] = 0X00;
	data_from_stm300.tx_data[data_len + 0X14] = 0X03;
	
	data_from_stm300.tx_data[data_len + 0X15] = 0XFF;
	data_from_stm300.tx_data[data_len + 0X16] = 0XFF;
	data_from_stm300.tx_data[data_len + 0X17] = 0XFF;
	data_from_stm300.tx_data[data_len + 0X18] = 0XFF;
	data_from_stm300.tx_data[data_len + 0X19] = 0XFF;
	vlong = data_len+0X14;
	 SetCRC8Sub(&data_from_stm300.tx_data[0X06],vlong);
}




void save_gateway_ID(unsigned char * pgateway_ID)
{
	id.GATE_WAY_ID[3] = *pgateway_ID;
	pgateway_ID++;
	id.GATE_WAY_ID[2] = *pgateway_ID;
	pgateway_ID++;
	id.GATE_WAY_ID[1] = *pgateway_ID;
	pgateway_ID++;
	id.GATE_WAY_ID[0] = *pgateway_ID;
	
}

void task_deal_tcm300(void)
{
	data_from_stm300.recv_ok = 0;
	if(VerifyCRC8Sub(&data_from_stm300.data[5],(data_from_stm300.data[1]+data_from_stm300.data[2])) == 0XFF)
		{
			if(data_from_stm300.data[3] == 1)
				{
					switch(data_from_stm300.data[5])
						{
							case 0XF6:
						             Deal_RPS_RadioSub();
						             break;

						        case 0XA5:
						             Deal_4BS_RadioSub();
						             break;

						        case 0XC5:
						             //Deal_SYSEX_RadioSub();
						             break;

						        case 0XD1:
	                       					Deal_Gateway_RadioSub();
						             break;

	                	                        default:
	                	                             break;
						}
				}
			if(data_from_stm300.data[3] == 7)
				{
					data_from_stm300.vBigRxMax =data_from_stm300.data[1]-4;
					DealBigdata();
				}
			
		}
}

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




void Deal_RPS_RadioSub(void)
{
	unsigned char command;
	command = data_from_stm300.data[6];
	id.TARGAT_ID[0] = data_from_stm300.data[7];
	id.TARGAT_ID[1] = data_from_stm300.data[8];
	id.TARGAT_ID[2] = data_from_stm300.data[9];
	id.TARGAT_ID[3] = data_from_stm300.data[10];
	if((command == cOperateCode_A)||(command == cOperateCode_B)||(command == cOperateCode_C)||(command == cOperateCode_D))
		{
			if(find_tcm_ID(&Enocean_Switch))
				{
					Enocean_Switch.device[Enocean_Switch.find_point].this_device_effect = EFFECT;
					Enocean_Switch.G_last_time_recv_data = 0;
					Enocean_Switch.device[Enocean_Switch.find_point].last_time_recv_data = 0;
					Enocean_Switch.device[Enocean_Switch.find_point].count_trigger_on_time = 0;
					Enocean_Switch.device[Enocean_Switch.find_point].trigger_on = 1;//计时count_trigger_on_time
					
					if((rtos_key.key_ok[7] == 0)||(rtos_key.key_ok[6] == 0)|(rtos_key.key_ok[5] == 0))
					{
						spk_echo(2,200);
						return;
					}
					//按下开关就要清除传感器恢复之前状态标志位					
					
					switch(command)
						{
							case cOperateCode_A:
								air_fliter.current_switch_stat.ctrl_speed = FAN_AUTO;
								air_fliter.current_switch_stat.set_priority_delay_time = Enocean_Switch.device[Enocean_Switch.find_point].set_trigger_on_delay_time;
								air_fliter.current_switch_stat.cur_ctrl_device = ((unsigned char) ENOCEAN_SWITCH << 4) + Enocean_Switch.find_point;
								air_fliter.current_switch_stat.enable_turn_on = 1;
								air_fliter.current_switch_stat.switch_ctrl_power = !air_fliter.current_switch_stat.switch_ctrl_power;
								if(air_fliter.current_switch_stat.switch_ctrl_power)
									{
										//air_fliter.current_fan_stat.current_fan_speed = FAN_MID;
										air_fliter.current_fan_stat.auto_ctrl_protect_time = 0;
									}
								break;
								
							case cOperateCode_B:								
								air_fliter.current_switch_stat.ctrl_speed = FAN_LOW;
								air_fliter.current_switch_stat.set_priority_delay_time = Enocean_Switch.device[Enocean_Switch.find_point].set_trigger_on_delay_time;
								air_fliter.current_switch_stat.cur_ctrl_device = ((unsigned char) ENOCEAN_SWITCH << 4) + Enocean_Switch.find_point;
								air_fliter.current_switch_stat.enable_turn_on = 1;
								air_fliter.current_switch_stat.switch_ctrl_power = 1;
								
								break;
								
							case cOperateCode_C:								
								air_fliter.current_switch_stat.ctrl_speed = FAN_MID;
								air_fliter.current_switch_stat.set_priority_delay_time = Enocean_Switch.device[Enocean_Switch.find_point].set_trigger_on_delay_time;
								air_fliter.current_switch_stat.cur_ctrl_device = ((unsigned char) ENOCEAN_SWITCH << 4) + Enocean_Switch.find_point;
								air_fliter.current_switch_stat.enable_turn_on = 1;
								air_fliter.current_switch_stat.switch_ctrl_power = 1;

								break;
								
							case cOperateCode_D:
								air_fliter.current_switch_stat.ctrl_speed = FAN_HIGH;
								air_fliter.current_switch_stat.set_priority_delay_time = Enocean_Switch.device[Enocean_Switch.find_point].set_trigger_on_delay_time;
								air_fliter.current_switch_stat.cur_ctrl_device = ((unsigned char) ENOCEAN_SWITCH << 4) + Enocean_Switch.find_point;
								air_fliter.current_switch_stat.enable_turn_on = 1;
								air_fliter.current_switch_stat.switch_ctrl_power = 1;								
								break;
							
						}
					air_fliter.current_fan_stat.warn_key_pressed = 0;
					switch_wifi_tirg_pir();
				}
		}
}

void Deal_4BS_RadioSub(void)
{
	id.TARGAT_DATA[3] = data_from_stm300.data[6];
	id.TARGAT_DATA[2] = data_from_stm300.data[7];
	id.TARGAT_DATA[1] = data_from_stm300.data[8]; ////FF KAI   00 GUAN
	id.TARGAT_DATA[0] = data_from_stm300.data[9];
	id.TARGAT_ID[0] =  data_from_stm300.data[10];
	id.TARGAT_ID[1] =  data_from_stm300.data[11];
	id.TARGAT_ID[2] =  data_from_stm300.data[12];
	id.TARGAT_ID[3] =  data_from_stm300.data[13];
	
	if(find_tcm_ID(& Wireless_Windows_Sensor) == 1)
		{
			deal_windows_sub();
		}

	if(find_tcm_ID(& Wireless_Body_Sensor) == 1)
		{
			deal_pir_sub();
		}
}

