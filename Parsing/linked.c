/* Notes to Neil:
   1. Code does not include any Extension functions 
   2. Compiling with testlinked_s gives warning 
      "AddressSanitizer: 1472 byte(s) leaked in 46 allocation(s)"
      this is due to the  //lisp_copy tests//(line 99 - 115)
      which we only free l5 and l9 
*/

#include "general.h"
#include "specific.h"

//typedef struct lisp lisp;

//typedef int atomtype;

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

   //only returns the atomic element, not a list
   temp->choice = true;

   return temp;

}

//Returns a boolean depending if boolean if it is in a list or not
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

   lisp* p = (lisp*)ncalloc(1, sizeof(lisp));
   
   //head of the list
   lisp* head = p;
   lisp* temp1;
   lisp* temp2;

   //if the list is not NULL
   if(l != NULL){ 
      p->atomtype = l->atomtype;
   }else{
      return NULL;
   }

   //goes through the parent nodes
   while(l->cdr != NULL){

      p->cdr = (lisp*)ncalloc(1, sizeof(lisp));

      if(l->car != NULL){       
         
         p->car = (lisp*)ncalloc(1, sizeof(lisp)); 
         temp1 = l->car;
         temp2 = p->car;
         temp2->atomtype = temp1->atomtype;
      
         //goes through the nested lisps 
         while(temp1->cdr != NULL){

            temp2->cdr = (lisp*)ncalloc(1, sizeof(lisp));            
            temp2->cdr->atomtype = temp1->cdr->atomtype;  

            temp2 = temp2->cdr;            
            temp1 = temp1->cdr;
            
         }
         
         //to take care of the last block of nested lisps
         temp2 = (lisp*)ncalloc(1, sizeof(lisp));
         temp2->atomtype = temp1->atomtype;
         temp2->cdr = NULL;

      }

      //move to the next position
      l = l->cdr;
      p = p->cdr;

      p->atomtype = l->atomtype; 

   }

   //set the last block to point to NULL
   p->cdr = NULL;

   return head;

}


//Returns a list containing the car as 'l1' and cdr as 'l2'
lisp* lisp_cons(const lisp* l1,  const lisp* l2){

   lisp* temp = (lisp*)ncalloc(1, sizeof(lisp));

   //checks if the lisps are null
   if(l1 == NULL && l2 != NULL){
      temp->car = NULL;
      temp->cdr = (lisp*) l2;

      return temp;
   }

   if(l2 == NULL){
      temp->cdr = NULL;
   }

   //check if the length of the lisps are > 1
   if(l1->cdr != NULL){

      //store l1 in temp->car
      temp->car = (lisp*) l1;
   
   //store only the atom value in the list;
   }else{
      
      temp->atomtype = l1->atomtype;
      
   }
   
   //assign temp->cdr with l2
   temp->cdr = (lisp*) l2;

   return temp;

}



//returns the number of components in the lisp
int lisp_length(const lisp* l){

   if(l == NULL || l->choice == true){
      return 0;
   }

   lisp* tracker = (lisp*) l;

   int counter = 0;

   while(tracker != NULL){

      counter++;
      tracker = tracker->cdr;

   }

   return counter;
}



// Returns the (1st) component of the list 'l'.
lisp* lisp_car(const lisp* l){

   lisp* temp = (lisp*)ncalloc(1, sizeof(lisp));

   //set the choice to true to extract the individual atom
   temp->choice = true;

   //store atomtype with value of atomtype from l
   //or store temp with car
   if(l->car == NULL){
      temp->atomtype = l->atomtype;   
   }else{
      temp = l->car; 
   }

   return temp; 
}

// Returns all but the 1st, component of the list 'l'.
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

   arr[*index] = LEFT_BRACKET; 
   (*index)++; 

   return;
}

//function to populate the left and right brackets
void populate_right_bracket(char* arr, int* index){

   arr[*index] = RIGHT_BRACKET; 
   (*index)++; 

   return;
}

