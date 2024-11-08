#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/sql_parsed_command.h"
#include "../include/sql_parser.h"
#include "../include/statements.h"
#include "../include/str_split.h"

static SQLParsedCommand* parse_insert(char **tokens);
static SQLParsedCommand* parse_delete(char **tokens);

SQLParsedCommand* sql_parser(char *buffer, Statement *statement) {
    SQLParsedCommand *parsed_command = NULL;

    const char *delim[] = {" "};
    
    char **tokens = str_split(buffer, delim, 1);
    
    switch (statement->type) {
        case STATEMENT_INSERT:
            parsed_command = parse_insert(tokens);
            break;
        case STATEMENT_SELECT:
            break;
        case STATEMENT_DELETE:
            parsed_command = parse_delete(tokens);
            break;
    }

    free_split(tokens);

    return parsed_command;
}

static SQLParsedCommand* parse_insert(char **tokens) {
    if (!tokens[1] || strcmp(tokens[1], "INTO") != 0 || !tokens[2] || !tokens[3] || strcmp(tokens[3], "VALUES") != 0) {
        printf("Invalid INSERT command syntax!\n");
        return NULL;
    }

    SQLParsedCommand *parsed_command = malloc(sizeof(SQLParsedCommand));
    if (!parsed_command) {
        fprintf(stderr, "Memory allocation error.\n");
        return NULL;
    }
    
    parsed_command->command_type = 0; // 0 for insert
    InsertCommand *insert_cmd = &parsed_command->command_data.insert_data;

    insert_cmd->table_name = strdup(tokens[2]);
    if (!insert_cmd->table_name) {
        fprintf(stderr, "Memory allocation error.\n");
        free(parsed_command);
        return NULL;
    }

    int value_count = 0;
    for (char **token = &tokens[4]; *token; token++) {
        value_count++;
    }

    insert_cmd->values = malloc((value_count + 1) * sizeof(char *));
    if (!insert_cmd->values) {
        fprintf(stderr, "Memory allocation error.\n");
        free(insert_cmd->table_name);
        free(parsed_command);
        return NULL;
    }

    int idx = 0;
    for (char **token = &tokens[4]; *token; token++) {
        insert_cmd->values[idx++] = strdup(*token);
        if (!insert_cmd->values[idx - 1]) {
            fprintf(stderr, "Memory allocation error.\n");
            for (int i = 0; i < idx; i++) {
                free(insert_cmd->values[i]);
            }
            free(insert_cmd->values);
            free(insert_cmd->table_name);
            free(parsed_command);
            return NULL;
        }
    }
    insert_cmd->values[idx] = NULL;
    insert_cmd->value_count = value_count;

    return parsed_command;
}

static SQLParsedCommand* parse_delete(char **tokens) {
    if (!tokens[1] || strcmp(tokens[1], "FROM") != 0 || !tokens[2]) {
        printf("Invalid DELETE command syntax!\n");
        return NULL;
    }

    SQLParsedCommand *parsed_command = malloc(sizeof(SQLParsedCommand));
    if (!parsed_command) {
        fprintf(stderr, "Memory allocation error.\n");
        return NULL;
    }

    parsed_command->command_type = 1; // 1 for delete
    DeleteCommand *delete_cmd = &parsed_command->command_data.delete_data;

    delete_cmd->table_name = strdup(tokens[2]);
    if (!delete_cmd->table_name) {
        fprintf(stderr, "Memory allocation error.\n");
        free(parsed_command);
        return NULL;
    }

    if (tokens[3] && strcmp(tokens[3], "WHERE") == 0) {
        // Collect all tokens after "WHERE" into a single string
        int condition_length = 0;
        for (char **token = &tokens[4]; *token; token++) {
            condition_length += strlen(*token) + 1; // +1 for space or null terminator
        }

        delete_cmd->condition = malloc(condition_length);
        if (!delete_cmd->condition) {
            fprintf(stderr, "Memory allocation error.\n");
            free(delete_cmd->table_name);
            free(parsed_command);
            return NULL;
        }

        delete_cmd->condition[0] = '\0'; // Initialize the string
        for (char **token = &tokens[4]; *token; token++) {
            strcat(delete_cmd->condition, *token);
            strcat(delete_cmd->condition, " ");
        }
        delete_cmd->condition[condition_length - 1] = '\0'; // Remove the trailing space
    } else {
        delete_cmd->condition = NULL;
    }

    return parsed_command;
}