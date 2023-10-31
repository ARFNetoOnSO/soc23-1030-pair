At this point you have some good answers as for why you have a memory leak, but I will leave here an example of a way of *creating* this type of things that can be useful. Or ignored. :) 

**From the original code:**    

I changed `element1` for `el1` for clarity. Also `el2`.

```C
    my_list.pairs[0].el1 =  (struct element*)malloc(sizeof(struct element));
    my_list.pairs[0].el2 =  (struct element*)malloc(sizeof(struct element));
    my_list.pairs[0].el1 = &el1;
    my_list.pairs[0].el2 = &el2;
```

After reordering the lines:

```C
    my_list.pairs[0].el1 =  (struct element*)malloc(sizeof(struct element));
    my_list.pairs[0].el1 = &el1;

    my_list.pairs[0].el2 =  (struct element*)malloc(sizeof(struct element));
    my_list.pairs[0].el2 = &el2;
```

And it makes clear that as soon the 2nd and 4th line runs each area returned from `malloc` is gone for good, since in each case the single pointer for the area was overwritten.

Also

```C
    // declare variable my_list
    struct list my_list;
```

Maybe you could avoid this type of comment. It is clear what the declaration does.

## The example ##

We have here a list of pairs. Could be implemented as an array, as a linked list, as a dictionary... [*java*  has `Pair` as of Key and Value things](https://docs.oracle.com/javase%2F9%2Fdocs%2Fapi%2F%2F/javafx/util/Pair.html), and [*C++* has `pair` of `first` and `second` in *std::pair*](https://en.cppreference.com/w/cpp/utility/pair) as examples.

I will show you a way of writing this, using encapsulation, and viewing your pairs and list more like objects. It can be more productive doing so as it uses a lot of copy and paste and simply reuses things from these other languages and for the other functions here.

## The hierarchy: a list of elements in pairs ##

We will use `List` like a simple array of pointers, of fixed capacity. It can be implemented in any other way, like using a linked list or a simple array. But a list is just it: **a (possibly empty) collection of things**. In this case, pairs of something. For the `List` it makes no difference what *something* is. This is in essence *encapsulation*.

### The list ###    

```C
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
```

So a list has inside it the values for its actual size and capacity. The functions, called *methods* in other languages, makes no reference to elements. Even the `Pair` could have been abstracted, using a generic name line `Info` or `Node`.
  - `make_list` returns a pointer to a new empty list with the requested capacity
  - `delete_list` destroys a list and return `NULL`. Why? This is so we can delete a `List` and invalidate its pointer at the same time, for safety.
  - `insert_list` does the expected: inserts a record into a list. In this case a `Pair`
  - `show_list` shows on-screen the elements of the list. To save a few `printf` calls a prefix message can be added.

  This is like a constructor, a destructor and a *toString* or *serialize* method in other places/languages. Only pointers are used. It is easier.

### The pair ###

```C
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
```

Here we have the `C++` naming for pairs: elements are `first` and `second`

  - `make_pair` returns a `Pair` with the provided elements.
  - `show_pair` does as expected.
  - `delete_pair` frees a pair and return `NULL`.

  These are just copied from `List` and adapted. Only pointers are used.
  
### The elements, at last ###

Here is where things are customized. I will use 2 `Element`:

```C
typedef struct
{
    int i;
} Element;
```

and

```C
typedef struct
{
    char* name;
    int   code;
} Element;
```

The first is the same as in the question. The second is just to show that an `Element` can allocate memory, can have pointers inside, so we can not just copy its value inside a `Pair` and then inside a `List`.

So in order to `List` and `Pair` could handle such contents the user needs to provide the methods. Doing so `List` and `Pair` go unchanged from program to program like in other languages.

This is what we do in `qsort` providing the compare function, so the code can sort anything.

**The functions needed for `Element`**    

```C
Element* copy_el(Element*);
Element* delete_el(Element*);
int      show_el(Element*, const char*);

Element* make_el(const int); // helper for the example only
```

It is almost the same as for the other *"classes"*:    

  - `copy_el` is the *copy constructor*, and it is all we need to insert an element in a `List`: it returns the address of a copy of the element, so it is owned by the list and can be inserted at a `Pair`
  - `delete_el` is the destructor, since the `Element` can have complex allocation issues.
  - `show_el` is a method to display on-screen an `Element`
  
  ### an example using `int` ###
```C
#include <stdio.h>
#include <stdlib.h>
#include "list.h"

int main(void)
{
    Element* one     = make_el(42);
    Element* other   = make_el(-42);
    Pair*    my_pair = make_pair(one, other);
    delete_el(one);
    delete_el(other);

    show_pair(my_pair, "test_pair is ");
    List* my_list = make_list(5);  // capacity = 5
    show_list(my_list, "\n(list still empty) ");
    while (0 == insert_list(my_pair, my_list)) {};
    show_list(
        my_list, "\n(list now filled with same pair) ");
    my_pair = delete_pair(my_pair);  // free test_pair
    my_list = delete_list(my_list);  // free list
    return 0;
}
```

- `make_el` is just a helper to create an `Element` in these examples. The container only needs to know how to copy one, not how to create one.

This program    

  - creates a `Pair`
  - creates a '` List` of 5 elements
  - fills the list with this pair, since the contents are irrelevant here.
  - shows the list on-screen
  - `free`'s everithing
  
  ### output ###

```none
test_pair is   [42,-42]

(list still empty) [0/5] element(s) in list


(list now filled with same pair) [5/5] element(s) in list
  [42,-42]
  [42,-42]
  [42,-42]
  [42,-42]
  [42,-42]


    deleting 5 elements list
```


### a `List` using a `struct` that allocates memory ###

Now for this `Element` 
```C
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
int      show_el(Element*, const char*);

Element* make_el(const char*, const int);  // helper
#endif  // EL_H
```

Here `make_el` is of course different, but this is just for the example. `List` does not see this and `Pair` does not create elements, just copy them. Sure the user of a `List` of `MagicStuff` has some items to manage, and knows how to do it. As long as `copy_el` works, it is ok to 'List'. As long as `show_el` works `List` can do their job.

In fact pointers to these functions could be added to `List` and we would have something closer to *C++*.

### `main` for this case ###
```C
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
```

Only the first five lines are different, since we need to build a pair of elements. The rest of the code is the same. The more important is the idea that the code for `Pair` and `List` is the same.

### outout for this example ###
```none
test_pair is   [ ("Stack",42), ("Overflow",-42)]

(list still empty) [0/5] element(s) in list


(list now filled with same pair [5/5] element(s) in list
  [ ("Stack",42), ("Overflow",-42)]
  [ ("Stack",42), ("Overflow",-42)]
  [ ("Stack",42), ("Overflow",-42)]
  [ ("Stack",42), ("Overflow",-42)]
  [ ("Stack",42), ("Overflow",-42)]


    deleting 5 elements list
```

### example code for `Pair` ###

```C
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
```

### example code for `List` ###
```C
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
```

### simple implementations for `Element` ### 

A simple one using just `int`. In `C++` this is called *trivially_copyable* 

```C
typedef struct
{
    int i;
} Element;

Element* copy_el(Element*);
Element* delete_el(Element*);
Element* make_el(const int);
int      show_el(Element*, const char*);


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
```

**This is an example that has a pointer inside and allocates memory. It can be used the same way since the user provides a *copy constructor*.** 

```C
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
```


### A note on *encapsulation* ###

Here in `insert_list` a pair is inserted
```C
    Pair* pair       = make_pair(p->first, p->second);
``` 

but the code to do it come from `Pair` implementation.

But there in `make_pair`    

```C
    one->first  = copy_el(a);
    one->second = copy_el(b);
```
We see that the user-provided copy creator is used.

### a note on building this ###

  - Each level of *container* adds in general a pair of files, a `.h` header and the implementation file `.c`.
  - Each level sees only the one below and the code is just copied and adapted
  - Is is normal to have a constructor, a destructor, a copy constructor, a print function, declared as here or following some pattern. It works fine in general, since is what is done in other fields and languages.

  code and a visual studio project are available at [GitHub on this link](https://github.com/ARFNetoOnSO/soc23-1030-pair)



















