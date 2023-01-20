/* Notes to Neil:
   1. Code does not include any Extension functions 
   2. Compiling with testlinked_s gives warning 
      "AddressSanitizer: 1472 byte(s) leaked in 46 allocation(s)"
      this is due to the  //lisp_copy tests//(line 99 - 115)
      which we only free l5 and l9 
*/


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <ctype.h>

#define LISTSTRLEN 1000
#define LISPIMPL "Linked"
#define LEFT_BRACKET '('
#define RIGHT_BRACKET ')'
#define SPACE ' '
#define TEN 10
#define ZERO 0

struct lisp{    

    int atomtype;
    struct lisp* car;
    struct lisp* cdr;
    bool choice;

};

typedef struct lisp lisp;

typedef int atomtype;


// Returns element 'a' - this is not a list, and
// by itelf would be printed as e.g. "3", and not "(3)"
lisp* lisp_atom(const atomtype a);

// Returns a list containing the car as 'l1'
// and the cdr as 'l2'- data in 'l1' and 'l2' are reused,
// and not copied. Either 'l1' and/or 'l2' can be NULL
lisp* lisp_cons(const lisp* l1,  const lisp* l2);

// Returns a boolean depending up whether l points to an atom (not a list)
bool lisp_isatomic(const lisp* l);

// Returns the car (1st) component of the list 'l'.
// Does not copy any data.
lisp* lisp_car(const lisp* l);

// Returns the cdr (all but the 1st) component of the list 'l'.
// Does not copy any data.
lisp* lisp_cdr(const lisp* l);

// Returns the data/value stored in the cons 'l'
atomtype lisp_getval(const lisp* l);

// Returns a deep copy of the list 'l'
lisp* lisp_copy(const lisp* l);

// Returns number of components in the list.
int lisp_length(const lisp* l);

// Returns stringified version of list
void lisp_tostring(const lisp* l, char* str);

//frees allocated space for lisps
void lisp_free(lisp** l);

//populates character string array with values from lisp 
void populate_list(const struct lisp* l, char* arr, int* index);

//populates an empty lisp
void empty_list(char* arr);

//populates space in character string array
void populate_space(char* arr, int* index);

//populates right bracket in character string array
void populate_right_bracket(char* arr, int* index);

//populates left bracket in character string array
void populate_left_bracket(char* arr, int* index);

void append(char* s,char c);

struct lisp* lisp_fromstring(const char* str);

struct lisp* lisp_ans(struct lisp* temp_lisp, const char* str, int index);
