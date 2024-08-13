#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include "../memlib.h"

typedef struct Node{
    Object val;
    char *name;
    struct Node *next;
}Node;

#endif