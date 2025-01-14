#include "list.h"

#include <stdlib.h>
#include <string.h>

List list_create(uint32_t type_size) {
    List l = malloc(sizeof(*l));
    l->type_size = type_size;
    l->size = 0;
    l->head = NULL;
    
    return l;
}

LNode list_node_create(uint32_t type_size, void *elem) {
    LNode node = malloc(sizeof(*node));
    node->elem = malloc(type_size);
    memcpy(node->elem, elem, type_size);

    return node;
}

LNode list_node_create_shallow(void *elem) {
    LNode node = malloc(sizeof(*node));
    node->elem = elem;

    return node;
}

void list_insert(List l, void *elem) {
    LNode new_node = list_node_create(l->type_size, elem);
    new_node->next = l->head;
    l->head = new_node;
    l->size++;
}

void list_insert_shallow(List l, void *elem) {
    LNode new_node = list_node_create_shallow(elem);
    new_node->next = l->head;
    l->head = new_node;
    l->size++;
}

void list_clear(List l) {
    LNode curr_node = l->head;

    while (curr_node != NULL) {
        LNode tmp = curr_node->next;
        free(curr_node->elem);
        free(curr_node);
        curr_node = tmp;
    }

    l->head = NULL;
}

void list_free(List l) {
    list_clear(l);
    free(l);
}
