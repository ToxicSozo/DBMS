#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/sql_parsed_command.h"
#include "../include/sql_parser.h"
#include "../include/statements.h"
#include "../include/str_split.h"
#include "../include/list.h"

static SQLParsedCommand* parse_insert(List *tokens);
static SQLParsedCommand* parse_delete(List *tokens);


SQLParsedCommand* sql_parser(char *buffer, Statement *statement) {
    SQLParsedCommand *parsed_command = NULL;
    
    List *tokens = str_split(buffer, " ");
    
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

    free_list(tokens);

    return parsed_command;
}

static SQLParsedCommand* parse_insert(List *tokens) {
    if (tokens->size < 4 || strcmp(get_element_at(tokens, 1), "INTO") != 0
    || strcmp(get_element_at(tokens, 3), "VALUES") != 0) {

        printf("Invalid INSERT command syntax!\n");
        return NULL;
    }

    SQLParsedCommand *parsed_command = (SQLParsedCommand*)malloc(sizeof(SQLParsedCommand));
    parsed_command->command_type = 0;

    InsertCommand *insert_cmd = &parsed_command->command_data.insert_data;

    insert_cmd->table_name = strdup((char*)get_element_at(tokens, 2));

    insert_cmd->values = new_list();
    if (insert_cmd->values == NULL) {
        printf("Memory allocation failed for insert_cmd->values\n");
        return NULL;
    }

    int value_count = 0;
    for (size_t i = 4; i < tokens->size; i++) {
        push_back(insert_cmd->values, strdup(get_element_at(tokens, i)));
        value_count++;
    }

    insert_cmd->value_count = value_count;

    return parsed_command;
}

static SQLParsedCommand* parse_delete(List *tokens) {
    if (tokens->size < 3 || strcmp((char*)get_element_at(tokens, 1), "FROM") != 0) {
        printf("Invalid DELETE command syntax!\n");
        return NULL;
    }

    SQLParsedCommand *parsed_command = malloc(sizeof(SQLParsedCommand));
    if (!parsed_command) {
        fprintf(stderr, "Memory allocation error.\n");
        return NULL;
    }

    parsed_command->command_type = 1;
    DeleteCommand *delete_cmd = &parsed_command->command_data.delete_data;

    delete_cmd->table_name = strdup((char*)get_element_at(tokens, 2));
    if (!delete_cmd->table_name) {
        fprintf(stderr, "Memory allocation error.\n");
        free(parsed_command);
        return NULL;
    }

    if (tokens->size > 3 && strcmp((char*)get_element_at(tokens, 3), "WHERE") == 0) {
        int condition_length = 0;
        for (size_t i = 4; i < tokens->size; i++) {
            condition_length += strlen((char*)get_element_at(tokens, i)) + 1; // +1 for space or null terminator
        }

        delete_cmd->condition = malloc(condition_length);
        if (!delete_cmd->condition) {
            fprintf(stderr, "Memory allocation error.\n");
            free(delete_cmd->table_name);
            free(parsed_command);
            return NULL;
        }

        delete_cmd->condition[0] = '\0';
        for (size_t i = 4; i < tokens->size; i++) {
            strcat(delete_cmd->condition, (char*)get_element_at(tokens, i));
            if (i < tokens->size - 1) {
                strcat(delete_cmd->condition, " ");
            }
        }
    } else {
        delete_cmd->condition = NULL;
    }

    return parsed_command;
}