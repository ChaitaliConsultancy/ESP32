#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

enum
{
  CONFIG_PARSER_SUCCESS                         = 0,
  CONFIG_PARSER_FAILED                          = 1,
};

#  define SEEK_SET        0       /* Seek from beginning of file.  */
#  define SEEK_CUR        1       /* Seek from current position.  */
#  define SEEK_END        2       /* Set file pointer to EOF plus "offset" */

int SerialConfigParser(char * file_name, const char * section, const char * key, char * resultString);

#endif //#ifdef CONFIG_PARSER_H