//function to populate the left and right brackets
void populate_space(char* arr, int* index){

   arr[*index] = SPACE; 
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

   //check to store negative numbers between -1 to -9
   if(l->atomtype > -10 && l->atomtype < 0){
         
      arr[*index] = '-';
      (*index)++;

      //turn negative number to positive
      int val = l->atomtype * - 1; 

      arr[*index] = val + '0';
      (*index)++;  

   //store numbers lesser than -10
   }else if(l->atomtype <= -10){
      
      int counter = 0;
      int val = l->atomtype;
      int n = l->atomtype;
      int array[LISTSTRLEN];
      int in = 0;

      //counts the number of digits
      while(n != 0){
         n=n/10;
         counter++;
      }

      //stores the values inside a int temporary array in reverse order
      while (val != 0){
      
         array[in] = val % 10;
         array[in] = array[in] * -1;
         in++;
         val /= 10;
      
      }

         //include negetive sign in character string array 
         arr[*index] = '-';
         (*index)++; 

      //store values in character string array
      for(int i = counter - 1; i >= 0; i--){

         arr[*index] = array[i] + '0';
         (*index)++; 
      
      }

   //store nymbers greater than 10
   }else if(l->atomtype >= 10){
         
      int counter = 0;
      int val = l->atomtype;
      int n = l->atomtype;
      int array[LISTSTRLEN];
      int in = 0;

      //counts the number of digits
      while(n != 0){
         n=n/10;
         counter++;
      }

      while (val != 0){
      
         array[in] = val % 10;
         in++;
         val /= 10;
      
      }

      //store values in character string array
      for(int i = counter - 1; i >= 0; i--){

         arr[*index] = array[i] + '0';
         (*index)++; 

      }

   //numbers that are between 0 to 9
   }else{

         arr[*index] = l->atomtype + '0';
         (*index)++;

   }
}

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

            if(arr[index - 1] != SPACE){
               populate_space(arr, &index);
      
            }

            //last cons block pointing to NULL
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
   //(1 2 (3 4))
   //last cons block pointing to NULL
   if(l->car != NULL){         
      populate_left_bracket(arr, &index);
      temp = l->car;

      while(temp->cdr != NULL){
         populate_list(temp, arr, &index);             
         populate_space(arr, &index);

         temp = temp->cdr;
      
      }

      if(arr[index - 1] != SPACE){
         populate_space(arr, &index);

      }

      //last cons block pointing to NULL
      populate_list(temp, arr, &index);
      populate_right_bracket(arr, &index);
      populate_space(arr, &index);
      index--;

   }
   else
   {
      populate_list(l, arr, &index);
   }
   populate_right_bracket(arr, &index);

   arr[index] = '\0';

   strcpy(str, arr);

   return;
}


// Clears up all space used
void lisp_free(lisp** l){

   lisp* head;
   lisp* body;
   head = *l;

   //checks if the lisp passed is not NULL
   if(head != NULL){
      body = head->cdr;
   
   }else{
      
      return;
   }

   lisp* temp1;
   lisp* temp2;

   //traverse through the lisp 
   while(head->cdr != NULL){

      if(head->car != NULL){
         
         temp1 = head->car;
         if(temp1->cdr != NULL){
            temp2 = temp1->cdr;
         }

         //check for nested lisps and free values
         while(temp1->cdr != NULL){
            
            free(temp1);
            temp1 = temp2;
            temp2 = temp2->cdr;
            
         }

         temp1 = NULL;

         //free last block of the nested list
         free(temp1);

      }

      //free the next cdr blocks
      free(head);
      head = body;

      if(body != NULL){
         body = body->cdr;
      }

   }
   
   //last block of the parent struct
   free(head);
   *l = NULL;
   
}



void append(char* s,char c){

   int len = strlen(s);
   s[len] = c;
   s[len + 1] = '\0';
   
}

