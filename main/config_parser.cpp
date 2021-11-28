#include "FS.h"
#include "SD.h"
#include "config_parser.h"


void remove_white_spaces(char * str) ;

/*******************************************************************************
 * general function
 ******************************************************************************/
void remove_white_spaces(char * str) 
{ 
    // To keep track of non-space character count 
  int i = 0; 
  int count = 0; 
  
  for (i = 0; str[i]; i++) 
  {
    if (str[i] != ' ') 
    {
      str[count++] = str[i]; // here count is incremented 
    }
  }
    str[count] = '\0'; 
} 


/**************************************************************************************************
 *
 * @fn          SerialConfigParser
 *
 * @brief       This function searches for a string a returns its value
 *
 * input parameters
 *
 * @param          configFilePath   - path to configuration file
 * @param          section          - section to search for
 * @param          key                                                         - key to return value of within section
 *
 * output parameters
 *
 * None.
 *
 * @return      None.
 *
 **************************************************************************************************/
int SerialConfigParser(char * file_name, const char * section, const char * key, char * resultString)
{
  uint8_t sectionFound = false, invalidLineLen = false;
  char * resString = (char*)malloc(128);
  char * resStringToFree = resString;
  char * psStr;     // Processing string pointer
  int res = CONFIG_PARSER_FAILED;
  // Do nothing if the file doesn't exist
  File serialCfgFd = SD.open(file_name, FILE_READ);
  if(!serialCfgFd)
  {
    Serial.println("unable to open config file");
    return 0;
  }
  if (serialCfgFd != NULL)
  {
    // Make sure we start search from the beginning of the file
    serialCfgFd.seek(SEEK_SET);

    // Search through the configuration file for the wanted
    while(serialCfgFd.available())
    {
      int i = 0;
      memset(resString,0,sizeof(resString));
      char ch = serialCfgFd.read();
      resString[i]= ch;
      i++;
      while(ch != '\n')
      {
        ch = serialCfgFd.read();
        resString[i] = ch;
        i++;
      }
      resString[i] = '\0';
      // Serial.printf("resString : %s\n", resString);
      // Check if we have a valid line, i.e. begins with [.
      // Note! No valid line can span more than 128 bytes. Hence we
      // must hold off parsing until we hit a newline.
      if (strlen(resString) == 128)
      {
        invalidLineLen = true;
        Serial.println("\t SerialConfigParser : Found line > 128 bytes");
        fflush(stdout);
      }
      else
      {
        // First time we find a valid line length after having
        // found invalid line length may be the end of the
        // invalid line. Hence, do not process this string.
        // We set the invalidLineLen parameter to false after
        // the processing logic.
        if (invalidLineLen == false)
        {
          // Remove the newline character (ok even if line had length 128)
          resString[strlen(resString) - 1] = '\0';

          // Serial.println"\tSerialConfigParser : Found line < 128 bytes");
          fflush(stdout);
          if (resString[0] == '[')
          {
            // Serial.println"\tSerialConfigParser : Found section %s", resString);
            // Search for wanted section
            psStr = strstr(resString, section);
            if (psStr != NULL)
            {
              resString = psStr;
              // We found our wanted section. Now search for wanted key.
              sectionFound = true;
              // Serial.println"\tSerialConfigParser : Found wanted section!");
            }
            else
            {
              // We found another section.
              sectionFound = false;
            }
          }
          else if (sectionFound == true)
          {
            // Serial.println"\t(SerialConfigParser : Line to process %s (strlen=%zd)", resString, strlen(resString));
            // We have found our section, now we search for wanted key
            // Check for commented lines, tagged with '#', and
            // lines > 0 in length
            if ((resString[0] != '#') && (strlen(resString) > 0))
            {
              // Search for wanted section
              psStr = strstr(resString, key);
              if (psStr != NULL)
              {
                // Serial.println("\tSerialConfigParser : Found key \t'%s' in \t'%s'", key, resString);
                // We found our key. The value is located after the '='
                // after the key.
                // printf("%s", psStr);
                psStr = strtok(psStr, "=");
                // printf("%s", psStr);
                psStr = strtok(NULL, "=;\"");
                // printf("%s", psStr);

                resString = psStr;
                // Serial.printf("\tSerialConfigParser : Found value '%s'\n", (char *)psStr);
                // remove_white_spaces(resString);
                strncpy(resultString, resString, strlen(resString));
                // Serial.printf("\tSerialConfigParser : Found value2 '%s'\n", resultString);
                res = CONFIG_PARSER_SUCCESS;
                // We can return this string to the calling function
                break;
              }
            }
          }
        }
        else
        {
          Serial.println("\t SerialConfigParser :  Found end of line > 128 bytes");
          invalidLineLen = false;
        }
      }
    }
  }
  else
  {
    serialCfgFd.close();
    free(resStringToFree);
    return CONFIG_PARSER_FAILED;
  }
  serialCfgFd.close();
  free(resStringToFree);
  return res;
}
