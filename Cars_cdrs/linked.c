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

   //only returns the atom element, not a list
   temp->choice = true;

   return temp;

}

// Returns a boolean depending up whether l points to an atom (not a list)
bool lisp_isatomic(const lisp* l){

   if(l == NULL){
      return false;
   }

   if(l->choice == true){

      return true;

   }

   return false;   

}



// Returns a deep copy of the list 'l'
lisp* lisp_copy(const lisp* l){
/*
   lisp* temp = (lisp*)ncalloc(1, sizeof(lisp));

   temp->car = l->car;
   temp->cdr = l->cdr;
   temp->atomtype = l->atomtype;
*/

   lisp* temp = (lisp*)ncalloc(1, sizeof(lisp));

   while(l->cdr != NULL && *head != NULL){

      if(*head->car != NULL){
         
         *temp1 = *head->car;
         *temp2 = *temp1->cdr;

         while(*temp1->cdr != NULL){
            
            free(*temp1);
            *temp1 = *temp2;
            *temp2 = *temp2->cdr;
            
         }

      }else{

         free(*head);

      }  

      *head = *body;
      *body = *body->cdr;
   }





   return temp;

}


lisp* lisp_cons(const lisp* l1,  const lisp* l2){

   lisp* temp = (lisp*)ncalloc(1, sizeof(lisp));

   //checks if the strings are null
   if(l1 == NULL && l2 != NULL){
      temp->car = NULL;
      temp->cdr = (lisp*) l2;

      return temp;
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

   if(l == NULL || l->choice == true){
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


void populate_list(const lisp* l, char* arr, int* index){

      //check to store negative numbers
      if(l->atomtype < 0){
                  
         arr[*index] = '-';
         (*index)++;

         //Get rid of "-" in the atom type
         int val = l->atomtype * - 1; 

         arr[*index] = val + '0';
         (*index)++;   

      }else if(l->atomtype >= 0){

         arr[*index] = l->atomtype + '0';
         (*index)++;

      }
}


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
      
      //check to store negative numbers
      if(l->atomtype < 0){
         
         populate_list(l, arr, &index);
 
         arr[index] = '\0';      

      }else if(l->atomtype >= 0){

         populate_list(l, arr, &index);

         arr[index] = '\0';

      }

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
               
               populate_list(temp, arr, &index);             
               populate_space(arr, &index);

            temp = temp->cdr;
            
            }

            if(arr[index - 1] != ' '){
               populate_space(arr, &index);
            }

            //last cons block pointing to NULL
            //check to store negative numbers
            populate_list(temp, arr, &index);
            populate_right_bracket(arr, &index);
            populate_space(arr, &index);

         }else{
            
            //check to store negative numbers
            populate_list(l, arr, &index);
            populate_space(arr, &index);

         }

         l = l->cdr;
      }


      //last cons block pointing to NULL
      //check to store negative numbers
      populate_list(l, arr, &index);
 
      populate_right_bracket(arr, &index);

      arr[index] = '\0';

      strcpy(str, arr);

      return;


}

// Clears up all space used
// Double pointer allows function to set 'l' to NULL on success
void lisp_free(lisp** l){

/*
   lisp** head, body;
   *head = *l;
   *body = (*head)->cdr;
   lisp** temp1, temp2;

   while(*head->cdr != NULL && *head != NULL){

      if(*head->car != NULL){
         
         *temp1 = *head->car;
         *temp2 = *temp1->cdr;

         while(*temp1->cdr != NULL){
            
            free(*temp1);
            *temp1 = *temp2;
            *temp2 = *temp2->cdr;
            
         }

      }else{

         free(*head);

      }  

      *head = *body;
      *body = *body->cdr;
   }

*/

/*

   *l = NULL;
   free(*l);
   
*/
}



