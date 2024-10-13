#ifndef DB_CREATOR_H
#define DB_CREATOR_H

#include "../include/database.h"
#include <dirent.h>

typedef struct {
    char **path_files;
    size_t file_count;
} Files;

typedef struct {
    char *name_dir;
    char *path_dir;
    Files *files;
} Subdirectory;

typedef struct {
    char *name;
    Subdirectory *subdirectories;
    size_t subdirectory_count;
} DataBaseDir;

void build_database_file_system(DataBase *schema);
void collect_database_file_system(DataBaseDir *db_dir, DataBase *schema);

#endif