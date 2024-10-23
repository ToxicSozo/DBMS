#ifndef STATEMENTS_H
#define STATEMENTS_H

#include "input_buffer.h"
#include "database.h"
#include "insert.h"
#include "select.h"
#include "delete.h"

typedef enum {
    PREPARE_SUCCESS,
    PREPARE_UNRECOGNIZED_STATEMENT
} PrepareResult;

typedef enum { STATEMENT_INSERT, STATEMENT_SELECT, STATEMENT_DELETE } StatementType;

typedef struct {
    StatementType type;
} Statement;

PrepareResult prepare_statement(InputBuffer *input_buffer, Statement *statement);
void execute_statement(DataBase *db, Statement *statement, InputBuffer *input_buffer);

#endif