#ifndef LEDS_H
#define LEDS_H

#include <math.h>

#define RLED_PWM_PIN            23  /* GPIO16 */
#define GLED_PWM_PIN            22  /* GPIO17 */
#define BLED_PWM_PIN            21  /* GPIO4 */

#define RLED_PWM_FREQ           500 /* 500 Hz */
#define RLED_PWM_CH             0
#define RLED_PWM_RESOLUTION     14
const int RLED_MAX_DUTY_CYCLE   = (int)(pow(2, RLED_PWM_RESOLUTION) - 1);

#define GLED_PWM_FREQ           500 /* 500 Hz */
#define GLED_PWM_CH             2
#define GLED_PWM_RESOLUTION     14
const int GLED_MAX_DUTY_CYCLE   = (int)(pow(2, GLED_PWM_RESOLUTION) - 1);

#define BLED_PWM_FREQ           500 /* 500 Hz */
#define BLED_PWM_CH             4
#define BLED_PWM_RESOLUTION     14
const int BLED_MAX_DUTY_CYCLE   = (int)(pow(2, BLED_PWM_RESOLUTION) - 1);


void leds_init(void);

void update_channle_1_leds(int value);

void update_channle_2_leds(int value);

void update_channle_3_leds(int value);

#endif //#ifdef LEDS_H
