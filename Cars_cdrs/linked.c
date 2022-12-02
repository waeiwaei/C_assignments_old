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

    return temp;

}


lisp* lisp_cons(const lisp* l1,  const lisp* l2){

    //atom values
    lisp* temp = (lisp*)ncalloc(1, sizeof(lisp));

        temp->car = (lisp*)l1;        
        temp->cdr = (lisp*)l2;


    return temp;
}


//returns the number of components in the lisp
int lisp_length(const lisp* l){

    if(l == NULL){
        return 0;
    }

    //tracks the head and car leaf nodes
    lisp* head = (lisp*) l;

    lisp* tracker_car = l->car;
    lisp* tracker_cdr = l->cdr;    


    printf("Tracker %i \n", l->car->atomtype);
    printf("Head %i \n", head->cdr->car->atomtype);
    
    
    //counter for integers
    int counter = 0;


//FOR ASSERTION - LINE 57 (TESTLIP.C)
//we must check both car and cdr if null or not null
    //we should be pointing to the next cdr value
    
    //while the list i s given is not empty 
    while(tracker_car != NULL){

        if(tracker_car->atomtype >= 0){
            counter++;

            tracker_car = tracker_car->cdr;    
        }else{

            tracker_car = tracker_car->car;

        }

    }

    while(tracker_cdr != NULL){

        if(tracker_cdr->car != NULL){
            tracker_cdr = tracker_cdr->car;
        }   

        if(tracker_cdr->atomtype >= 0){
            counter++;

            tracker_cdr = tracker_cdr->cdr;
        }else{

            tracker_cdr = tracker_cdr->car;

        }

    }

        //not necessary for the count_length function - we only need to know the car and cdr nodes 
        //of the heads, not necessarily the leaf nodes . More useful for converting the list into string

        //possibly some kind of recursive function to check the car and cdr
     /*   if(tracker->car == NULL && tracker->cdr != NULL){

            tracker->car = tracker->cdr;

            if(tracker->car == NULL && tracker->cdr == NULL){

                break;

            }

        }*/

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

