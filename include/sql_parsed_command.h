#ifndef SQL_PARSED_COMMAND
#define SQL_PARSED_COMMAND

typedef struct {
    char *table_name;
    char **values;
    int value_count;
} SQLParsedCommand;

#endif

void free_parsed_command(SQLParsedCommand *parsed_command);