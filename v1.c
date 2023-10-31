#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

int main(void)
{
    Element* one     = make_el("Stack", 42);
    Element* other   = make_el("Overflow", -42);
    Pair*    my_pair = make_pair(one, other);
    delete_el(one);
    delete_el(other);
    show_pair(my_pair, "test_pair is ");

    List* my_list = make_list(5);  // capacity = 5
    show_list(my_list, "\n(list still empty) ");

    while (0 == insert_list(my_pair, my_list)) {};
    show_list(
        my_list, "\n(list now filled with same pair ");

    my_pair = delete_pair(my_pair);  // free test_pair
    my_list = delete_list(my_list);  // free list
    return 0;
}
