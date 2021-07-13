/*
 * networkTime.h
 *
 * Created: 2017-11-22 10:52:34
 *  Author: STELLA
 */ 


#ifndef NETWORKTIME_H_
#define NETWORKTIME_H_


#ifndef pdMS_TO_TICKS
#define pdMS_TO_TICKS( xTimeInMs ) ( ( portTickType ) ( ( ( portTickType ) ( xTimeInMs ) * ( portTickType ) configTICK_RATE_HZ ) / ( portTickType ) 1000 ) )
#endif





/* Useful Constants */
#define SECS_PER_MIN  (60UL)
#define SECS_PER_HOUR (3600UL)
#define SECS_PER_DAY  (SECS_PER_HOUR * 24UL)
#define DAYS_PER_WEEK (7UL)
#define SECS_PER_WEEK (SECS_PER_DAY * DAYS_PER_WEEK)
#define SECS_PER_YEAR (SECS_PER_WEEK * 52UL)
#define SECS_YR_2000  (946684800UL) // the time at the start of y2k

typedef struct  {
	uint8_t Second;
	uint8_t Minute;
	uint8_t Hour;
	uint8_t Wday;   // day of week, sunday is day 1
	uint8_t Day;
	uint8_t Month;
	uint8_t Year;   // offset from 1970;
} 	tmElements_t, TimeElements, *tmElementsPtr_t;

void vStartSoftTimer();
void breakTime(time_t timeInput);
time_t makeTime(tmElements_t *tm);
void setSystemTimeLong(uint32_t lTime);
uint32_t getTimeLong();

int hour() ;
int minute();
int tmsecond();
int year();
int month();
int day();



//void getTimeFromServer(void);



#endif /* NETWORKTIME_H_ */