#ifndef DB_CREATOR_H
#define DB_CREATOR_H

#include "database.h"
#include <dirent.h>

void build_database_file_system(DataBase *schema);
bool lock_table(const char *db_path, const char *table_name);
void unlock_table(const char *db_path, const char *table_name);

#endif