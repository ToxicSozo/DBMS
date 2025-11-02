#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/sql_parsed_command.h"
#include "../include/sql_parser.h"
#include "../include/statements.h"
#include "../include/str_split.h"
#include "../include/list.h"

static SQLParsedCommand* parse_insert(SQLParsedCommand *parsed_command, List *tokens);
static SQLParsedCommand* parse_select(SQLParsedCommand *parsed_command, List *tokens);
static SQLParsedCommand* parse_delete(SQLParsedCommand *parsed_command, List *tokens);

SQLParsedCommand* sql_parser(char *buffer, Statement *statement) {
    SQLParsedCommand *parsed_command = new_sql_parsed_command();
    
    List *tokens = str_split(buffer, " ");
    
    switch (statement->type) {
        case STATEMENT_INSERT:
            parsed_command = parse_insert(parsed_command, tokens);
            break;
        case STATEMENT_SELECT:
            parsed_command = parse_select(parsed_command, tokens);
            break;
        case STATEMENT_DELETE:
            parsed_command = parse_delete(parsed_command, tokens);
            break;
    }

    free_list(tokens);

    return parsed_command;
}

static SQLParsedCommand* parse_insert(SQLParsedCommand *parsed_command, List *tokens) {
    if (tokens->size < 4 || strcmp(get_element_at(tokens, 1), "INTO") != 0
    || strcmp(get_element_at(tokens, 3), "VALUES") != 0) {
        printf("Invalid INSERT command syntax!\n");
        return NULL;
    }

    push_back(parsed_command->tables, 
              strdup(get_element_at(tokens, 2)));

    for (size_t i = 4; i < tokens->size; i++) {
        push_back(parsed_command->values, 
                  strdup(get_element_at(tokens, i)));
    }

    return parsed_command;
}

static SQLParsedCommand* parse_select(SQLParsedCommand *parsed_command, List *tokens) {
    if(tokens->size < 3 || !(elemin_list(tokens, "FROM"))) {
        printf("Invalid SELECT command syntax!\n");
        return NULL;
    }

    size_t i = 1;
    while (i < tokens->size && strcmp((char*)get_element_at(tokens, i), "FROM") != 0) {
        uint8_t *column = strdup(get_element_at(tokens, i));
        
        size_t len = strlen(column);
        if (column[len - 1] == ',') {
            column[len - 1] = '\0';
        }
        push_back(parsed_command->columns, column);
        i++;
    }
    i++;

    while (i < tokens->size && strcmp((char*)get_element_at(tokens, i), "WHERE") != 0) {
        uint8_t *table = strdup(get_element_at(tokens, i));
        
        size_t len = strlen(table);
        if (table[len - 1] == ',') {
            table[len - 1] = '\0';
        }
        push_back(parsed_command->tables, table);
        i++;
    }

    if(elemin_list(tokens, "WHERE")) {
        i++;

        int condition_length = 0;
        
        for (size_t j = i; j < tokens->size; j++) {
            condition_length += strlen(get_element_at(tokens, j)) + 1;
        }

        parsed_command->condition = malloc(condition_length);

        parsed_command->condition[0] = '\0';
        for (size_t j = i; j < tokens->size; j++) {
            strcat(parsed_command->condition, (char*)get_element_at(tokens, j));
            if (j < tokens->size - 1) {
                strcat(parsed_command->condition, " ");
            }
        }
    }

    return parsed_command;
}

static SQLParsedCommand* parse_delete(SQLParsedCommand *parsed_command, List *tokens) {
    if (tokens->size < 3 || strcmp((char*)get_element_at(tokens, 1), "FROM") != 0) {
        printf("Invalid DELETE command syntax!\n");
        return NULL;
    }

    push_back(parsed_command->tables, 
              strdup(get_element_at(tokens, 2)));

    if (tokens->size > 3 && strcmp((char*)get_element_at(tokens, 3), "WHERE") == 0) {        
        int condition_length = 0;
        
        for (size_t i = 4; i < tokens->size; i++) {
            condition_length += strlen(get_element_at(tokens, i)) + 1;
        }

        parsed_command->condition = malloc(condition_length);

        parsed_command->condition[0] = '\0';
        for (size_t i = 4; i < tokens->size; i++) {
            strcat(parsed_command->condition, (char*)get_element_at(tokens, i));
            if (i < tokens->size - 1) {
                strcat(parsed_command->condition, " ");
            }
        }
    }

    return parsed_command;
}