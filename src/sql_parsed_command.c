#include <stdlib.h>

#include "../include/sql_parsed_command.h"

void free_parsed_command(SQLParsedCommand *parsed_command) {
    if (parsed_command == NULL) {
        return;
    }

    if (parsed_command->command_type == 0) {
        // Free insert command data
        InsertCommand *insert_cmd = &parsed_command->command_data.insert_data;
        if (insert_cmd->table_name != NULL) {
            free(insert_cmd->table_name);
        }
        if (insert_cmd->values != NULL) {
            for (int i = 0; i < insert_cmd->value_count; i++) {
                if (insert_cmd->values[i] != NULL) {
                    free(insert_cmd->values[i]);
                }
            }
            free(insert_cmd->values);
        }
    } else if (parsed_command->command_type == 1) {
        // Free delete command data
        DeleteCommand *delete_cmd = &parsed_command->command_data.delete_data;
        if (delete_cmd->table_name != NULL) {
            free(delete_cmd->table_name);
        }
        if (delete_cmd->condition != NULL) {
            free(delete_cmd->condition);
        }
    }

    // Free the parsed command itself
    free(parsed_command);
}