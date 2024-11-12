#ifndef LIST_H
#define LIST_H


#include <stdbool.h>
#include <stdint.h>

typedef struct Node {
    uint8_t *string;
    struct Node *next;
} Node;

typedef struct List {
    Node *head;
    Node *tail;
    size_t size; // Добавляем поле для хранения размера списка
} List;

extern List *new_list();
extern void free_list(List *list);

extern void push_back(List *list, uint8_t *string);
extern void push_front(List *list, uint8_t *string);

extern void pop_front(List *list);

extern void print_list(List *list);

extern bool elemin_list(List *list, uint8_t *string);
extern void remove_by_value(List *list, uint8_t *string);

extern uint8_t *get_element_at(List *list, size_t index);

#endif