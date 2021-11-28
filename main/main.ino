#include <WiFi.h>
#include "wifi.h"
#include "LEDs.h"
#include "sd_card.h"
#include "sensors.h"
#include "date_time.h"

TaskHandle_t Task1;
char wifiConfigFile[50]   = "/wifi_config.ini";

void mqttLoop( void * parameter) 
{
  for(;;) 
  {
    mqttClientLoop();  // looping the incomming & out goinng messages over MQTT broker.
    delay(500);
  }
}

void setup() 
{
  // init serial terminal for debugging
  Serial.begin(115200); 

  // init sd card
  if(!SD_card_Init())  
  {
    exit(0);
  }

   // init wifi connection
  if(!wifiSetup(wifiConfigFile))
  {
    exit(0);
  }

  xTaskCreatePinnedToCore(
      mqttLoop, /* Function to implement the task */
      "Task1",  /* Name of the task */
      10000,    /* Stack size in words */
      NULL,     /* Task input parameter */
      0,        /* Priority of the task */
      &Task1,   /* Task handle. */
      0);       /* Core where the task should run */

  // init sensors
  sensors_init(); 

  //cofigure ntp server
  configureTime(DATE_TIME_24, gmtOffset_sec, ntpServer);
  
  //get local date time
  getCurrentDateTime(NTP_SERVER, &dateTime_sample);
  
  //create initial file
   SD_card_create_file();

   leds_init();
}



void loop() 
{
  bool data_sample = getDateTime_SD_FSM_Sate(); // get the data sampling time status
  if(data_sample)
  {
    // update date time
    message.tm_sec = dateTime_sample.tm_sec;  
    message.tm_min = dateTime_sample.tm_min;
    message.tm_hour = dateTime_sample.tm_hour;
    message.tm_mday = dateTime_sample.tm_mday;
    message.tm_mon = dateTime_sample.tm_mon;
    message.tm_year = dateTime_sample.tm_year;

    // update ambian value
    message.ambiant = LUX_BH1750_Read();

    // update PIR values
    message.occ1 = digitalRead(MOTION_SENSOR_IP_1);
    message.occ2 = digitalRead(MOTION_SENSOR_IP_2);
    message.occ3 = digitalRead(MOTION_SENSOR_IP_3);

    // update Error status
    message.err = 0x00;

    // arrange data in csv file format 
    char messageString[256];
    sprintf(messageString, "%02d, %02d, %04d, %02d, %02d, %02d, %04d, %d, %d, %d, %03d, %03d, %03d, %08d, %08d, %08d, %08d, %08d, %08d, %08d, %08d, %08d, %08d, %02d",
            message.tm_mday, message.tm_mon, message.tm_year, message.tm_hour, message.tm_min, message.tm_sec,
            message.ambiant, message.occ1, message.occ2, message.occ3, message.r_pwm, message.g_pwm, message.b_pwm,
            message.p1, message.p2, message.p3, message.p4, message.p5, message.p6, message.p7, message.p8, message.p9, message.p10, message.err);

    bool internetConnected = publishDataToMQTTClient(messageString);
    if(internetConnected)
    {
      Serial.print("\t\t message sent : "); Serial.println(messageString);
      // read file data and transmit over wifi network
      SD_card_read_directory();
    }
    else
    {
      // append raw data
      SD_card_append_file(messageString);
    }
  }
  delay(1000);
} 
