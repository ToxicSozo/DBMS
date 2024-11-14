#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/sql_parsed_command.h"
#include "../include/sql_parser.h"
#include "../include/statements.h"
#include "../include/str_split.h"
#include "../include/list.h"

static SQLParsedCommand* parse_insert(SQLParsedCommand *parsed_command, List *tokens);
static SQLParsedCommand* parse_select(List *tokens);
static SQLParsedCommand* parse_delete(List *tokens);

SQLParsedCommand* sql_parser(char *buffer, Statement *statement) {
    SQLParsedCommand *parsed_command = new_sql_parsed_command();
    
    List *tokens = str_split(buffer, " ");
    
    switch (statement->type) {
        case STATEMENT_INSERT:
            parsed_command = parse_insert(parsed_command, tokens);
            break;
        case STATEMENT_SELECT:
            parsed_command = parse_select(tokens);
            break;
        case STATEMENT_DELETE:
            parsed_command = parse_delete(tokens);
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

static SQLParsedCommand* parse_delete(List *tokens) {
}

static SQLParsedCommand* parse_select(List *tokens) {
}