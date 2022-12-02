#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>

#define LISTSTRLEN 1000


struct lisp{

    int atomtype;
    struct lisp* car;
    struct lisp* cdr;

};



