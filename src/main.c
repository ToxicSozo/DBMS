#include "../include/json_reader.h"
#include "../include/json_parser.h"
#include "../include/database.h"
#include "../include/database_directory.h"
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
 
int main() {
    DataBase *schema = parse_json(load_json_data("scheme.json"));
    build_database_file_system(schema);

    free_database(schema);

    return 0;
}