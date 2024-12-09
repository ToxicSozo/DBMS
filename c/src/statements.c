#include "../include/statements.h"
#include "../include/sql_parsed_command.h"

#include <stdio.h>
#include <string.h>

PrepareResult prepare_statement(char *buffer, Statement *statement) {
    if (strncmp(buffer, "INSERT", 6) == 0) {
        statement->type = STATEMENT_INSERT;
        return PREPARE_SUCCESS;
    } else if (strncmp(buffer, "SELECT", 6) == 0) {
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    } else if (strncmp(buffer, "DELETE", 6) == 0) {
        statement->type = STATEMENT_DELETE;
        return PREPARE_SUCCESS;
    }

    return PREPARE_UNRECOGNIZED_STATEMENT;
}

void execute_statement(DataBase *db, Statement *statement, SQLParsedCommand *parsed_comand) {
    switch (statement->type) {
        case (STATEMENT_INSERT):
            insert(db, parsed_comand);
            break;
        case (STATEMENT_SELECT):
            // Select(db, parsed_comand);
            break;
        case (STATEMENT_DELETE):
            // delete(db, parsed_comand);
            break;
    }
}