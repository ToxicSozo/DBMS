#include "../include/json_reader.h"
#include "../include/json_parser.h"
#include "../include/schema.h"
#include "../include/db_creator.h"

int main() {
    char *json_data = load_JSON_file("scheme.json");
    Schema* schema = parse_json(json_data);

    create_db_structure(schema);

    

    free(json_data);

    return 0;
}