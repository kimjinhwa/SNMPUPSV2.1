/*
 * networkTime.c
 *
 * Created: 2017-11-22 10:44:05
 *  Author: STELLA
 */ 



#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sys.h"
#include "networkTime.h"


#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#include "partest.h"

#include "usart.h"

#include "networkTime.h"
#include "print_funcs.h"

#define watchSTACK_SIZE			configMINIMAL_STACK_SIZE

#include "sockets.h"

/* ethernet includes */
#include "ethernet.h"
#include "wdt.h"
#include "rtc.h"
#include "FlashMemory.h"
#include "snmp.h"
#define mainAUTO_RELOAD_TIMER_PERIOD	pdMS_TO_TICKS(10)

extern u32_t sysuptime_from_SetTime ;

static void prvAutoReloadTimerCallBack(xTimerHandle xTimer) ;
portTickType systemTime=0;
portTickType encresedTime=0;
// 10분(600초) -> 
// 25%가 빠르다.. 1250으로 설정을 해줘야 한다.
// 10분이면 600*1.25 750이 될것이다.
//
static portTASK_FUNCTION( vWatchdog, pvParameters );
bool isReboot=false;
extern wdt_opt_t opt;
extern uint16_t modebusPrcessCount;
extern int isNowWebServiceRunningCount;
extern uint16_t  isNowSNMPServiceRunning ;


static portTASK_FUNCTION( vWatchdog, pvParameters )
{
	uint16_t isModbusAlive;
	int beforeNowWebServiceRunningCount=0;
	uint16_t beforeNowSNMPServiceRunning=0;
	( void ) pvParameters;
	wdt_enable(&opt);
	vTaskDelay( 3000);
	for(;;)
	{
		if(isReboot==false)
		{
			isModbusAlive = modebusPrcessCount;
			beforeNowWebServiceRunningCount = isNowWebServiceRunningCount;

			if(isNowSNMPServiceRunning > 0 )
			beforeNowSNMPServiceRunning = isNowSNMPServiceRunning ;
			//else beforeNowSNMPServiceRunning = false;

			wdt_clear();
			vParTestSetLED(2, pdTRUE); vTaskDelay( 1000);
			vParTestSetLED(2, pdFALSE); vTaskDelay( 1000);
			wdt_clear();
			vParTestSetLED(2, pdTRUE); vTaskDelay( 1000);
			vParTestSetLED(2, pdFALSE); vTaskDelay( 1000);
			wdt_clear();
			wdt_clear();
			vParTestSetLED(2, pdTRUE); vTaskDelay( 1000);
			vParTestSetLED(2, pdFALSE); vTaskDelay( 1000);
			wdt_clear();
			wdt_clear();
			vParTestSetLED(2, pdTRUE); vTaskDelay( 1000);
			vParTestSetLED(2, pdFALSE); vTaskDelay( 1000);
			wdt_clear();
			wdt_clear();
			vParTestSetLED(2, pdTRUE); vTaskDelay( 1000);
			vParTestSetLED(2, pdFALSE); vTaskDelay( 1000);
			wdt_clear();

			//시리얼통신이 죽을 일은 없다.
			//if(isModbusAlive == modebusPrcessCount) isReboot = true;

			//웹서비스가 실행 중이라면 이제는 반드시 끝이 났어야 한다
			if(beforeNowWebServiceRunningCount == isNowWebServiceRunningCount) isReboot = true;
			else isNowWebServiceRunningCount=0;
			/*
			if( beforeNowSNMPServiceRunning == true) {  // snmp는 시간을 추가하여 준다.
				wdt_clear();
				vParTestSetLED(2, pdTRUE); vTaskDelay( 1000);
				if(isNowSNMPServiceRunning==true)break;
				vParTestSetLED(2, pdFALSE); vTaskDelay( 1000);
				if(isNowSNMPServiceRunning==true)break;
				wdt_clear();
				vParTestSetLED(2, pdTRUE); vTaskDelay( 1000);
				if(isNowSNMPServiceRunning==true)break;
				vParTestSetLED(2, pdFALSE); vTaskDelay( 1000);
				wdt_clear();
				vParTestSetLED(2, pdTRUE); vTaskDelay( 1000);
				if(isNowSNMPServiceRunning==true)break;
				vParTestSetLED(2, pdFALSE); vTaskDelay( 1000);
				if(isNowSNMPServiceRunning==true)isReboot = true;  // 문제가 생긴 것이다.
			}
			*/
		}
		else{
			vParTestSetLED(1, pdTRUE); vParTestSetLED(2, pdTRUE);
			vParTestSetLED(3, pdTRUE); vParTestSetLED(0, pdTRUE);
			while(1);
		}
	}
}

