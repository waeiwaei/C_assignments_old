#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_LENGTH 10000

struct lisp{

    int atomtype;
    struct lisp* car;
    struct lisp* cdr;
    char list[MAX_LENGTH];
    bool choice;

};

typedef struct lisp lisp;


/*Function definitions*/
bool program(char* input);
void instructions(char* input, int* len_index);
void instruction(char* input, int* len_index);
bool functions(char* input);
bool variable(char value);
int length(char* input, int line);
lisp* ins_set(char* input, int* len_index);
bool ins_var(char* input, int* len_index);
void ins_list(char* input, int* len_index, lisp* temp);
bool ins_print(char* array);

//we will also need to read a file, and store the input in a string, and then find each word based on "\n"
int main(int argc, char* argv[]){

   char input[MAX_LENGTH];
   char c;

   /*read files from argv[1]*/
   if(argc == 0){
      printf("No file has been read\n");
      return 0;
   }else{
      FILE * fp = fopen(argv[1], "r");

      if(fp == NULL){
         printf("Error in opening file");
         return 0;
      }else{

         int val;
         c = fgetc(fp);

      /*Populate 1D string with input from file*/
      while(c != EOF){
         
         if(c =='\n'){
            c = fgetc(fp);
         }else{
            input[val] = c;
            val++;
            c = fgetc(fp);
         }

      }


      fclose(fp); 

      printf("%s\n\n", input);

      /*program function for parser and interpreter*/
      program(input);

      }
   }
   

   return 0;
}


bool program(char* input){

   //we check if the first character is a '(', then go into instructions
   if(input[0] == '('){

      int len_index = 1;
      //go into the instruction
      instructions(input, &len_index);
      
   }else{
      printf("Input does not conform to formal grammar");
      return false;
   }

   return false;
}


void instructions(char* input, int* len_index){

   //static int inc_len = 0;

   while(input[*len_index] != ')'){
      if(input[*len_index] == '('){
         (*len_index)++;
         instructions(input, len_index);
      }else if(input[*len_index] == ' '){
         (*len_index)++;
      }else{
         instruction(input, len_index);
      }

   }


}


//to read the first word after that list to determine the specific instruction
void instruction(char* input, int* len_index){
   
   static int flag = 0;
   char inst[MAX_LENGTH];
   int len = 0;
   lisp* val = (lisp*)calloc(1, sizeof(lisp));

   while(input[*len_index] != ')'){

   //unable to read the val lisp pointer variable inside PRINT condition
   printf("%s", val->list);

      if(input[*len_index] == '('){
         flag = 1;
         instructions(input, len_index);
      }

         //read the instructions given
         //1. SET & PRINT (IO)
         //2. CAR, CDR, CONS (LIST)
         //3. PLUS & LENGTH (INT)
         //4. LESS, GREATER, EQUAL (BOOL) 
         inst[len] = input[*len_index];

         //IO Functions
         if(strcmp(inst, "SET") == 0){
            printf("SET has been called\n");
            (*len_index)++;
            //pass in the rest of the string into the function
            val = ins_set(input, len_index);

         }else if(strcmp(inst, "PRINT") == 0){
            printf("PRINT has been called\n");
            // Unable to read val lisp pointer in PRINT condition
            //printf("%s", val->list);

            if(val != NULL){
               char temp_array[MAX_LENGTH];
               strcpy(temp_array, val->list);

               ins_print(temp_array);

            }else{

               //increase the index to point to the "
               (*len_index) = (*len_index) + 2;

               //string - "xxx" save in an array
               if(input[*len_index] == 34){
                 
                  (*len_index)++;
                  char str_input[MAX_LENGTH];
                  int index1 = 0;

                  while(input[*len_index] == 34){
                     str_input[index1] = input[*len_index];
                     index1++;
                     (*len_index)++;
                  }
                  
                  ins_print(str_input);
               }

            }

         //LIST Functions
         }else if(strcmp(inst, "CAR") == 0){
            printf("CAR has been called\n");

         }else if(strcmp(inst, "CDR") == 0){
            printf("CDR has been called\n");

         }else if(strcmp(inst, "CONS") == 0){
            printf("CONS has been called\n");

         //INT Functions
         }else if(strcmp(inst, "PLUS") == 0){
            printf("PLUS has been called\n");

         }else if(strcmp(inst, "LENGTH") == 0){
            printf("LENGTH has been called\n");

         //BOOL Functions
         }else if(strcmp(inst, "LESS") == 0){
            printf("LESS has been called\n");


         }else if(strcmp(inst, "GREATER") == 0){
            printf("GREATER has been called\n");


         }else if(strcmp(inst, "EQUAL") == 0){
            printf("EQUAL has been called\n");

         //error message - does not conform to grammar
         }

         len++;
         (*len_index)++;
   }

   if(input[*len_index] == ')'){
      if(flag == 0){
         (*len_index)++;
         return;
      }else{
      return;
      }
   } 

}


//<SET> ::= "SET" <VAR> <LIST>
lisp* ins_set(char* input, int* len_index){
   
   lisp* temp = (lisp*)calloc(1, sizeof(lisp));

   while(input[*len_index] != ')'){
      if(input[*len_index] == ' '){
         (*len_index)++;
      }

      if(ins_var(input, len_index) == true){
         (*len_index)++;
         //If it is a string literal
         ins_list(input, len_index, temp);

      //error message - input does not conform to grammar
      }else{
         printf("SET - Input does not conform to formal grammar\n");

      }

      (*len_index)++;
   }


   return temp;
}


bool ins_var(char* input, int* len_index){

   if(input[*len_index] >= 'A' && input[*len_index] <= 'Z'){
      (*len_index)++;
      return true;
   }else{
      return false;
   }

   return false;
}


//<LIST> ::= <VAR> | <LITERAL> | "NIL" | "(" <RETFUNC> ")"
void ins_list(char* input, int* len_index, lisp* temp){

char check_nil[MAX_LENGTH];
int index1 = 0, index2 = 0;

   // printf("Value - %c, len index - %i\n", input[*len_index], *len_index);

   if(input[*len_index] == 39){
      (*len_index)++;

      while(input[*len_index] != 39){
         temp->list[index1] = input[*len_index];
         index1++;
         (*len_index)++;
      }
   }

/*
char check_nil[MAX_LENGTH];
int index1 = 0, index2 = 0;

while(input[*len_index] != ')'){
   //check if it is a single quoted literal
   if(input[*len_index] == 39){
      (*len_index)++;

      while(input[*len_index] != 39){
         temp->list[index1] = input[*len_index];
         index1++;
         (*len_index)++;
      }

   }else{

      check_nil[index1] = input[*len_index];

      if(strcmp(check_nil, "NIL") == 0){
         strcpy(temp->list , "NIL");   
      }

   }

   index2++;
   (*len_index)++;

}
*/

}


//should print the values of the string   
bool ins_print(char* array){

   if(strlen(array) > 0){
      printf("%s", array);
      return true;
   }else{
      return false;
   }
}
