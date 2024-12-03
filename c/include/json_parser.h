#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include "database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

DataBase* parse_json(const char* json);
char* parse_string(const char **json);
int parse_number(const char **json);
void parse_columns_array(const char** json, Table *table);
void parse_tables_object(const char** json, DataBase *db);
void skip_whitespace(const char **json);

#endif