//recursive function to itteratively go through each character in a string
lisp* lisp_ans(lisp* temp_lisp, const char* str, int* index){

   if(str[*index] == ')'){
      return NULL;
   }

   temp_lisp = (lisp*)ncalloc(1, sizeof(lisp));
    
   //tracker of the cons
   lisp* p;
   p = temp_lisp;

   while(str[*index] != ')'){

   printf("INDEX - %i  , Character - %c\n", *index, str[*index]);

      if(str[*index] == '('){

            (*index)++;
            //use a recursive function to go through the list
            //and store the values inside this function
            p->car = lisp_ans(NULL, str, index);
      }

      else{
         if(str[*index] == ' '){
            (*index)++;
         }

         else{

            int sign = 1;
            int number = 0;

            //check for two digit integers  

            number = atoi((&str[*index]));

            if(str[*index] == '-'){

               sign = -1;
               (*index)++;

               number = number * sign;

            }
            printf("Number - %d \n", number);

            printf("Value of P: %i\n\n", p->atomtype);

            if(p == NULL){

               p = lisp_atom(number);
               (*index)++;

            }else{

               p->cdr = lisp_atom(number);
               p = p->cdr;                  
               (*index)++;

            }
         }        
      }
   }


   p->cdr = NULL;

   (*index)++;

   return temp_lisp;

}


lisp* lisp_fromstring(const char* str){

   // lisp* temp_lisp = (lisp*)ncalloc(1, sizeof(lisp));
   int index = 1;
   return lisp_ans(NULL, str, &index);
   
}



void test(){

   int index1, index2, index3, index4;
   char str_test_empty[LISTSTRLEN];
   char str_test_left_bracket[LISTSTRLEN];
   char str_test_right_bracket[LISTSTRLEN];
   char str_test_space[LISTSTRLEN];
   char str_test_list[LISTSTRLEN];


   lisp* t1 = lisp_cons(lisp_atom(3), NULL);
   assert(lisp_isatomic(t1) == false);
   assert((lisp_length(t1) == 1));

   //self-created functions
   empty_list(str_test_empty);
   assert(strcmp(str_test_empty, "()")==0);


   populate_left_bracket(str_test_left_bracket, &index1);
   assert(strcmp(str_test_left_bracket, "(") == 0);

   populate_space(str_test_space, &index2);
   assert(strcmp(str_test_space, " ") == 0);

   populate_list(t1, str_test_list, &index3);
   assert(strcmp(str_test_list, "3") == 0);

   populate_right_bracket(str_test_right_bracket, &index4);
   assert(strcmp(str_test_right_bracket, ")") == 0);
  
   lisp_free(&t1);   


   //testing get_val function and isatomic function
   lisp* t2 = lisp_cons(lisp_atom(2), NULL);
   lisp* t3 = lisp_cons(lisp_atom(1), t2);
   lisp* t4 = lisp_cons(lisp_atom(3), lisp_cons(lisp_atom(4), lisp_cons(lisp_atom(5), NULL)));
   lisp* t5 = lisp_cons(t3, t4);
   lisp* t6 = lisp_cons(lisp_atom(0), t5);

   lisp* la =  lisp_car(t6);
   lisp* lb =  lisp_car(lisp_car(lisp_cdr(t6)));
   lisp* lc =  lisp_car(lisp_cdr(lisp_car(lisp_cdr(t6))));
   lisp* ld =  lisp_car(lisp_cdr(lisp_cdr(t6)));
   lisp* le =  lisp_car(lisp_cdr(lisp_cdr(lisp_cdr(t6))));
   lisp* lf =  lisp_car(lisp_cdr(lisp_cdr(lisp_cdr(lisp_cdr(t6)))));   

   assert(lisp_getval(la) == 0);
   assert(lisp_getval(lb) == 1);
   assert(lisp_getval(lc) == 2);
   assert(lisp_getval(ld) == 3);
   assert(lisp_getval(le) == 4);
   assert(lisp_getval(lf) == 5);

   assert(lisp_isatomic(t2) != true);
   assert(lisp_isatomic(lisp_car(t3)) == true);
   assert(lisp_isatomic(lisp_cdr(t4)) == false);

   //if NULL head is called, value return's will be 0;
   lisp* t7 = lisp_cons(NULL, t5);

   assert(lisp_getval(t7) == 0);

   //free memory allocation
   lisp_free(&t1);
   assert(!t1);
   lisp_free(&t2);
   assert(!t2);
   lisp_free(&t3);
   assert(!t3);
   lisp_free(&t4);
   assert(!t4);
   lisp_free(&t5);
   assert(!t5);
   lisp_free(&t6);
   assert(!t6);

   lisp_free(&la);
   assert(!la);
   lisp_free(&lb);
   assert(!lb);
   lisp_free(&lc);
   assert(!lc);
   lisp_free(&ld);
   assert(!ld);
   lisp_free(&le);
   assert(!le);
   lisp_free(&lf);
   assert(!lf);

   printf("Ended testing\n");

}
