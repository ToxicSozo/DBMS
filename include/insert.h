#ifndef INSERT_H
#define INSERT_H

#include "database.h"
#include "csv_reader.h"
#include "csv_writer.h"
#include "sql_parsed_command.h"

void insert(DataBase *db, SQLParsedCommand *parsed_command);

#endif