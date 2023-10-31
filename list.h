#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include "pair.h"

typedef struct
{
    size_t size;
    size_t limit;
    Pair** pair;
} List;

List* make_list(size_t);
List* delete_list(List*);
int   insert_list(Pair*, List*);
int   show_list(List*, const char*);

#endif // LIST_H
