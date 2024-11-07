#include <stdlib.h>

#include "../include/sql_parsed_command.h"

void free_parsed_command(SQLParsedCommand *parsed_command) {
    if (!parsed_command) {
        return;
    }

    if (parsed_command->table_name) {
        free(parsed_command->table_name);
    }

    if (parsed_command->values) {
        for (int i = 0; i < parsed_command->value_count; i++) {
            free(parsed_command->values[i]);
        }
        free(parsed_command->values);
    }

    free(parsed_command);
}