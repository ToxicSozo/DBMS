#include "../include/statements.h"

#include <stdio.h>
#include <string.h>

PrepareResult prepare_statement(InputBuffer *input_buffer, Statement *statement) {
    if (strncmp(input_buffer->buffer, "INSERT", 6) == 0) {
        statement->type = STATEMENT_INSERT;
        return PREPARE_SUCCESS;
    } else if (strncmp(input_buffer->buffer, "SELECT", 6) == 0) {
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    } else if (strncmp(input_buffer->buffer, "DELETE", 6) == 0) {
        statement->type = STATEMENT_DELETE;
        return PREPARE_SUCCESS;
    }

    return PREPARE_UNRECOGNIZED_STATEMENT;
}

void execute_statement(DataBase *db, Statement *statement, InputBuffer *input_buffer) {
    switch (statement->type) {
        case (STATEMENT_INSERT):
            insert(db, input_buffer->buffer);
            break;
        case (STATEMENT_SELECT):
            Select(db, input_buffer->buffer);
            break;
        case (STATEMENT_DELETE):
            delete(db, input_buffer->buffer);
            break;
    }
}