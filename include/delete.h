#ifndef DELETE_H
#define DELETE_H

#include "database.h"
#include "csv_reader.h"
#include "csv_writer.h"

#include "sql_parsed_command.h"


void delete(DataBase *db, SQLParsedCommand *parsed_command);

#endif