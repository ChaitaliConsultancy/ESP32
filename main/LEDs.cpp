#include <WiFi.h>
#include "LEDs.h"

//Intensity table definition for duty ratio. Total numer of step is 0 to 9
// we have 3 channels Intensity will be sum of the three channels=
//0 = 20%DC, 1= 30%DC, 2 = 60%, 3 = 90%DC
byte ubIntensityTbl_Ch1[10] = {0, 0, 1, 1, 1, 2, 2, 2, 3, 3};
byte ubIntensityTbl_Ch2[10] = {0, 1, 0, 1, 2, 1, 2, 3, 2, 3};
byte ubIntensityTbl_Ch3[10] = {0, 0, 1, 1, 1, 2, 2, 2, 3, 3};

void leds_init(void)
{
   /* Initialize PWM Channels with Frequency and Resolution */
  ledcSetup(RLED_PWM_CH, RLED_PWM_FREQ, RLED_PWM_RESOLUTION);
  ledcSetup(GLED_PWM_CH, GLED_PWM_FREQ, GLED_PWM_RESOLUTION);
  ledcSetup(BLED_PWM_CH, BLED_PWM_FREQ, BLED_PWM_RESOLUTION);
  /* Attach the LED PWM Channel to the GPIO Pin */
  ledcAttachPin(RLED_PWM_PIN, RLED_PWM_CH);
  ledcAttachPin(GLED_PWM_PIN, GLED_PWM_CH);
  ledcAttachPin(BLED_PWM_PIN, BLED_PWM_CH);
}

void update_channle_1_leds(int value)
{
  int uhwRedDutyCycle;
  switch (ubIntensityTbl_Ch1[value])
  {
      case 00:
        uhwRedDutyCycle = map(20, 0, 100, 0, RLED_MAX_DUTY_CYCLE);
        break;
      
      case 01:
        uhwRedDutyCycle = map(30, 0, 100, 0, RLED_MAX_DUTY_CYCLE);
        break;
      
      case 02:
        uhwRedDutyCycle = map(60, 0 , 100, 0, RLED_MAX_DUTY_CYCLE);
        break;
      
      case 03:
        uhwRedDutyCycle = map(90, 0, 100, 0, RLED_MAX_DUTY_CYCLE);
        break;
      
      default:
        uhwRedDutyCycle = 0;
        break;
  }
  ledcWrite(RLED_PWM_CH, uhwRedDutyCycle);
}


void update_channle_2_leds(int value)
{
  int uhwGreenDutyCycle;
  switch (ubIntensityTbl_Ch2[value])
  {
      case 00:
        uhwGreenDutyCycle = map(20, 0, 100, 0, GLED_MAX_DUTY_CYCLE);
        break;
      
      case 01:
        uhwGreenDutyCycle = map(30, 0, 100, 0, GLED_MAX_DUTY_CYCLE);
        break;
      
      case 02:
        uhwGreenDutyCycle = map(60, 0, 100, 0, GLED_MAX_DUTY_CYCLE);
        break;
      
      case 03:
        uhwGreenDutyCycle = map(90, 0, 100, 0, GLED_MAX_DUTY_CYCLE);
        break;
      
      default:
        uhwGreenDutyCycle = 0;
        break;
  }
  ledcWrite(GLED_PWM_CH, uhwGreenDutyCycle);
}

void update_channle_3_leds(int value)
{
  int uhwBlueDutyCycle;
  switch (ubIntensityTbl_Ch3[value])
  {
      case 00:
        uhwBlueDutyCycle = map(20, 0, 100, 0, BLED_MAX_DUTY_CYCLE);
        break;
      
      case 01:
        uhwBlueDutyCycle = map(30, 0, 100, 0, BLED_MAX_DUTY_CYCLE);
        break;
      
      case 02:
        uhwBlueDutyCycle = map(60, 0, 100, 0, BLED_MAX_DUTY_CYCLE);
      break;
      
      case 03:
        uhwBlueDutyCycle = map(90, 0, 100, 0, BLED_MAX_DUTY_CYCLE);
        break;
      
      default:
        uhwBlueDutyCycle = 0;
        break;
  }
  ledcWrite(BLED_PWM_CH, uhwBlueDutyCycle);
}
