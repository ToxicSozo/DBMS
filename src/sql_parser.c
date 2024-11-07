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
    
    char **tokens = str_split(buffer, ' ');
    
    switch (statement->type) {
        case (STATEMENT_INSERT):
            parsed_command = parse_insert(tokens);
            break;
        case (STATEMENT_SELECT):
            break;
        case (STATEMENT_DELETE):
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
    
    parsed_command->table_name = strdup(tokens[2]);
    if (!parsed_command->table_name) {
        fprintf(stderr, "Memory allocation error.\n");
        free(parsed_command);
        return NULL;
    }

    int value_count = 0;
    for (char **token = &tokens[4]; *token; token++) {
        value_count++;
    }

    parsed_command->values = malloc((value_count + 1) * sizeof(char *));
    if (!parsed_command->values) {
        fprintf(stderr, "Memory allocation error.\n");
        free(parsed_command->table_name);
        free(parsed_command);
        return NULL;
    }

    int idx = 0;
    for (char **token = &tokens[4]; *token; token++) {
        parsed_command->values[idx++] = strdup(*token);
        if (!parsed_command->values[idx - 1]) {
            fprintf(stderr, "Memory allocation error.\n");
            for (int i = 0; i < idx; i++) {
                free(parsed_command->values[i]);
            }
            free(parsed_command->values);
            free(parsed_command->table_name);
            free(parsed_command);
            return NULL;
        }
    }
    parsed_command->values[idx] = NULL;
    parsed_command->value_count = value_count;

    return parsed_command;
}