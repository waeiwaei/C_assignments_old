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

#define LISTSTRLEN 1000
#define LISPIMPL "Linked"
#define LEFT_BRACKET '('
#define RIGHT_BRACKET ')'
#define SPACE ' '


struct lisp{    

    int atomtype;
    struct lisp* car;
    struct lisp* cdr;
    bool choice;

};

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

