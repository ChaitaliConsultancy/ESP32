#ifndef DATE_TIME_H
#define DATE_TIME_H

#define SAMPLE_TIME     15

extern struct dateTime dateTime_sample;

extern const long gmtOffset_sec;
extern const char* ntpServer;

typedef enum{
  DATE_TIME_24,
  DATE_TIME_12
}DATE_TIME_FORMAT;

typedef enum{
  INTERNAL_RTC,
  NTP_SERVER
}DATE_TIME_CONTROL;

struct dateTime {
   int tm_sec;              /* seconds,  range 0 to 59          */
   int tm_min;              /* minutes, range 0 to 59           */
   int tm_hour;             /* hours, range 0 to 23             */
   int tm_mday;             /* day of the month, range 1 to 31  */
   int tm_mon;              /* month, range 0 to 11             */
   int tm_year;             /* The number of years since 1900   */
   unsigned long tm_epoch; 
};

struct Date {
    int d; 
    int m;
    int y;
};

const int monthDays[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

void configureTime(bool format, long offSet, const char* server);
int getCurrentDateTime(int type, struct dateTime *dateTimeNow);
bool getDateTime_SD_FSM_Sate();
int getDifference(Date dt1, Date dt2);

#endif //#ifdef DATE_TIME_H
