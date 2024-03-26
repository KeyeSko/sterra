#include <stdio.h>
#include <stdlib.h>

typedef struct list_item {
    int *data;
    struct list_item *next;
    struct list_item *prev;
} list_item;


typedef struct list {
    int count;
    list_item *head;
    list_item *tail;
} list;

list * list_create();