void vStartSoftTimer( unsigned portBASE_TYPE xPriority )
{
	xTimerHandle	xAutoReloadTimer;
	portBASE_TYPE	xTimer1Started;
	
	//systemTime = get_last_log_time() ;
	xAutoReloadTimer = xTimerCreate(
		(const signed char *)"AutoReload",
		mainAUTO_RELOAD_TIMER_PERIOD,
		pdTRUE,
		(void * )0,
		prvAutoReloadTimerCallBack
	);
	if(xAutoReloadTimer != NULL )
	{
		xTimer1Started = xTimerStart(xAutoReloadTimer,0);
	}
	//signed portBASE_TYPE xLEDTask;

	xTaskCreate( vWatchdog, ( signed char * ) "watchdog", watchSTACK_SIZE, NULL, xPriority , ( xTaskHandle * ) NULL );
}

static void prvAutoReloadTimerCallBack(xTimerHandle xTimer)
{
	snmp_inc_sysuptime();
}




// leap year calulator expects year argument as years offset from 1970
#define LEAP_YEAR(Y)     ( ((1970+Y)>0) && !((1970+Y)%4) && ( ((1970+Y)%100) || !((1970+Y)%400) ) )
//convenience macros to convert to and from tm years
#define  tmYearToCalendar(Y) ((Y) + 1970)  // full four digit year
#define  CalendarYrToTm(Y)   ((Y) - 1970)
#define  tmYearToY2k(Y)      ((Y) - 30)    // offset is from 2000
#define  y2kYearToTm(Y)      ((Y) + 30)

static  const uint8_t monthDays[]={31,28,31,30,31,30,31,31,30,31,30,31}; // API starts months from 1, this array starts from 0

static tmElements_t tm;          // a cache of time elements

tmElements_t *tm_ptr ;

//void breakTime(time_t timeInput, tmElements_t &tm) ;


time_t makeTime(tmElements_t *tmr){   

	 int i;
	 uint32_t seconds;
	seconds= tmr->Year*(SECS_PER_DAY * 365);
	for (i = 0; i < tmr->Year; i++) {
		if (LEAP_YEAR(i)) {
			seconds +=  SECS_PER_DAY;   // add extra days for leap years
		}
	}	 

	// add days for this year, months start from 1
	for (i = 1; i < tmr->Month; i++) {
		if ( (i == 2) && LEAP_YEAR(tm.Year)) {
			seconds += SECS_PER_DAY * 29;
			} else {
			seconds += SECS_PER_DAY * monthDays[i-1];  //monthDay array starts from 0
		}
	}
	seconds+= (tmr->Day-1) * SECS_PER_DAY;
	seconds+= tmr->Hour * SECS_PER_HOUR;
	seconds+= tmr->Minute * SECS_PER_MIN;
	seconds+= tmr->Second;
	return (time_t)seconds;	
}

void setSystemTimeLong(uint32_t lTime)
{
	systemTime = lTime;
	sysuptime_from_SetTime = 0;
}
uint32_t getTimeLong()
{
	//encresedTime = (rtc_get_value(&AVR32_RTC)-systemTime) *0.56987826086956521;
	//return (uint32_t)systemTime + (rtc_get_value(&AVR32_RTC)-systemTime) *0.56987826086956521;
	//return (uint32_t)systemTime + (xTaskGetTickCount()*0.56987826086956521)/1000;
	
	//return (uint32_t)systemTime + xTaskGetTickCount()/1250;
	return (uint32_t)systemTime +sysuptime_from_SetTime/125.0;
}
void breakTime(time_t timeInput){

	uint8_t year;
	uint8_t month, monthLength;
	uint32_t time;
	unsigned long days;
	//tm_ptr = &tm;
	time = (uint32_t)timeInput  ;//+ (rtc_get_value(&AVR32_RTC)-systemTime) *0.56987826086956521;
	tm.Second = time % 60;
	time /= 60; // now it is minutes
	tm.Minute = time % 60;
	time /= 60; // now it is hours
	tm.Hour = time % 24;
	time /= 24; // now it is days
	tm.Wday = ((time + 4) % 7) + 1;  // Sunday is day 1
	year = 0;
	days = 0;
	while((unsigned)(days += (LEAP_YEAR(year) ? 366 : 365)) <= time) {
		year++;
	}
	tm.Year = year; // year is offset from 1970
	days -= LEAP_YEAR(year) ? 366 : 365;
	time  -= days; // now it is days in this year, starting at 0
	
	days=0;
	month=0;
	monthLength=0;
	for (month=0; month<12; month++) {
		if (month==1) { // february
			if (LEAP_YEAR(year)) {
				monthLength=29;
				} else {
				monthLength=28;
			}
			} else {
			monthLength = monthDays[month];
		}
		
		if (time >= monthLength) {
			time -= monthLength;
			} else {
			break;
		}
	}
	tm.Month = month + 1;  // jan is month 1
	tm.Day = time + 1;     // day of month
};

int hour() 
{ 
	breakTime(systemTime);
	return tm.Hour ;
}
int minute()
{
	breakTime(systemTime);
	return tm.Minute;
}
int tmsecond()
{
	breakTime(systemTime);
	return tm.Second;
}
int year()
{
	breakTime(systemTime);
	return tm.Year + 1900;
}
int month()
{
	breakTime(systemTime);
	return tm.Month;
}
int day()
{
	breakTime(systemTime);
	return tm.Day;
}


