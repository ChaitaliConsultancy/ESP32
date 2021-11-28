#ifndef WIFI_H
#define WIFI_H

typedef enum{
  SUCCESS,
  FAILED
}connection_state;

int reconnect(void);
void mqttClientLoop(void);
int wifiSetup(char *file_name);
int publishDataToMQTTClient(char * messsage);

#endif //#ifdef WIFI_H
