#include "rtc_time.h"
const unsigned char DayOfMon[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

STNP_Header stnp_tel;
DATE_TIME time_info;

void get_stnp_time(void)
{
	if(0 == time_info.is_time_checked)
		{
			RTC_TimeTypeDef set_time;
			unsigned int recv_time = 0;
			stnp_tel.LiVnMode = 0X0B;
			HAL_UART_Transmit(&WIFI_COM, (unsigned char*)&stnp_tel, sizeof(STNP_Header), 300);
			osDelay(500);
			mem_copy((char*)(wifi_recv_data.rx_data),(char*)(&stnp_tel),sizeof(STNP_Header));
			if(stnp_tel.TranTimeInt == 0)
				{
					return;
				}
			recv_time += wifi_recv_data.rx_data[40];
			recv_time <<= 8;
			recv_time += wifi_recv_data.rx_data[41];
			recv_time <<= 8;
			recv_time += wifi_recv_data.rx_data[42];
			recv_time <<= 8;
			recv_time += wifi_recv_data.rx_data[43];
			recv_time  -= TIME_1990;
		  	GetDateTimeFromSecond(recv_time,&time_info);
			if((0 == time_info.is_time_checked)&&(time_info.iYear))
				{
					time_info.is_time_checked = 1;
					set_time.Hours = time_info.iHour;
					set_time.Minutes = time_info.iMin;
					set_time.Seconds = time_info.iSec;
					HAL_RTC_SetTime(&hrtc,&set_time,RTC_FORMAT_BIN);
				}
		}
}
unsigned long GetSecondTime(DATE_TIME *date_time)
{
  unsigned short iYear, iMon, iDay, iHour, iMin, iSec;
  iYear = date_time->iYear;
  iMon = date_time->iMon;
  iDay = date_time->iDay;
  iHour = date_time->iHour;
  iMin = date_time->iMin;
  iSec = date_time->iSec;
 
  unsigned short i, Cyear=0;
  unsigned long CountDay=0;
 
  for(i=1970; i<iYear; i++) 
  {
    if(((i%4==0) && (i%100!=0)) || (i%400==0)) Cyear++;
  }
  CountDay = Cyear * 366 + (iYear-1970-Cyear) * 365;
  for(i=1; i<iMon; i++)
  {
    if((i==2) && (((iYear%4==0)&&(iYear%100!=0)) || (iYear%400==0)))
      CountDay += 29;
    else
      CountDay += DayOfMon[i-1];
  }
  CountDay += (iDay-1);
 
  CountDay = CountDay*SECOND_OF_DAY + (unsigned long)iHour*3600 + (unsigned long)iMin*60 + iSec;
  return CountDay;
}


void GetDateTimeFromSecond(unsigned long lSec, DATE_TIME *tTime)
{
  unsigned short i,j,iDay;
  unsigned long lDay;
 
  lDay = lSec / SECOND_OF_DAY;  
  lSec = lSec % SECOND_OF_DAY;
 
  i = 1970;
  while(lDay > 365)
  {
   if(((i%4==0)&&(i%100!=0)) || (i%400==0))  
    lDay -= 366;
   else
    lDay -= 365;
   i++;
  }
  if((lDay == 365) && !(((i%4==0)&&(i%100!=0)) || (i%400==0))) 
  {
   lDay -= 365;
   i++;
  }
  tTime->iYear = i;   
  for(j=0;j<12;j++)   
  {
   if((j==1) && (((i%4==0)&&(i%100!=0)) || (i%400==0)))
    iDay = 29;
   else
    iDay = DayOfMon[j];
   if(lDay >= iDay) lDay -= iDay;
   else break;
  }
  tTime->iMon = j+1;
  tTime->iDay = lDay+1;
  tTime->iHour = ((lSec / 3600)+8)%24;//这里注意，世界时间已经加上北京时间差8，
  tTime->iMin = (lSec % 3600) / 60;
  tTime->iSec = (lSec % 3600) % 60;
}

