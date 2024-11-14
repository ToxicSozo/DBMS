#ifndef SQL_PARSED_COMMAND
#define SQL_PARSED_COMMAND

#include "list.h"

typedef struct SQLParsedCommand {
    List *tables;
    List *columns;
    List *values;
    uint8_t *condition;
} SQLParsedCommand;

SQLParsedCommand* new_sql_parsed_command();
void free_parsed_command(SQLParsedCommand *parsed_command);

#endif