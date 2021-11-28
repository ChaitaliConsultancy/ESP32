#include <Wire.h>
#include <BH1750.h>
#include <i2cdetect.h>
#include "sensors.h"

BH1750 lightMeter;

void LUX_BH1750_Init(int address) 
{
  Wire.begin(16, 17);
  i2cdetect();
  Wire.beginTransmission(address);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.beginTransmission(address);
  Wire.write(0x01);
  Wire.endTransmission();
  Wire.beginTransmission(address);
  Wire.write(0x07);
  Wire.endTransmission();
  Wire.beginTransmission(address);
  Wire.write(0x10);
  Wire.endTransmission();
}

void PIR_Init(void) 
{
  pinMode(MOTION_SENSOR_IP_1,INPUT);
  pinMode(MOTION_SENSOR_IP_2,INPUT);
  pinMode(MOTION_SENSOR_IP_3,INPUT);
}

void sensors_init(void)
{
  PIR_Init();
  LUX_BH1750_Init(BH1750_SLAVE_ADDR);
}

uint8_t buff[2];
uint16_t BH1750_Read(int address) //
{
  int i=0;
  uint16_t val = 0;
  Serial.println(F("Read function: "));
  Wire.requestFrom(address, 2);
  while(Wire.available()) 
  {
    buff[i] = Wire.read();
    i++;
  }
  val = ((buff[0]<<8)| buff[1]) / 1.2;
//  Serial.printf("val : %d", val);
  val = val + 15;
  return val;
}

uint16_t LUX_BH1750_Read(void)
{
  uint16_t lux = 0;
  lux = BH1750_Read(BH1750_SLAVE_ADDR);
  return lux;
}
