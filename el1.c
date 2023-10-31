#include "el1.h"

Element* copy_el(Element* orig)
{
    if (orig == NULL) return NULL;
    Element* one = malloc(sizeof(Element));
    if (one == NULL) return NULL;
    one->name = malloc(1 + strlen(orig->name));
    if (one->name == NULL)
    {
        free(one);
        return NULL;
    }
    strcpy(one->name, orig->name);
    one->code = orig->code;
    return one;
}

Element* delete_el(Element* orig)
{
    if (orig == NULL) return NULL;
    free(orig->name);
    free(orig);
    return NULL;
}

int show_el(Element* el, const char* msg)
{
    if (el == NULL) return -1;
    if (msg != NULL) printf("%s", msg);
    printf(" (\"%s\",%d)", el->name, el->code);
    return 0;
}

Element* make_el(const char* name, const int code)
{
    if (name == NULL) return NULL;
    Element* one = malloc(sizeof(Element));
    if (one == NULL) return NULL;
    one->name = malloc(1 + strlen(name));
    if (one->name == NULL)
    {
        free(one);
        return NULL;
    }
    strcpy(one->name, name);
    one->code = code;
    return one;
}