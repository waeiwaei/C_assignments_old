//#pragma once

#include "general.h"
#include "specific.h"

typedef struct lisp lisp;

typedef int atomtype;

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>



// Returns element 'a' - this is not a list, and
// by itelf would be printed as e.g. "3", and not "(3)"
lisp* lisp_atom(const atomtype a){

    lisp* temp = (lisp*)ncalloc(1, sizeof(lisp));
    temp->atomtype = a;
    temp->car = NULL;
    temp->cdr = NULL; 

    return temp;

}

// Returns a deep copy of the list 'l'
lisp* lisp_copy(const lisp* l){

    lisp* temp = (lisp*)ncalloc(1, sizeof(lisp));

    temp->car = l->car;
    temp->cdr = l->cdr;
    temp->atomtype = l->atomtype;

    return temp;

}


lisp* lisp_cons(const lisp* l1,  const lisp* l2){

    lisp* temp = (lisp*)ncalloc(1, sizeof(lisp));
    
    if(l1->cdr != NULL){

        temp->car = (lisp*)l1;
    
    }else{

        temp->atomtype = l1->atomtype;
    
    }

    //check if l1->cdr is not NULL
        //we store the l1 to the temp->car: temp->car = (lisp*)l1;
    //else
    //temp->atomtype = l1->atomtype

        temp->cdr = (lisp*)l2;

    return temp;

}


//returns the number of components in the lisp
int lisp_length(const lisp* l){

    if(l == NULL){
        return 0;
    }
 
    lisp* tracker = (lisp*) l;

    //counter for integers
    int counter = 0;

    while(tracker != NULL){

        counter++;
        tracker = tracker->cdr;

    }

    return counter;
}






// Returns the car (1st) component of the list 'l'.
// Does not copy any data.
lisp* lisp_car(const lisp* l){

    lisp* temp = (lisp*)ncalloc(1, sizeof(lisp));

    temp->car = l->car;

    return temp; 
}



// Returns the cdr (all but the 1st) component of the list 'l'.
// Does not copy any data.
lisp* lisp_cdr(const lisp* l){

    lisp* temp = (lisp*)ncalloc(1, sizeof(lisp));

    temp->car = l->cdr;

    return temp;

}

