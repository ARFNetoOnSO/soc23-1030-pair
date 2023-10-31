#ifndef PAIR_H
#define PAIR_H
#include <stdio.h>
#include <stdlib.h>
#include "el1.h"

typedef struct
{
    Element* first;
    Element* second;
} Pair;

Pair* delete_pair(Pair*);
Pair* make_pair(Element*, Element*);
int   show_pair(Pair*, const char*);
#endif // PAIR_H