#include <WiFi.h>
#include <PubSubClient.h>
#include "wifi.h"
#include "LEDs.h"
#include "sd_card.h"
#include "config_parser.h"

uint16_t mqttPort = 0;
char ssid[50] = "";
char password[50] = "";
char clientId[50] = "";
char mqttServer[50] = "";
char pubChannelTopic[50] = "";
char subChannelTopic[50] = "";

const char * sectionKey = "WIFI";
const char * portKey = "PORT";
const char * ssidKey = "SSID";
const char * passwordKey = "PASSWORD";
const char * clientIdKey = "CLIENT_ID";
const char * serverKey = "MQTT_SERVER_NAME";
const char * pubTopicKey = "PUBLISH_TO";
const char * subTopicKey = "SUBCRIBE_TO";

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient); 



void callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.println("client callback :");
  Serial.print("\t Message Received :");
  char messageRecv[200];
  memset(messageRecv, 0, sizeof(messageRecv));
  for (int i = 0; i < length; i++) {
    messageRecv[i] = (char)payload[i];
  }
  Serial.println(messageRecv);
  char *ipMessage;
  int i = 0; 
  ipMessage = strtok(messageRecv, ","); 
  while(ipMessage != NULL)
  {
    if(i == 0)
    {
      message.r_pwm = atoi(ipMessage);
      update_channle_1_leds(message.r_pwm);
    }
    else if(i == 1)
    {
      message.g_pwm = atoi(ipMessage);
      update_channle_2_leds(message.g_pwm);
    }
    else if(i == 2)
    {
      message.b_pwm = atoi(ipMessage);
      update_channle_3_leds(message.b_pwm);
    }
    ipMessage = strtok(NULL, ","); 
    i++;
  }
}


void setupMQTT() 
{
  mqttClient.setServer(mqttServer, mqttPort);
  // set the callback function
  mqttClient.setCallback(callback);
  reconnect();
}


int connectToWiFi() 
{
  Serial.printf("Connecting to %s\n", ssid);
  WiFi.begin(ssid, password);
  Serial.print("\t");
  int waitingTime = 10;
  while ((WiFi.status() != WL_CONNECTED) && (waitingTime > 0))
  {
      delay(1000);
      waitingTime--;
      Serial.print(".");
  }
  if(WiFi.status() == WL_CONNECTED)
  {
    Serial.println(" CONNECTED");  
    return 1;
  }
  else
  {
    Serial.println(" CONNECTION FAILED");  
    return 0;
  }
}

int wifiSetup(char *file_name) 
{
  Serial.println("wifiSetup :");
  
  Serial.println("\t wifi configuration file opened up read configurations");
  if(SerialConfigParser(file_name, sectionKey, ssidKey, ssid))
  {
      Serial.println("\t ssid config not found");
      return 0;
  }
  if(SerialConfigParser(file_name, sectionKey, passwordKey, password))
  {
    Serial.println("\t password config not found");
    return 0;
  }
  if(SerialConfigParser(file_name, sectionKey, clientIdKey, clientId))
  {
    Serial.println("\t clientId config not found");
    return 0;
  }
  if(SerialConfigParser(file_name, sectionKey, serverKey, mqttServer))
  {
    Serial.println("\t server config not found");
    return 0;
  }
  if(SerialConfigParser(file_name, sectionKey, pubTopicKey, pubChannelTopic))
  {
    Serial.println("\t pubChannelTopic config not found");
    return 0;
  }
  if(SerialConfigParser(file_name, sectionKey, subTopicKey, subChannelTopic))
  {
    Serial.println("\t subChannelTopic config not found");
    return 0;
  }
  char port[50];
  if(SerialConfigParser(file_name, sectionKey, portKey, port))
  {
    Serial.println("\t port config not found");
    return 0;
  }
  mqttPort = atoi(port);
  
  Serial.printf("\t ssid : %s\n", ssid);
  Serial.printf("\t passowrd : %s\n", password);
  Serial.printf("\t mqttServer : %s\n", mqttServer);
  Serial.printf("\t clientId : %s\n", clientId);
  Serial.printf("\t mqttPort : %04d\n", mqttPort);
  Serial.printf("\t pubChannelTopic : %s\n", pubChannelTopic);
  Serial.printf("\t subChannelTopic : %s\n", subChannelTopic);
  connectToWiFi();
  setupMQTT();
  return 1;
}

int reconnect(void)
{
  if(WiFi.status() != WL_CONNECTED)
  {
    if(!connectToWiFi())
    {
      return 0;
    }
  }
  if (!mqttClient.connected())
  {
    Serial.print("Reconnecting to MQTT Broker..");
    if (mqttClient.connect(clientId)) 
    {
      Serial.println("Connected.");
      // subscribe to topic
      mqttClient.subscribe(subChannelTopic);
      return 1;
    }
  }
  Serial.println("Connection falied.");
  return 0;
}

int publishDataToMQTTClient(char * messsage)
{
  bool connectionUp = mqttClient.connected();
  if (!connectionUp)
  {
    connectionUp = reconnect(); 
  }
  if(connectionUp)
  {
    return mqttClient.publish(pubChannelTopic, messsage);  
  }
}


void mqttClientLoop(void)
{
  mqttClient.loop();
}
