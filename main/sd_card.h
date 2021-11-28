#ifndef SD_CARD_H
#define SD_CARD_H

#include "SD.h"
#include "FS.h"

#define SD_MOSI       13
#define SD_MISO       05
#define SD_SCK        14
#define SD_CS_PIN     15

#define FILE_PATH_LEN       100

typedef struct {
   uint16_t duration;
   uint16_t no_of_enteirs;
   uint16_t no_of_files;
}sd_config_t;

typedef struct {
   uint8_t tm_sec;
   uint8_t tm_min;
   uint8_t tm_hour;
   uint8_t tm_mday;
   uint8_t tm_mon;
   uint16_t tm_year;
   uint16_t ambiant;
   uint8_t occ1;
   uint8_t occ2;
   uint8_t occ3;
   uint8_t r_pwm;
   uint8_t g_pwm;
   uint8_t b_pwm;
   uint32_t p1;
   uint32_t p2;
   uint32_t p3;
   uint32_t p4;
   uint32_t p5;
   uint32_t p6;
   uint32_t p7;
   uint32_t p8;
   uint32_t p9;
   uint32_t p10;
   uint8_t err;
}message_t;



extern message_t message;

bool SD_card_Init(void);
bool SD_card_create_file(void);
bool SD_card_append_file(const char * message);
bool SD_card_read_directory();

uint16_t sd_config_get_no_of_entries(void);
uint16_t sd_config_get_no_of_files(void);
uint32_t sd_config_get_duration(void);

#endif //#ifdef SD_CARD_H
