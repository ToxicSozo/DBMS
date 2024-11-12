#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include <stdint.h>

typedef struct Node {
    void *data;
    struct Node *next;
} Node;

typedef struct List {
    Node *head;
    Node *tail;
    size_t size;
} List;

extern List *new_list();
extern void free_list(List *list);

extern void push_back(List *list, void *data);
extern void push_front(List *list, void *data);

extern void pop_front(List *list);

extern void print_list(List *list);

extern bool elemin_list(List *list, void *data);
extern void remove_by_value(List *list, void *data);

extern void *get_element_at(List *list, size_t index);

#endif