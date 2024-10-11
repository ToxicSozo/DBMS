#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include "schema.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

Schema* parse_json(const char* json);
char* parse_string(const char **json);
int parse_number(const char **json);
ColumnsList* parse_columns_array(const char** json);
TableEntry* parse_tables_object(const char** json, size_t* table_count);
void skip_whitespace(const char **json);

#endif