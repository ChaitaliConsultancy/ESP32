#ifndef SENSORS_H
#define SENSORS_H

#define MOTION_SENSOR_IP_1     2
#define MOTION_SENSOR_IP_2     4
#define MOTION_SENSOR_IP_3     25

#define LUX_I2C_SDA            16
#define LUX_I2C_SCL            17

#define BH1750_SLAVE_ADDR      0x5C

void sensors_init(void);
uint16_t LUX_BH1750_Read(void);

#endif //#ifdef SENSORS_H
