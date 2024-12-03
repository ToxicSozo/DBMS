#include <stdlib.h>

#include "../include/sql_parsed_command.h"

SQLParsedCommand* new_sql_parsed_command() {
    SQLParsedCommand *parsed_command = (SQLParsedCommand *)malloc(sizeof(SQLParsedCommand));
    if (!parsed_command) {
        return NULL;
    }

    parsed_command->tables = new_list();
    parsed_command->columns = new_list();
    parsed_command->values = new_list();

    parsed_command->condition = NULL;
    
    if (!parsed_command->tables || !parsed_command->columns || !parsed_command->values) {
        free_parsed_command(parsed_command);
        return NULL;
    }

    return parsed_command;
}

void free_parsed_command(SQLParsedCommand *parsed_command) {
    if (!parsed_command) return;

    if (parsed_command->tables)     free_list(parsed_command->tables);
    if (parsed_command->columns)    free_list(parsed_command->columns);
    if (parsed_command->values)     free_list(parsed_command->values);
    if (parsed_command->condition)  free(parsed_command->condition);

    free(parsed_command);
}