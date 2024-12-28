#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/bucket.h"

bucket* new_bucket(vtype_buket key_type, vtype_buket value_type) {
    // Выделяем память для нового бакета
    bucket *bkt = (bucket *)malloc(sizeof(bucket));
    if (!bkt) {
        return NULL; // Не удалось выделить память
    }

    // Инициализация
    memset(bkt, 0, sizeof(bucket)); // Обнуляем все поля

    // Устанавливаем типы для ключа и значения
    bkt->key_type = key_type;
    bkt->value_type = value_type;
    bkt->overflow = NULL; // Нет переполнения по умолчанию

    return bkt;
}

bool is_cell_empty(uint8_t val) {
    return val == 0; // Предположим, что пустая ячейка имеет значение 0
}

bool get(bucket *bkt, value_buket key, uint8_t top_hash, value_buket *out_value) {
    if (!bkt) return false;

    bucket *current_bkt = bkt;

    // Проходим по всем бакетам, пока не найдем нужный
    while (current_bkt != NULL) {
        for (int i = 0; i < BUCKET_SIZE; i++) {
            if (current_bkt->tophash[i] == top_hash && !is_cell_empty(current_bkt->tophash[i])) {
                // Сравниваем ключи в зависимости от типа
                if (current_bkt->key_type == DECIMAL_ELEM && current_bkt->keys[i].decimal == key.decimal) {
                    *out_value = current_bkt->values[i];
                    return true;
                }
                if (current_bkt->key_type == REAL_ELEM && current_bkt->keys[i].real == key.real) {
                    *out_value = current_bkt->values[i];
                    return true;
                }
                if (current_bkt->key_type == STRING_ELEM && strcmp((char *)current_bkt->keys[i].string, (char *)key.string) == 0) {
                    *out_value = current_bkt->values[i];
                    return true;
                }
            }
        }
        current_bkt = current_bkt->overflow; // Переходим к следующему бакету в цепочке
    }

    return false; // Если ключ не найден
}

bool put(bucket *bkt, value_buket key, value_buket value, uint8_t top_hash) {
    if (!bkt) return false;

    bucket *current_bkt = bkt;

    while (current_bkt != NULL) {
        for (int i = 0; i < BUCKET_SIZE; i++) {
            if (current_bkt->tophash[i] == 0 || is_cell_empty(current_bkt->tophash[i])) {
                current_bkt->tophash[i] = top_hash;
                current_bkt->keys[i] = key;
                current_bkt->values[i] = value;
                return true;
            } else if (current_bkt->tophash[i] == top_hash) {
                current_bkt->values[i] = value;
                return true;
            }
        }

        if (current_bkt->overflow == NULL) {
            current_bkt->overflow = new_bucket(bkt->key_type, bkt->value_type);
            current_bkt = current_bkt->overflow;
        } else {
            current_bkt = current_bkt->overflow;
        }
    }

    return false;
}

void debug(bucket *bkt) {
    printf("bucket[");
    for (int i = 0; i < BUCKET_SIZE; i++) {
        if (bkt->key_type == DECIMAL_ELEM) {
            printf("%lld:%lld ", bkt->keys[i].decimal, bkt->values[i].decimal);
        } else if (bkt->key_type == REAL_ELEM) {
            printf("%f:%f ", bkt->keys[i].real, bkt->values[i].real);
        } else if (bkt->key_type == STRING_ELEM) {
            printf("\"%s\":\"%s\" ", bkt->keys[i].string, bkt->values[i].string);
        }
    }
    printf("]\n");
}