#ifndef EL_H
#define EL_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int i;
} Element;

Element* copy_el(Element*);
Element* delete_el(Element*);
Element* make_el(const int);
int      show_el(Element*, const char*);

#endif // EL_H