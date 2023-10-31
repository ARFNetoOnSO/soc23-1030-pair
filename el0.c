#include "el0.h"

Element* copy_el(Element* orig)
{
    if (orig == NULL) return NULL;
    Element* one = malloc(sizeof(Element));
    if (one == NULL) return NULL;
    one->i = orig->i;
    return one;
}

Element* delete_el(Element* orig)
{
    if (orig == NULL) return NULL;
    free(orig);
    return NULL;
}

Element* make_el(const int value)
{
    Element* one = malloc(sizeof(Element));
    if (one == NULL) return NULL;
    one->i = value;
    return one;
}

int show_el(Element* el, const char* msg)
{
    if (el == NULL) return -1;
    if (msg != NULL) printf("%s", msg);
    printf("%d", el->i);
    return 0;
}
