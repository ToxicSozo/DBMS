#ifndef BUCKET_H
#define BUCKET_H

#include <stdint.h>
#include <stdbool.h>

#define BUCKET_SIZE 8

typedef enum {
    DECIMAL_ELEM,
    REAL_ELEM,
    STRING_ELEM,

    ANY_ELEM
} vtype_buket;

typedef union {
    int64_t decimal;
    double real;
    uint8_t *string;

    void *any;
} value_buket;


typedef struct bucket {
    uint8_t tophash[BUCKET_SIZE];

    value_buket keys[BUCKET_SIZE];
    value_buket values[BUCKET_SIZE];

    vtype_buket key_type;
    vtype_buket value_type;

    struct bucket *overflow;
} bucket;

bucket* new_bucket(vtype_buket key_type, vtype_buket value_type);
bool is_cell_empty(uint8_t val);
bool get(bucket *bkt, value_buket key, uint8_t top_hash, value_buket *out_value);
bool put(bucket *bkt, value_buket key, value_buket value, uint8_t top_hash);

#endif