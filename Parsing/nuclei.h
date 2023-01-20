#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define ON_ERROR(S) {fprintf(stderr, "%s",S); \ exit(EXIT_FAILURE);}
#define MAX_LENGTH 1000

typedef int atomtype;

typedef struct lisp lisp;

typedef struct prog prog;


// Clears up all space used
// Double pointer allows function to set 'l' to NULL on success
void lisp_free(lisp** l);
bool program(prog* prog);
void white_space(prog* prog);
bool list_car(prog* prog);
bool list_cdr(prog* prog);
bool int_plus(prog* prog);
bool list_cons(prog* prog);
bool list_func(prog* prog);
bool bool_less(prog* prog);
bool bool_greater(prog* prog);
bool int_length(prog* prog);
bool bool_equal(prog* prog);
bool func(prog* prog);
bool in_out_func(prog* prog);
bool int_func(prog* prog);
bool bool_func(prog* prog);
bool loop_func(prog* prog);
bool if_func(prog* prog);
bool instructions(prog* prog);
bool instruction(prog* prog);
bool ins_set(prog* prog);
bool ins_list(prog* prog);
bool ins_var(prog* prog);
bool ins_literal(prog* prog);
bool ins_string(prog* prog);
bool ins_print(prog* prog);
void clear_string(prog* prog);
bool ret_func(prog* prog);
atomtype lisp_getval(const lisp* l);
lisp* lisp_fromstring(const char* str);
lisp* lisp_ans(lisp* temp_lisp, const char* str, int index);
void append(char* s,char c);




