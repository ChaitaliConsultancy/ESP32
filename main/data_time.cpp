#include <WiFi.h>
#include <ESP32Time.h>
#include "time.h"
#include "sd_card.h"
#include "date_time.h"

// test_code to generate day pass condition
int minut = 0;

ESP32Time rtc;
const long  gmtOffset_sec = 19800;
const char* ntpServer = "pool.ntp.org";

struct dateTime dateTime_sample;

void configureTime(bool format, long offSet, const char* server)
{
  if(format == DATE_TIME_24)
  {
    configTime(offSet, 0, server);  // set 24Hr date time format
  }
  else
  {
    long newOffset = offSet/2;
    configTime(newOffset, newOffset, server); // set 12Hr date time format
  }
}

int getCurrentDateTime(int type, struct dateTime *dateTimeNow) 
{
  time_t now;
  char buf[10];
  struct tm timeinfo;
  if(type == INTERNAL_RTC)
  {
    timeinfo = rtc.getTimeStruct();     // Date Time from interal RTC
  }
  else
  {
    if (!getLocalTime(&timeinfo))     // Date Time from NTP server
    {
      Serial.println("getDateTime : Failed to obtain time");
      return(0);
    }
  }
    
  memset(&buf, 0, sizeof(buf));
  strftime(buf, sizeof(buf), "%Y", &timeinfo);
  dateTimeNow->tm_year = atoi(buf);
  
  memset(&buf, 0, sizeof(buf));
  strftime(buf, sizeof(buf), "%d", &timeinfo);
  dateTimeNow->tm_mon = atoi(buf);
  
  memset(&buf, 0, sizeof(buf));
  strftime(buf, sizeof(buf), "%x", &timeinfo);
  dateTimeNow->tm_mday = atoi(buf);
  
  memset(&buf, 0, sizeof(buf));
  strftime(buf, sizeof(buf), "%H", &timeinfo);
  dateTimeNow->tm_hour = atoi(buf);
  
  
  memset(&buf, 0, sizeof(buf));
  strftime(buf, sizeof(buf), "%M", &timeinfo);
  dateTimeNow->tm_min = atoi(buf);
   
  memset(&buf, 0, sizeof(buf));
  strftime(buf, sizeof(buf), "%S", &timeinfo);
  dateTimeNow->tm_sec = atoi(buf);

  time(&now);
  dateTimeNow->tm_epoch = now;
  // Serial.println("getCurrentDateTime : ");
  // Serial.print(dateTimeNow->tm_year);Serial.print(":"); Serial.print(dateTimeNow->tm_mon);Serial.print(":"); 
  // Serial.print(dateTimeNow->tm_mday);Serial.print(":"); Serial.print(dateTimeNow->tm_hour);Serial.print(":");
  // Serial.print(dateTimeNow->tm_min); Serial.print(":"); Serial.println(dateTimeNow->tm_sec);
 
  if(type == NTP_SERVER)   // update internal RTC date time
  {
    minut = dateTimeNow->tm_min;
    // test_code to generate day pass condition
    if(minut == 59){minut = 0;}else{ minut++;}
    rtc.setTimeStruct(timeinfo);  
  }
  return 1;
}


bool getDateTime_SD_FSM_Sate()
{
  struct dateTime dateTime_now;
  // Serial.println("getDateTime_SD_FSM_Sate : ");
  if(!getCurrentDateTime(INTERNAL_RTC, &dateTime_now))
  {
    Serial.println("\t failed to get date-time");
    return false;
  }
// test_code to generate day pass condition
// if((dateTime_now.tm_hour == 0) && (dateTime_now.tm_min == 0) && (dateTime_now.tm_sec == 0))
  if(dateTime_now.tm_min == minut)
  {
    Serial.println("getDateTime_SD_FSM_Sate : ");
    Serial.println("\t its a new day, create file now"); 
    dateTime_sample = dateTime_now;
    SD_card_create_file();
    // test_code to generate day pass condition
    // Serial.print(dateTime_now.tm_min); Serial.print(" : "); Serial.println(minut);
    if(minut == 59){minut = 0;}else{ minut++;}
    return true;
  }
  if((dateTime_sample.tm_epoch + SAMPLE_TIME) <= dateTime_now.tm_epoch)
  {
    Serial.println("getDateTime_SD_FSM_Sate : ");
    Serial.println("\t its a data sampling time");
    Serial.println("\t collect data");
    dateTime_sample = dateTime_now;
    return true;
  }
  return false;
}

int countLeapYears(Date d)
{
    int years = d.y;
    if (d.m <= 2)
    {
      years--;
    }
    
    return years / 4
           - years / 100
           + years / 400;
}

int getDifference(Date dt1, Date dt2)
{
    long int n1 = dt1.y * 365 + dt1.d;
 
    for (int i = 0; i < dt1.m - 1; i++)
    {
      n1 += monthDays[i];
    }
    
    n1 += countLeapYears(dt1);
 
    long int n2 = dt2.y * 365 + dt2.d;
    for (int i = 0; i < dt2.m - 1; i++)
    {
      n2 += monthDays[i];
    }
    n2 += countLeapYears(dt2);
    return (n2 - n1);
}
