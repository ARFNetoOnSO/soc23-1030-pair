#include "stdio.h"
#include "stdlib.h"

struct element
{
    int i;
};

struct pair
{
    struct element *element1, *element2;
};

struct list
{
    struct pair* pairs;
};

int main(void)
{
    // declare variable my_list
    struct list my_list;

    // set size of pairs array (one for the example)
    my_list.pairs =
        (struct pair*)malloc(sizeof(struct pair));

    // set element1 and element2 of pairs[0]
    struct element element1 = {.i = 1};
    struct element element2 = {.i = 2};
    my_list.pairs[0].element1 =
        (struct element*)malloc(sizeof(struct element));
    my_list.pairs[0].element2 =
        (struct element*)malloc(sizeof(struct element));
    my_list.pairs[0].element1 = &element1;
    my_list.pairs[0].element2 = &element2;

    // check element values
    printf(
        "elements are: %d %d\n",
        my_list.pairs[0].element1->i,
        my_list.pairs[0].element2->i);

    // free memory
    free(my_list.pairs);

    return 0;
}