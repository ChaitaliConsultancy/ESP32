#include "SD.h"
#include "SPI.h"
#include "wifi.h"
#include "sd_card.h"
#include "date_time.h"
#include "config_parser.h"

message_t message;
sd_config_t sd_config;

char sdConfigFile[50]   = "/sd_config.ini";

const char * configSectionKey = "SD_CARD";
const char * noOfEntriesKey = "NO_OF_ENTRIES";
const char * noOfFilesKey = "NO_OF_FILES";
const char * durationKey = "DURATION";

const char* directory = "/logs";
const char* device_id = "ALC1234";

SPIClass SPISD(HSPI);

bool SD_card_Init(void)
{
    uint8_t cardType;
    uint64_t cardSize;
    SPISD.begin(SD_SCK, SD_MISO, SD_MOSI);
    Serial.println("SDCard_Init : ");    
    if(!SD.begin(SD_CS_PIN, SPISD))
    {
        Serial.println("\t SD Card Mount Failed");     
        return false;   
    }
    else
    {
        Serial.println("\t SD Card Mount Successfully");
        cardType = SD.cardType();
        if(cardType == CARD_NONE)
        {
            Serial.println("\t No SD card attached");        
        }
        Serial.print("\t SD Card Type: ");
        if(cardType == CARD_MMC)
        {
            Serial.println("MMC");
        } 
        else if(cardType == CARD_SD)
        {
            Serial.println("SDSC");
        }
        else if(cardType == CARD_SDHC)
        {
            Serial.println("SDHC");
        } 
        else 
        {
            Serial.println("UNKNOWN");
        }
        cardSize = SD.cardSize() / (1024 * 1024);
        Serial.printf("\t SD Card Size: %lluMB\n", cardSize);   
        
        Serial.println("\t sd card config file opened up, read configurations");
        sd_config.no_of_enteirs = sd_config_get_no_of_entries();
        sd_config.no_of_files = sd_config_get_no_of_files();
        sd_config.duration = sd_config_get_duration();
        Serial.printf("\t noOfEntries : %d\n", sd_config.no_of_enteirs);
        Serial.printf("\t noOfFiles : %d\n", sd_config.no_of_files);
        Serial.printf("\t duration : %d\n", sd_config.duration);
        return true;
    }
}


void get_file_path(char *file)
{
  // test_code to generate day pass condition
  // snprintf (file, FILE_PATH_LEN, "%s%c%s_%04d_%02d_%02d.csv", directory, '/', device_id, dateTime_sample.tm_year, dateTime_sample.tm_mon, dateTime_sample.tm_mday);
  snprintf (file, FILE_PATH_LEN, "%s%c%s_%04d_%02d_%02d.csv", directory, '/', device_id, dateTime_sample.tm_year, dateTime_sample.tm_mon, dateTime_sample.tm_min);
}

int SD_card_get_file_count(const char * sdDirectory) 
{
  int file_count = 0;
  File dir = SD.open(sdDirectory);
  if(!dir)
  {
    Serial.println("SD_card_get_file_count : failed to open directory");
    return false;
  }
  if(!dir.isDirectory())
  {
    Serial.println("SD_card_get_file_count : not a directory");
    return false;
  }
  File file = dir.openNextFile();
  while(file)
  {
    if(!file.isDirectory())
    {
      file_count++;
    }
    file = dir.openNextFile();
  }
  file.close();
  dir.close();
  return file_count;
}


void get_date_of_log_file(char * file, struct Date *date)
{
  int NOE = 0;
  char file_name[FILE_PATH_LEN];
  strcpy(file_name, file);
  char* subString = NULL;
  subString = strtok(file_name, "_");
  while(subString != NULL)
  {
    if(NOE == 1)
    {
      date->y = atoi(subString);
      // Serial.print("year : ");Serial.println(date->y);
    }
    else if(NOE == 2)
    {
      date->m = atoi(subString);
      // Serial.print("month : ");Serial.println(date->m);
    }
    else if(NOE == 3)
    {
      subString = strtok(subString, ".");
      date->d = atoi(subString);
      // Serial.print("date : ");Serial.println(date->d);
    }
    subString = strtok(NULL, "_");
    NOE++;
  }
}


int SD_card_delete_old_file(const char * directory) 
{
  char referance_file[FILE_PATH_LEN];
  char index_file[FILE_PATH_LEN];
  File dir = SD.open(directory);
  if(dir)
  {
    bool swap = false; bool exicute = true;
    File file = dir.openNextFile();
    strcpy(referance_file, file.name());
    while(exicute)
    {
      file = dir.openNextFile();
      if (!file) 
      {
        if(swap)
        {
          dir.rewindDirectory(); 
          swap = false;
        }
        else
        {
          dir.rewindDirectory(); 
          exicute = false;
        }
      }
      else
      {
        Date d1, d2; 
        strcpy(index_file, file.name());
        get_date_of_log_file(referance_file, &d1);
        get_date_of_log_file(index_file, &d2);
        int date_count = getDifference(d1, d2);
        if(date_count < 0)
        {
          strcpy(referance_file, index_file);
          swap = true;
        }
      }
    }
  
    if(SD.remove(referance_file))
    {
      Serial.print("\t File deleted : "); Serial.println(referance_file);
    }
    else
    {
      Serial.print("\t unable to delete File : "); Serial.println(referance_file);
    }
    file.close();
    dir.close();
  }
  return 0;
}

