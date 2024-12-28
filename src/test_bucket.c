#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/bucket.h"

int main() {
    // Создаем бакет с типами ключа DECIMAL_ELEM и значения STRING_ELEM
    bucket *bkt = new_bucket(DECIMAL_ELEM, STRING_ELEM);
    if (bkt == NULL) {
        printf("Failed to create a new bucket.\n");
        return -1;
    }

    // Пример значения для ключа
    value_buket key;
    key.decimal = 12345;

    // Пример значения для вставки
    value_buket value;
    value.string = (uint8_t *)"Hello, World!";

    // Вставляем ключ и значение в бакет
    uint8_t top_hash = 1; // Это значение должно быть вычислено хеш-функцией
    if (put(bkt, key, value, top_hash)) {
        printf("Successfully inserted the key-value pair.\n");
    } else {
        printf("Failed to insert the key-value pair.\n");
    }

    // Теперь извлекаем значение по ключу
    value_buket out_value;
    if (get(bkt, key, top_hash, &out_value)) {
        printf("Successfully retrieved value: %s\n", (char *)out_value.string);
    } else {
        printf("Failed to retrieve the value.\n");
    }

    // Освобождаем память, связанную с бакетом
    free(bkt);

    return 0;
}
