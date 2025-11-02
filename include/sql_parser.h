#ifndef SQL_PARSER_H
#define SQL_PARSER_H

#include "sql_parsed_command.h"
#include "statements.h"

SQLParsedCommand* sql_parser(char *buffer, Statement *statement);

#endif