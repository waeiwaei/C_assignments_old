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
    //temp->car = (lisp*)ncalloc(1, sizeof(lisp));
    //temp->cdr = (lisp*)ncalloc(1, sizeof(lisp));

    //checks if the strings are null
    if(l1 == NULL){
        temp->car = NULL;
    }

    if(l2 == NULL){
        temp->cdr = NULL;

    }

    //check if the length of the string is > 1
    if(l1->cdr != NULL){

        //store l1 in temp->car
        temp->car = (lisp*) l1;
    
    //store only the atom value in the list;
    }else{
        
        temp->atomtype = l1->atomtype;
        
    }
    
    temp->cdr = (lisp*) l2;

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

    temp = l->car;

    return temp; 
}



// Returns the cdr (all but the 1st) component of the list 'l'.
// Does not copy any data.
lisp* lisp_cdr(const lisp* l){

    lisp* temp = (lisp*)ncalloc(1, sizeof(lisp));

    temp = l->cdr;

    printf("tempcdr - %i \n", temp->atomtype);

    return temp;

}

// Returns the data/value stored in the cons 'l'
atomtype lisp_getval(const lisp* l){

    int value = 0;

    value = l->atomtype;

    return value;
}


// Returns stringified version of list
void lisp_tostring(const lisp* l, char* str){

    lisp* temp;
    int index = 0;
    int length = 1;
    char arr[LISTSTRLEN];

    //str = (char*)ncalloc(2, sizeof(char));

    //only printing a list with one cons
    if(l->cdr == NULL){
        if(l->car == NULL){

            arr[index] = l->atomtype + '0';

            index++;
            arr[index] = '\0';

            strcpy(str, arr);
            
            return;

        }
    }


    while(l->cdr != NULL){

        if(l->car != NULL){
        
            arr[index] = '('; 
            
            index++; 
            length++;  
        
            temp = l->car;            
            while(temp->cdr != NULL){
                
                //str = (char*)nremalloc(str, length * sizeof(char));
                arr[index] = temp->atomtype + '0';
                index++;
                //length++;  

                temp = temp->cdr;
            }

            arr[index] = ')';
            index++;
            //length++;

        }else{
            
            //str = (char*)nremalloc(str, length * sizeof(char));
            arr[index] = l->atomtype + '0';
            index++;
            //length++;

        }

        l = l->cdr;
    }

    //last cons block pointing to NULL
    arr[index] = l->atomtype + '0';
    index++;
    //length++;

    //str = (char*)nremalloc(str, length * sizeof(char));
    arr[index] = '\0';

    
    printf("%i  ", index);
    printf("%s", arr);

    strcpy(str, arr);


}