bool SD_card_create_file(void)
{
  Serial.println("SD_card_create_file : ");
  if (!SD.exists(directory)) 
  {    
    SD.mkdir(directory);
    Serial.print("\t directory created : "); Serial.println(directory);
  }
  
  File dir = SD.open(directory);
  int count = SD_card_get_file_count(directory);
  while(count >= sd_config.no_of_files)
  {
    SD_card_delete_old_file(directory);
    count = SD_card_get_file_count(directory);
  }
  dir.rewindDirectory();
  dir.close();
  
  char file_name[FILE_PATH_LEN];
  get_file_path(file_name);
  if (SD.exists(file_name)) 
  {    
    Serial.println("\t file alreay exists");
    return true;
  }
  File file = SD.open(file_name, FILE_WRITE);
  if(file)
  {
    Serial.print("\t file created : "); Serial.println(file_name);
    char information_data[1024] = "DAY, MONTH, YEAR, HOUR, MINUT, SECONDS, AMBEINT, OCC1, OCC2, OCC3, RPWM, GPWM, BPWM,  P1,  P2,  P3,  P4,  P5,  P6,  P7,  P8,  P9,  P10, ERROR";
    file.println(information_data);
    file.close();
    return true;
  }
  return false;
}

bool SD_card_append_file(const char * message) 
{
  char file_name[FILE_PATH_LEN];
  get_file_path(file_name);
  Serial.println("SD_card_append_file : ");
  if (!SD.exists(file_name)) 
  {    
    // Serial.print("\t file dose not exists : "); Serial.println(file_name);
    if(!SD_card_create_file())
    {
      return false;  
    }
  }
  File file = SD.open(file_name, FILE_APPEND);
  if(!file) 
  {
    Serial.println("\t failed to open file");
    return false;
  }
  if(file.println(message))
  {
    Serial.print("\t store message : "); Serial.println(message);
  } 
  else 
  {
    Serial.println("\t failed to append data");
  }
  file.close();
  return true;
}



bool SD_card_read_directory() 
{
  Serial.println("SD_card_read_directory : ");
  File dir = SD.open(directory);
  if(!dir)
  {
    Serial.println("\t failed to open directory");
    return false;
  }
  File file = dir.openNextFile();
  while(file)
  {
    if(file.isDirectory())
    {
      Serial.print("DIR : ");
      Serial.println(file.name());
    } 
    else 
    {
      Serial.print("\t file_name : "); Serial.print(file.name());
      Serial.print("\t size: "); Serial.println(file.size());

      File rfile = SD.open(file.name());
      if(rfile)
      {
        long num_lines = 1;
        String msg = "";
        while(rfile.available())
        {
          char ch = rfile.read();
          if(ch == '\n')
          {
            if(num_lines != 1)
            {
              char messageString[256];
              msg.toCharArray(messageString, 256);
              publishDataToMQTTClient((char *)messageString);
              Serial.print("\t\t pending messages : "); Serial.println(messageString);
            }
            num_lines++;
            msg = "";
          }
          else
          {
            msg += ch;
          }
        }
        rfile.close();
      }
    }
    if(SD.remove(file.name()))
    {
      Serial.print("\t file deleted : "); Serial.println(file.name());
    }
    else
    {
      Serial.print("\t unable to delete file : "); Serial.println(file.name());
    }
    file.close();
    file = dir.openNextFile();
  }
  dir.close();
  return true;
}


uint16_t sd_config_get_no_of_entries(void)
{
  char string[50];
  memset(&string, 0, sizeof(string));
  if(SerialConfigParser(sdConfigFile, configSectionKey, noOfEntriesKey, string))
  {
      Serial.println("\t no of entries config not found");
      return false;
  }
  return (atol(string));
}

uint16_t sd_config_get_no_of_files(void)
{
  char string[50];
  memset(&string, 0, sizeof(string));
  if(SerialConfigParser(sdConfigFile, configSectionKey, noOfFilesKey, string))
  {
      Serial.println("\t no of files config not found");
      return false;
  }
  return (atol(string));
}

uint32_t sd_config_get_duration(void)
{
  char string[50];
  memset(&string, 0, sizeof(string));
  if(SerialConfigParser(sdConfigFile, configSectionKey, durationKey, string))
  {
      Serial.println("\t duration config not found");
      return false;
  }
  return (atol(string));
}
