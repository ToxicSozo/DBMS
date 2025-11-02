#ifndef SELECT_H
#define SELECT_H

#include "database.h"
#include "csv_reader.h"
#include "csv_writer.h"
#include "sql_parsed_command.h"

void Select(DataBase *db, SQLParsedCommand *parsed_comand);

#endif 