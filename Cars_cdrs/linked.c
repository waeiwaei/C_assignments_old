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

/*
memcpy(temp->car, l->car, sizeof(lisp));
memcpy(temp->cdr, l->cdr, sizeof(lisp));
memcpy(temp->atomtype, l->atomtype, sizeof(int));
*/


   temp->car = l->car;
   temp->cdr = l->cdr;
   temp->atomtype = l->atomtype;

   return temp;

}


lisp* lisp_cons(const lisp* l1,  const lisp* l2){

   lisp* temp = (lisp*)ncalloc(1, sizeof(lisp));

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

   temp->choice = true;

   if(l->car == NULL){
      temp->atomtype = l->atomtype;   
   }else{
      temp = l->car; 
   }

   return temp; 
}



// Returns the cdr (all but the 1st) component of the list 'l'.
// Does not copy any data.
lisp* lisp_cdr(const lisp* l){

   lisp* temp = (lisp*)ncalloc(1, sizeof(lisp));

   temp = l->cdr;

   return temp;

}

// Returns the data/value stored in the cons 'l'
atomtype lisp_getval(const lisp* l){

   int value = l->atomtype;

   return value;
}


//function to populate the left and right brackets
void populate_left_bracket(char* arr, int* index){

   arr[*index] = '('; 
   (*index)++; 

   return;

}

//function to populate the left and right brackets
void populate_right_bracket(char* arr, int* index){

   arr[*index] = ')'; 
   (*index)++; 

   return;

}

//function to populate the left and right brackets
void populate_space(char* arr, int* index){

   arr[*index] = ' '; 
   (*index)++; 

   return;

}


//returns empty list
void empty_list(char * arr){

   int index = 0;

   populate_left_bracket(arr, &index);
   populate_right_bracket(arr, &index);
   
   arr[index] = '\0';

   return;
}

/*
void populate_list(lisp* l, char* arr, int* index){

   arr[*index] = l->atomtype + '0'; 
   (*index)++; 
   
}
*/

// Returns stringified version of list
void lisp_tostring(const lisp* l, char* str){

   lisp* temp;
   int index = 0;
   char arr[LISTSTRLEN];

   //If list is empty, return "()" string
   if(l == NULL){

      empty_list(arr);

      strcpy(str, arr);
               
      return;

   }

   if(l->choice == true){
      
      arr[index] = l->atomtype + '0';
      index++;
      arr[index] = '\0';

      strcpy(str, arr);

      return;
   
   }

   //populate the char str with the list
   populate_left_bracket(arr, &index);

   while(l->cdr != NULL){
         
         if(l->car != NULL){
         
            populate_left_bracket(arr, &index);

            temp = l->car;
            while(temp->cdr != NULL){
                  
               arr[index] = temp->atomtype + '0';
               index++;

               //populate_list(temp, arr, &index);

               populate_space(arr, &index);

               temp = temp->cdr;
            }

            if(arr[index - 1] != ' '){
               populate_space(arr, &index);
            }

            //last cons block pointing to NULL
            arr[index] = temp->atomtype + '0';
            index++;

            //populate_list(temp, arr, &index);

            populate_right_bracket(arr, &index);

            populate_space(arr, &index);

         }else{
            
            arr[index] = l->atomtype + '0';
            index++;  
            
            //populate_list(l, arr, &index);
         
            populate_space(arr, &index);

         }

         l = l->cdr;
      }

      //last cons block pointing to NULL
      arr[index] = l->atomtype + '0';
      index++;

      //populate_list(l, arr, &index);

      populate_right_bracket(arr, &index);

      arr[index] = '\0';

      strcpy(str, arr);

      return;


}

// Clears up all space used
// Double pointer allows function to set 'l' to NULL on success
void lisp_free(lisp** l){

   *l = NULL;
   free(*l);

}



//function to store the new string values until the next '(' is reached 
lisp* nested(lisp* l, const char* str, int* index){

   lisp* tempo = (lisp*)ncalloc(1, sizeof(lisp));

   if(str[*index] == ')'){
      return NULL;
   }

   (*index)++;

   tempo = nested(l, str, index);

   return tempo;

}



lisp* lisp_fromstring(const char* str){

   lisp* temp_lisp = (lisp*)ncalloc(1, sizeof(lisp));
   int index = 0;

   while(str[index] != ')'){
      
      if(str[index] == '('){

         index++;

         while(str[index] != ')'){

            //use a recursive function to go through the list
            //and store the values inside this function
            temp_lisp->car = nested(temp_lisp, str, &index);
            
         }

      }

      temp_lisp->car = lisp_atom(atoi((&str[index])));

      index++;

   }

   return temp_lisp;
}

