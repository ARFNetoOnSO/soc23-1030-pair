#ifndef EL_H
#define EL_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
    char* name;
    int   code;
} Element;

Element* copy_el(Element*);
Element* delete_el(Element*);
Element* make_el(const char*, const int);  // helper
int      show_el(Element*, const char*);
#endif  // EL_H