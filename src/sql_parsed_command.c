#include <stdlib.h>

#include "../include/sql_parsed_command.h"

void free_parsed_command(SQLParsedCommand *parsed_command) {
    if (parsed_command == NULL) {
        return;
    }

    if (parsed_command->command_type == 0) {
        InsertCommand *insert_cmd = &parsed_command->command_data.insert_data;

        if (insert_cmd->table_name != NULL) {
            free(insert_cmd->table_name);
        }
        if (insert_cmd->values != NULL) {
            free_list(insert_cmd->values);
        }
    } else if (parsed_command->command_type == 1) {
        DeleteCommand *delete_cmd = &parsed_command->command_data.delete_data;
        if (delete_cmd->table_name != NULL) {
            free(delete_cmd->table_name);
        }
        if (delete_cmd->condition != NULL) {
            free(delete_cmd->condition);
        }
    }

    free(parsed_command);
}