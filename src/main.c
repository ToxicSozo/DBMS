#include "../include/json_reader.h"
#include "../include/json_parser.h"
#include "../include/database.h"
#include "../include/db_creator.h"

int main() {
    char *json_data = load_JSON_file("scheme.json");
    DataBase *schema = parse_json(json_data);

    create_db_structure(schema);

    free_database(schema);
    free(json_data);

    return 0;
}