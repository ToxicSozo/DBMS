#ifndef HMAP_H
#define HMAP_H

#include <stdbool.h>
#include <stddef.h>

typedef struct _Hash_Map_Entry {
    char *key;
    void *value;
} Hash_Map_Entry;

typedef struct _Hash_Map {
    Hash_Map_Entry *entries;
    size_t size;
} Hash_Map;

Hash_Map *hash_map_create(size_t size);
Hash_Map *hash_map_insert(Hash_Map *map, const char *key, void *value);
bool hash_map_has_key(Hash_Map *map, const char *key);
void *hash_map_at(Hash_Map *map, const char *key);
bool hash_map_remove(Hash_Map *map, const char *key);
void hash_map_print(Hash_Map *map, void (*print_value)(void *));
void hash_map_free(Hash_Map *map, void (*free_value)(void *));

#endif