lisp* lisp_ans(lisp* temp_lisp, const char* str,int index)
{
    static int inc=0;
    
    lisp* p;
    if (temp_lisp == NULL) p=NULL;
    else p=temp_lisp;

   while(str[index] != ')'){
      
      if(str[index] == '('){
            index++;
            //use a recursive function to go through the list
            //and store the values inside this function
            // lisp* new=(lisp*)ncalloc(1, sizeof(lisp));
            if (p==NULL)
            {
               temp_lisp=(lisp*)ncalloc(1, sizeof(lisp));
               p=temp_lisp;
            }
            else
            {
               p=temp_lisp;
               while (p->cdr!=NULL) p=p->cdr;
               p->cdr=(lisp*)ncalloc(1, sizeof(lisp));
               p=p->cdr;
            }
            
            p->car = lisp_ans(NULL, str, index);  
            p->cdr=NULL;
            index=inc;
             
            index++;
      }

        else
        {
            if (str[index] == ' ')
            {
               index++;
               // if (str[index+1] == ')')  index--;
            }
            else{
                char c[20] ="";
               //  if (str[index]=='-')
               //  {
               //    index=index+1;

               //    index=index+2;
               //  }
                while (str[index]=='-' || isdigit(str[index]))
                {
                     append(c,str[index]);
                     index++;
                }
               
                if (p==NULL) {
                    temp_lisp=(lisp*)ncalloc(1, sizeof(lisp));
                    p=temp_lisp;
                    p->atomtype=atoi(c);
            
                }
                else{
                    p->cdr=(lisp*)ncalloc(1, sizeof(lisp));
                    p->cdr->atomtype= atoi(c);
                    p=p->cdr;
                    
                }
            }
            
            
        }
        if (str[index] == ')')
      {
        inc=index;
      }

      }
      if (p) p->cdr=NULL;
      index++;

      return temp_lisp;

   }


lisp* lisp_fromstring(const char* str){
//    char inp[4][LISTSTRLEN] = {"()", "(1)", "(0 (1 -2) 3 4 50)", "((1 2) (3 4) (5 (6 7)))"};

    // lisp* temp_lisp = (lisp*)ncalloc(1, sizeof(lisp));
    int index=1;
    return lisp_ans(NULL, str, index);

}




void test(){

   char str_test_empty[LISTSTRLEN];
   char str_test_list[LISTSTRLEN];

   lisp* t1 = lisp_cons(lisp_atom(-50), NULL);
   assert(lisp_isatomic(t1) == false);
   assert(lisp_getval(t1) == -50);
   assert((lisp_length(t1) == 1));
   lisp_tostring(t1,str_test_list);
   assert(strcmp(str_test_list, "(-50)")==0);

   lisp_free(&t1);   

   empty_list(str_test_empty);
   assert(strcmp(str_test_empty, "()")==0);  

   //testing get_val function and isatomic function
   lisp* t2 = lisp_cons(lisp_atom(-2), NULL);
   lisp* t3 = lisp_cons(lisp_atom(1), t2);
   lisp* t4 = lisp_cons(lisp_atom(3), lisp_cons(lisp_atom(4), lisp_cons(lisp_atom(-500), NULL)));
   lisp* t5 = lisp_cons(t3, t4);
   lisp* t6 = lisp_cons(lisp_atom(0), t5);

   assert(lisp_getval(lisp_car(t6)) == 0);
   assert(lisp_getval(lisp_car(lisp_car(lisp_cdr(t6)))) == 1);
   assert(lisp_getval(lisp_car(lisp_cdr(lisp_car(lisp_cdr(t6))))) == -2);
   assert(lisp_getval(lisp_car(lisp_cdr(lisp_cdr(t6)))) == 3);
   assert(lisp_getval(lisp_car(lisp_cdr(lisp_cdr(lisp_cdr(t6))))) == 4);
   assert(lisp_getval(lisp_car(lisp_cdr(lisp_cdr(lisp_cdr(lisp_cdr(t6)))))) == -500);
   lisp_tostring(t6,str_test_list);
   assert(strcmp(str_test_list, "(0 (1 -2) 3 4 -500)")==0);
   
   assert(lisp_isatomic(t2) != true);
   assert(lisp_isatomic(lisp_car(t3)) == true);
   assert(lisp_isatomic(lisp_cdr(t4)) == false);

   //if CAR is NULL, value return's will be 0;
   lisp* t7 = lisp_cons(NULL, lisp_atom(2));
   assert(lisp_getval(t7) == 0);

   //free memory allocation
   lisp_free(&t6);
   assert(!t6);
   lisp_free(&t1);
   assert(!t1);
   lisp_free(&t2);
   assert(!t2);
   

   lisp_free(&t7);
   assert(!t7);

}

