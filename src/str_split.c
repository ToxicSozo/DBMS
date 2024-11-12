#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../include/list.h"

List* str_split(const char* s, const char* delim) {
    List* list = new_list(); // Создаем новый список
    if (list == NULL) {
        fprintf(stderr, "%s\n", "memory allocation failed");
        return NULL;
    }

    char* _s = (char*)s;
    const char* delim_start = delim;
    size_t delim_len = strlen(delim);

    while (*_s != '\0') {
        if (strncmp(_s, delim_start, delim_len) == 0) {
            _s += delim_len;
        } else {
            char* start = _s;
            while (*_s != '\0' && strncmp(_s, delim_start, delim_len) != 0) {
                _s++;
            }
            size_t len = _s - start;
            char* token = (char*)malloc((len + 1) * sizeof(char));
            if (token == NULL) {
                fprintf(stderr, "%s\n", "memory allocation failed");
                free_list(list);
                return NULL;
            }
            strncpy(token, start, len);
            token[len] = '\0';
            push_back(list, token); // Добавляем токен в список
        }
    }

    return list;
}