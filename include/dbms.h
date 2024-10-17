#ifndef DBMS_H
#define DBMS_H

#include "csv_reader.h"
#include "csv_writer.h"
#include "database.h"

void dbms ();
void insert(Table *table, const char **data);

#endif