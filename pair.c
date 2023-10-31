#include "pair.h"

Pair* delete_pair(Pair* gone)
{
    if (gone == NULL) return NULL;
    delete_el(gone->first);
    delete_el(gone->second);
    free(gone);
    return NULL;
}

Pair* make_pair(Element* a, Element* b)
{
    if (a == NULL) return NULL;
    if (b == NULL) return NULL;
    Pair* one = malloc(sizeof(Pair));
    if (one == NULL) return NULL;
    one->first  = copy_el(a);
    one->second = copy_el(b);
    return one;
}

int show_pair(Pair* p, const char* msg)
{
    if (p == NULL) return -1;
    if (msg != NULL) printf("%s", msg);
    printf("  [");
    show_el(p->first, NULL);
    printf(",");
    show_el(p->second, NULL);
    printf("]\n");
    return 0;
}
