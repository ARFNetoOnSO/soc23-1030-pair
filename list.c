#include "list.h"

List* make_list(size_t size)
{
    List* one = malloc(sizeof(List));
    if (one == NULL) return NULL;
    one->limit = size;
    one->size  = 0;
    one->pair  = (Pair**)malloc(size * sizeof(Pair*));
    if (one->pair == NULL)
    {
        free(one);
        return NULL;
    }
    return one;
}

List* delete_list(List* L)
{
    if (L == NULL) return NULL;
    fprintf(
        stderr, "\n    deleting %llu elements list\n",
        L->size);
    for (size_t i = 0; i < L->size; i += 1)
        delete_pair(L->pair[i]);
    free(L);
    return NULL;
}

int insert_list(Pair* p, List* l)
{
    if (p == NULL) return -1;
    if (l == NULL) return -2;
    if (l->size == l->limit) return -3;
    Pair* pair       = make_pair(p->first, p->second);
    l->pair[l->size] = pair;
    l->size += 1;
    return 0;
}

int show_list(List* L, const char* msg)
{
    if (L == NULL) return -1;
    if (msg != NULL) printf("%s", msg);
    printf(
        "[%llu/%llu] element(s) in list\n", L->size,
        L->limit);
    for (size_t i = 0; i < L->size; i += 1)
        show_pair(L->pair[i], NULL);
    printf("\n");
    return 0;
}
