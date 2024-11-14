#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/list.h"

List *new_list() {
    List *list = (List*)malloc(sizeof(List));
    if (list == NULL) {
        fprintf(stderr, "%s\n", "memory allocation failed");
        return NULL;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;

    return list;
}

void free_list(List *list) {
    if (list == NULL) {
        return;
    }

    Node *current = list->head;
    Node *next;

    while (current != NULL) {
        next = current->next;
        free(current->string);
        free(current);
        current = next;
    }

    free(list);
}

void push_back(List *list, uint8_t *string) {
    if (list == NULL) {
        fprintf(stderr, "%s\n", "list is null");
        return;
    }

    Node *new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        fprintf(stderr, "%s\n", "memory allocation failed");
        return;
    }

    new_node->string = strdup(string);
    new_node->next = NULL;

    if (list->tail == NULL) {
        list->head = new_node;
        list->tail = new_node;
    } else {
        list->tail->next = new_node;
        list->tail = new_node;
    }

    list->size++;
}

void push_front(List *list, uint8_t *string) {
    if (list == NULL) {
        fprintf(stderr, "%s\n", "list is null");
        return;
    }

    Node *new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        fprintf(stderr, "%s\n", "memory allocation failed");
        return;
    }

    new_node->string = strdup(string);
    new_node->next = list->head;

    if (list->head == NULL) {
        list->head = new_node;
        list->tail = new_node;
    } else {
        list->head = new_node;
    }

    list->size++;
}

void pop_back(List *list) {
    if (list == NULL || list->head == NULL) {
        fprintf(stderr, "List is empty, nothing to pop\n");
        return;
    }

    if (list->head == list->tail) {
        free(list->head->string);
        free(list->head);
        list->head = NULL;
        list->tail = NULL;
    } else {
        Node *current = list->head;
        while (current->next != list->tail) {
            current = current->next;
        }

        free(list->tail->string);
        free(list->tail);
        list->tail = current;
        list->tail->next = NULL;
    }

    list->size--;
}

void pop_front(List *list) {
    if (list == NULL || list->head == NULL) {
        fprintf(stderr, "List is empty, nothing to pop\n");
        return;
    }

    Node *old_head = list->head;
    list->head = list->head->next;
    free(old_head->string);
    free(old_head);

    if (list->head == NULL) {
        list->tail = NULL;
    }

    list->size--;
}

bool elemin_list(List *list, uint8_t *string) {
    if (list == NULL || list->head == NULL) {
        fprintf(stderr, "%s\n", "list is null or empty");
        return false;
    }

    Node *current = list->head;
    while (current != NULL) {
        if (strcmp(current->string, string) == 0) {
            return true;
        }
        current = current->next;
    }

    return false;
}
void remove_by_value(List *list, uint8_t *string) {
    if (list == NULL || list->head == NULL) {
        fprintf(stderr, "List is empty, nothing to remove\n");
        return;
    }

    Node *current = list->head;
    Node *previous = NULL;

    while (current != NULL) {
        if (strcmp((char*)current->string, (char*)string) == 0) {
            if (previous == NULL) {
                list->head = current->next;
                if (list->head == NULL) {
                    list->tail = NULL;
                }
            } else {
                previous->next = current->next;
                if (current->next == NULL) {
                    list->tail = previous;
                }
            }
            free(current->string);
            free(current);
            list->size--;
            return;
        }
        previous = current;
        current = current->next;
    }

    fprintf(stderr, "Value not found in the list\n");
}

uint8_t *get_element_at(List *list, size_t index) {
    if (list == NULL || index >= list->size) {
        fprintf(stderr, "Index out of bounds: %zu\n", index);
        return NULL;
    }

    Node *current = list->head;
    for (size_t i = 0; i < index; i++) {
        current = current->next;
    }

    return current->string;
}

void print_list(List *list) {
    if (list == NULL || list->head == NULL) {
        printf("[ ]\n");
        return;
    }

    printf("[ ");
    Node *current = list->head;
    while (current != NULL) {
        printf("'%s' ", current->string);
        current = current->next;
    }
    printf("]\n");
}