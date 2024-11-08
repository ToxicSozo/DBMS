#ifndef SQL_PARSED_COMMAND
#define SQL_PARSED_COMMAND

typedef struct {
    char *table_name;
    char **values;
    int value_count;
} InsertCommand;

typedef struct {
    char *table_name;
    char *condition;
} DeleteCommand;

typedef struct {
    union {
        InsertCommand insert_data;
        DeleteCommand delete_data;
    } command_data;
    int command_type; 
} SQLParsedCommand;

#endif

void free_parsed_command(SQLParsedCommand *parsed_command);