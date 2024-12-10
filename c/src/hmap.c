#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#include "../include/hmap.h"

static size_t hash(const char *key) {
    size_t hash = 0;
    size_t prime = 31;

    while (*key) {
        hash = hash * prime + (unsigned char)(*key);
        key++;
    }

    return hash;
}

static Hash_Map *hash_map_expand(Hash_Map *map) {
    assert(map != NULL);
    Hash_Map *expanded = hash_map_create(map->size * 2);

    for (size_t idx = 0; idx < map->size; idx++) {
        if (map->entries[idx].key != NULL) {
            expanded = hash_map_insert(expanded, map->entries[idx].key, map->entries[idx].value);
        }
    }
    free(map->entries);
    free(map);

    return expanded;
}

Hash_Map *hash_map_create(size_t size) {
    Hash_Map *map = malloc(sizeof(Hash_Map));
    assert(map != NULL);

    map->size = size;
    map->entries = calloc(map->size, sizeof(Hash_Map_Entry));
    assert(map->entries != NULL);

    return map;
}

Hash_Map *hash_map_insert(Hash_Map *map, const char *key, void *value) {
    assert(map != NULL);
    assert(key != NULL);

    size_t idx = hash(key) % map->size;

    while (map->entries[idx].key != NULL) {
        idx = (idx + 1) % map->size;

        if (idx == map->size) {
            return hash_map_insert(hash_map_expand(map), key, value);
        }
    }

    map->entries[idx].key = strdup(key);
    map->entries[idx].value = value;

    return map;
}

bool hash_map_has_key(Hash_Map *map, const char *key) {
    assert(map != NULL);
    assert(key != NULL);

    for (size_t idx = hash(key) % map->size; idx < map->size; idx++) {
        char *current = map->entries[idx].key;

        if (current == NULL) continue;

        if (!strcmp(current, key)) return true;
    }

    return false;
}

void *hash_map_at(Hash_Map *map, const char *key) {
    assert(map != NULL);
    assert(key != NULL);

    for (size_t idx = hash(key) % map->size; idx < map->size; idx++) {
        char *current = map->entries[idx].key;

        if (current == NULL) continue;

        if (!strcmp(current, key)) return map->entries[idx].value;
    }

    return NULL;
}

bool hash_map_remove(Hash_Map *map, const char *key) {
    assert(map != NULL);
    assert(key != NULL);

    size_t idx = hash(key) % map->size;

    while (map->entries[idx].key != NULL) {
        if (strcmp(map->entries[idx].key, key) == 0) {
            free(map->entries[idx].key);
            map->entries[idx].key = NULL;
            map->entries[idx].value = NULL;
            return true;
        }

        idx = (idx + 1) % map->size;
    }

    return false;
}

void hash_map_print(Hash_Map *map, void (*print_value)(void *)) {
    assert(map != NULL);
    
    printf("Hash Map Contents:\n");
    for (size_t idx = 0; idx < map->size; idx++) {
        if (map->entries[idx].key != NULL) {
            printf("Key: %s, Value: ", map->entries[idx].key);

            printf("\n");
        }
    }
}

void hash_map_free(Hash_Map *map, void (*free_value)(void *)) {
    assert(map != NULL);

    for (size_t idx = 0; idx < map->size; idx++) {
        if (map->entries[idx].key != NULL) {
            free(map->entries[idx].key);
            if (free_value) {
                free_value(map->entries[idx].value);
            }
        }
    }

    free(map->entries);
    free(map);
}