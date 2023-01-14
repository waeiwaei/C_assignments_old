#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "specific.h"
#include "general.h"

#define ON_ERROR(S) {fprintf(stderr, "%s", S);\
                     exit(EXIT_FAILURE);}

#define MAX_LENGTH 1000
#define INTERPRET

bool list_func(char* input, int* len_index, char* instruct);
bool func(char* input, int* len_index, char* inst, lisp* temp);
bool program(char* input);
bool in_out_func(char* input, int* len_index, char* instruc, lisp* temp);
bool int_func(char* input, int* len_index, char* instruct, lisp* temp);
bool bool_func(char* input, int* len_index, char* instruct, lisp* temp);
bool if_func(char* input, int* len_index, lisp* temp);
bool loop_func(char* input, int* len_index, lisp* temp);
bool instructions(char* input, int* len_index, lisp* temp);
bool instruction(char* input, int* len_index, lisp* temp);
bool ins_set(char* input, int* len_index, lisp* temp);
bool ins_list(char* input, int* len_index, lisp* temp);
bool ins_var(char* input, int* len_index);
bool ins_literal(char* input, int* len_index, lisp* temp);
bool ins_string(char* input, int* len_index);
bool ins_print(char* array, int* len_index, lisp* temp);
void clear_string(char* input);
bool ret_func(char* input, int* len_index, lisp* temp);


//read file from argv, and store characters in 1D array
int main(int argc, char* argv[]){

   char input[MAX_LENGTH];
   char c;
   int val = 0;

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

      input[val] = '\0';

      fclose(fp); 

      /*program function for parser*/
      if(program(input)){
         printf("Parsed OK!");
      }

      }
   }
   
   return 0;
}


//<PROG> ::= "(" <INSTRCTS>
bool program(char* input){

   printf("%s\n\n", input);

   //create the pointer to a space, to store the lisp from the set function
   //we can then pass this to the PRINT function to print if it is a lisp
   //if this is empty, then we know that PRINT function will print a string
   lisp* temp = NULL;

   printf("Initialised: Program function temp address - %p \n\n", (void*)(temp));

   //we check if the first character is a '(', then go into instructions
   if(input[0] == '('){

      int len_index = 1;
      //go into the instructions function

      if(instructions(input, &len_index, temp)){
         return true;
      }else{
         return false;
      }
   }
   
   ON_ERROR("Expression does not have an opening bracket\n");
   return false;

}


//<INSTRCTS> ::= <INSTRCT> <INSTRCTS> | ")"
bool instructions(char* input, int* len_index, lisp* temp){

   printf("Instructions - %i, %c \n", *len_index, input[*len_index]);

   printf("INSTRUCTIONS function temp address - %p \n\n", (void*)(temp));

   while(input[*len_index] == ' '){ 
      (*len_index)++;
   }

      if(input[*len_index] == '('){
         (*len_index)++;

         if(!instruction(input, len_index, temp)){            
            return false;
         }

         printf("This is the LEN index here - %i \n\n", *len_index);
   
      if(!instructions(input, len_index, temp)){
         return false;
      }else{
         return true;
      }
   }

   // | ")"
   if(input[*len_index] == ')'){
      printf("Instructions - ')', %i\n", *len_index);
      (*len_index)++;
      return true;
   }

   return false;

}

//<INSTRCT> ::= "(" <FUNC> ")"
bool instruction(char* input, int* len_index, lisp* temp){

   printf("INSTURCTION function temp address - %p \n\n", (void*)(temp));

   char inst[MAX_LENGTH] = "";
   int len = 0;

   while(input[*len_index] != ')'){

         inst[len] = input[*len_index];

         if(input[*len_index + 1] == ')'){
            printf("inst[len] - %s \n\n", inst);
            len++;
            inst[len] = '\0';
            func(input, len_index, inst, temp);
         }

         if(inst[len] == ' '){
              
            inst[len] = '\0';
            func(input, len_index, inst, temp);

         }       

         len++;
         (*len_index)++;
   }


   if(input[*len_index] == ')'){
      printf("Instruction - Value - %c, %i \n", input[*len_index], *len_index);
      (*len_index)++;
      return true;

   }else{
      return false;
   }

}



//<FUNC> ::= <RETFUNC> | <IOFUNC> | <IF> | <LOOP>
bool func(char* input, int* len_index, char* inst, lisp* temp){

   int flag = 1;

         printf("\n (%i)Instruction - %s \n\n\n", *len_index, inst);
         printf("FUNC function temp address - %p \n\n", (void*)(temp));

         //IO Functions
         if(strcmp(inst, "SET") == 0){

            (*len_index)++;

            if(in_out_func(input, len_index, inst, temp)){
               clear_string(inst);
               flag = 0;
            }

         }else if(strcmp(inst, "PRINT") == 0){

            (*len_index)++;

            if(in_out_func(input, len_index, inst, temp)){
               clear_string(inst);
               flag = 0;
            }


/*
         //LIST Functions
         }else if(strcmp(inst, "CAR") == 0){
            printf("ENTER - CAR has been called\n");

            if(ret_func(input, len_index)){
               clear_string(inst);
               flag = 0;
            }

         }else if(strcmp(inst, "CDR") == 0){
            printf("CDR has been called\n");

            if(ret_func(input, len_index)){
               clear_string(inst);
               flag = 0;
            }

         }else if(strcmp(inst, "CONS") == 0){
            printf("CONS has been called\n");

            if(ret_func(input, len_index)){
               clear_string(inst);
               flag = 0;
            }

         //INT Functions
         }else if(strcmp(inst, "PLUS") == 0){
            printf("PLUS has been called\n");
            
            if(ret_func(input, len_index)){
               clear_string(inst);
               flag = 0;
            }

         }else if(strcmp(inst, "LENGTH") == 0){
            printf("LENGTH has been called\n");

            if(ret_func(input, len_index)){
               clear_string(inst);
               flag = 0;
            }

         //BOOL Functions
         }else if(strcmp(inst, "LESS") == 0){
            printf("LESS has been called\n");

            if(bool_func(input, len_index, inst)){
               clear_string(inst);
               flag = 0;
            }


         }else if(strcmp(inst, "GREATER") == 0){
            printf("GREATER has been called\n");

            if(bool_func(input, len_index, inst)){
               clear_string(inst);
               flag = 0;
            }


         }else if(strcmp(inst, "EQUAL") == 0){
            printf("EQUAL has been called\n");

            if(bool_func(input, len_index, inst)){
               clear_string(inst);
               flag = 0;
            }

         //error message - does not conform to grammar
         }else if(strcmp(inst, "IF") == 0){
            (*len_index)++;
            printf("IF has been called\n");

            if(if_func(input, len_index)){
               clear_string(inst);
               flag = 0;
            }
       
         }else if(strcmp(inst, "WHILE") == 0){
            (*len_index)++;
            printf("WHILE has been called\n");

            if(loop_func(input, len_index)){
               clear_string(inst);
               flag = 0;
            }
*/
         }else{
            
            ON_ERROR("Was Expecting a function name\n");
            flag = 1;
            return false;

         }
         

   if(flag == 1){
      ON_ERROR("The instruction is not known\n");
      return false;
   }

   return true;
}

//<IOFUNC> ::= <SET> | <PRINT>
bool in_out_func(char* input, int* len_index, char* instruc, lisp* temp){

   if(strcmp(instruc, "PRINT") == 0){

      ins_print(input, len_index, temp);
      printf("After PRINT function - %c , %i\n",input[*len_index], *len_index);
      (*len_index)--;
      return true;

   }else if(strcmp(instruc, "SET") == 0){
      
      //pass in the rest of the string into the function - and check the values
      ins_set(input, len_index, temp);

      //It should exit at this point with the ) from the SET
      printf("After SET function - %c , %i\n",input[*len_index], *len_index);
      (*len_index)--;

      return true;

   }

   return false;

}

//we want to set the value of a value to store the list in memory space
//<SET> ::= "SET" <VAR> <LIST>
bool ins_set(char* input, int* len_index, lisp* temp){

   printf("SET - len_index - %i \n", *len_index);

   while(input[*len_index] != ')'){

      if(input[*len_index] == ' '){
         (*len_index)++;
      }

      //check if the VAR grammar is correct
      if(ins_var(input, len_index)){

         (*len_index)++;

      //error message - input does not conform to grammar
      }else{
         ON_ERROR("Expecting a variable - between A to Z");
         return false;
      }

      //If it is a string literal
      if(ins_list(input, len_index, temp)){
         return true;
      }else{
         return false;
      };

   }

   return false;

}

//<LIST> ::= <VAR> | <LITERAL> | "NIL" | "(" <RETFUNC> ")"
bool ins_list(char* input, int* len_index, lisp* temp){

   printf("'%c', LIST - len_index - %i \n", input[*len_index], *len_index);
   printf("INS_LIST function temp address - %p \n\n", (void*)(temp));

   //literal 
   if(input[*len_index] == 39){
   //pass into a literal function - single quoted list - only numbers brackets and space
      (*len_index)++;
      if(ins_literal(input, len_index, temp)){
         return true;
      }else{
         ON_ERROR("Issue with literal value");
      }

   }else if(input[*len_index] == 'N' && input[*len_index + 1] == 'I' && input[*len_index + 2] == 'L'){

      printf("it is a NIL\n");
      (*len_index)++;
      return true;

   //check if it is a VAR, NIL
   }else if(input[*len_index] >= 'A' && input[*len_index] <= 'Z'){

      printf("Entered var check %i\n", *len_index);
      if(input[*len_index + 1] == ')'){
         (*len_index)++;
         printf("VARIABLE\n");
         return true;
      }else{

         printf("failed var check\n");
      }
   
   
   }else if(input[*len_index] == '('){

      printf("RETFUNC instruction triggered - index %i \n", *len_index);

      (*len_index)++;

      if(ret_func(input, len_index, temp)){
         if(input[*len_index] == ')'){
            printf("Value after returning from ret_func - %c  , %i \n", input[*len_index], *len_index);
            (*len_index)++;  
            return true;
         }
      }

   }

   return false;

}

//<LITERAL> ::= Single-quoted list e.g. ’(1)’, ’(1 2 3 (4 5))’, or ’2’
bool ins_literal(char* input, int* len_index, lisp* temp){

   printf("LITERAL - len_index - %i \n", *len_index);

   if(temp == NULL){
      printf("Value of temp is NULL\n\n");
   }else{
      printf("Value of temp is NOT NULL\n\n");
   }

   int flag = 0;

   //only accept space, bracket and numbers
   while(input[*len_index] != 39){
      if(!(input[*len_index] >= 65 && input[*len_index] <= 90)){   
         flag = 0;
      }else{
         flag = 1;
      }

      (*len_index)++;
   }

      #ifdef INTERPRET
      
         int len_input = 0;
         int len_newarray = 0;
         int flag2 = 0;
         char temp_array[MAX_LENGTH];

         //retrieve just the literal portion
         while(input[len_input] != '\0'){

            if(input[len_input] == 39){
                              
               if(input[len_input + 1] != '('){

                  temp_array[len_newarray] = '(';
                  len_newarray++;
                  len_input++;

                  while(input[len_input] != 39){
                     temp_array[len_newarray] = input[len_input];
                     len_input++;
                     len_newarray++;
                     flag2 = 1;
                  }

               }else{

                  len_input++;

                  while(input[len_input] != 39){
                     temp_array[len_newarray] = input[len_input];

                     len_input++;
                     len_newarray++;
                  }

               }
            }

            len_input++;
         }

         if(flag2 == 1){
            printf("Entered flag2\n");
            temp_array[len_newarray] = ')';
            len_newarray++;
            temp_array[len_newarray] = '\0';

         }else{
            printf("line 541: len_newarray - %i\n", len_newarray);
            len_newarray++;
            temp_array[len_newarray] = '\0'; 
         }

         temp = (lisp_fromstring(temp_array));
         printf("After copying: SET function temp address - %p \n\n", (void*)(temp));


         char str[MAX_LENGTH];
         lisp_tostring(temp, str);
         printf("Printing the list %s\n",str);

         printf("CAR - %i\n", lisp_getval(lisp_car(temp)));
         printf("LENGTH - %i\n", lisp_length(temp));

         lisp* l7 = lisp_cdr(temp);
         lisp_tostring(l7, str);
         printf("Cdr of list \n%s\n\n\n\n", str);

      #endif

   printf("\nLITERAL FLAG - %i\n", flag);

   if(flag == 1){
      (*len_index)++;
      return false;
   }

   (*len_index)++;
   return true;

}

//<VAR> ::= [A-Z]
bool ins_var(char* input, int* len_index){

   printf("VARIABLE - len_index - %i \n", *len_index);

   if(input[*len_index] >= 'A' && input[*len_index] <= 'Z'){
      if(input[*len_index + 1] == ' '){
         (*len_index)++;
         return true;
      }
   }else{
      return false;
   }

   return false;
}

//<PRINT> ::= "PRINT" <LIST> | "PRINT" <STRING>  
bool ins_print(char* input, int* len_index, lisp* temp){

   printf("PRINT - function - %i\n", *len_index);

   #ifdef INTERPRET
      printf("PRINT function temp address - %p \n\n", (void*)(temp));
      printf("temp->atomtype - %i\n\n", lisp_getval(lisp_car(temp)));
   #endif

   int flag = 0;

   if(ins_string(input, len_index)){
      flag = 1;
   }

   #ifdef INTERPRET

      if(temp != NULL && flag == 1){
         char print_string[MAX_LENGTH];
         int len1 = 0;
         int len2 = 0;

         while(input[len1] != '\0'){
            if(input[len1] == '"'){
               len1++;

               while(input[len1] != '"'){
                  print_string[len2] = input[len1];
                  len2++;
                  len1++;
               }
            }
            
            len1++;
         }

         print_string[len2] = '\0';

         printf("String - %s \n", print_string);
      }

   #endif

   if(flag == 0){
      if(ins_list(input, len_index, temp)){
         flag = 1;
      }
   }

   #ifdef INTERPRET

   char print_list[MAX_LENGTH];

   if(flag == 1 && temp != NULL){
      lisp_tostring(temp, print_list);

      printf("line 623: LIST: %s\n\n", print_list);
   }

   #endif


   if(flag == 1){
      return true;
   }else{
      ON_ERROR("Was expecting a list or a string\n");
      return false;
   }

   return false;
}




//<STRING> ::= Double-quoted string constant e.g. "Hello, World!", or "FAILURE ?"
bool ins_string(char* input, int* len_index){
   printf("char %c - %i \n",input[*len_index], *len_index);

   int flag = 0;

   if(input[*len_index] == '"'){
      (*len_index)++;

      while(input[*len_index] != ')'){
         
         if(input[*len_index] == '"'){
            flag = 1;
         }

         (*len_index)++;
      }

   }

   if(flag == 1){
      return true;
   }

   return false;
}


//clear instruction string to SPACE
void clear_string(char* input){

   int length = strlen(input);

   for(int i = 0; i <= length; i++){
      input[i] = ' ';
   }

}


//<RETFUNC> ::= <LISTFUNC> | <INTFUNC> | <BOOLFUNC> - needed for the PRINT Statement
bool ret_func(char* input, int* len_index, lisp* temp){
   
   printf("\n\n-----------Return Func---------------\n\n");

   char array[MAX_LENGTH] = "";
   int index = 0;

   while(input[*len_index] != ')'){
      array[index] = input[*len_index];

      //LISTFUNC
      if(strcmp(array, "CAR") == 0){
         (*len_index) = (*len_index) + 2;
         printf("\n ---------CAR instruction triggered - index %i - value - %c \n", *len_index, input[*len_index]);
         list_func(input, len_index, array, temp);

      }else if(strcmp(array, "CDR") == 0){
         (*len_index) = (*len_index) + 2;
         printf("CDR instruction triggered \n");
         list_func(input, len_index, array, temp);

      }else if(strcmp(array, "CONS") == 0){
         (*len_index) = (*len_index) + 2;
         printf("CONS instruction triggered - index %i \n", *len_index);
         list_func(input, len_index, array, temp);

      }

      //INTFUNC
      else if(strcmp(array, "PLUS") == 0){
         (*len_index) = (*len_index) + 2;
         printf("PLUS instruction triggered - index %i \n", *len_index);
         int_func(input, len_index, array, temp);
      
      }else if(strcmp(array, "LENGTH") == 0){
         (*len_index) = (*len_index) + 2;
         printf("LENGTH instruction triggered - index %i \n", *len_index);
         int_func(input, len_index, array, temp);

      } 

      //BOOLFUNC
      else if(strcmp(array, "LESS") == 0){
         (*len_index) = (*len_index) + 2;
         printf("LESS instruction triggered - index %i \n", *len_index);
         bool_func(input, len_index, array, temp);

      }else if(strcmp(array, "GREATER") == 0){
         (*len_index) = (*len_index) + 2;
         printf("GREATER instruction triggered - index %i \n", *len_index);
         bool_func(input, len_index, array, temp);
      
      }else if(strcmp(array,"EQUAL") == 0){
         printf("before - EQUAL instruction triggered - index %i \n", *len_index);
         (*len_index) = (*len_index) + 2;
         printf("after - EQUAL instruction triggered - index %i \n", *len_index);
         bool_func(input, len_index, array, temp);
      
      }

      index++;
      (*len_index)++;
   }

   printf("Last index - %i   %c\n\n", *len_index, input[*len_index]);
   printf("------------------EXIT RET_FUNC---------------------\n\n");


   return true;
}



//<LISTFUNC> ::= "CAR" <LIST> | "CDR" <LIST> | "CONS" <LIST> <LIST>
bool list_func(char* input, int* len_index, char* instruct, lisp* temp){

while(input[*len_index] != ')'){

   if(strcmp(instruct, "CAR") == 0){

      printf("\n1.list_func - CAR instruction - len index %i - %c\n", *len_index, input[*len_index]);

      if(ins_list(input, len_index, temp)){

         printf("\n2.list_func - CAR instruction - len index %i - %c\n", *len_index, input[*len_index]);

         (*len_index)--;

         return true;
      }

   }else if(strcmp(instruct, "CDR") == 0){

      printf("\n1.list_func - CDR instruction - len index %i - %c\n", *len_index, input[*len_index]);

      if(ins_list(input, len_index, temp)){

         printf("\n2.list_func - CDR instruction - len index %i - %c\n", *len_index, input[*len_index]);

         (*len_index)--;

         return true;
      }

   }else if(strcmp(instruct, "CONS") == 0){

      printf("\n1.list_func - CONS instruction - len index %i \n", *len_index);

      if(ins_list(input, len_index, temp)){
         
         printf("\n2.list_func - CONS instruction - len index %i \n", *len_index);
            
            (*len_index)++;

               if(ins_list(input, len_index, temp)){

                     (*len_index)--;

                     return true;
               }

      }
   }

   if(input[*len_index] == ' '){
      (*len_index)++;
   }

}

   if(input[*len_index] == ')'){
     
      printf("list_func returns ') at index %i'\n", *len_index);

   }

   return true;
}

//<INTFUNC> ::= "PLUS" <LIST> <LIST> | "LENGTH" <LIST>
bool int_func(char* input, int* len_index, char* instruct, lisp* temp){

while(input[*len_index] != ')'){

   if(strcmp(instruct,"PLUS") == 0){

      printf("\n1.int_func - PLUS instruction - len index %i \n", *len_index);

      if(ins_list(input, len_index, temp)){
         
         printf("\n2.int_func - PLUS instruction - len index %i \n", *len_index);

         (*len_index)++;

         if(ins_list(input, len_index, temp)){

            (*len_index)--;

            return true;
         }
      }

   }else if(strcmp(instruct,"LENGTH") == 0){

      printf("\n1.int_func - LENGTH instruction - len index %i \n", *len_index);

      if(ins_list(input, len_index, temp)){

         (*len_index)--;

         return true;
      }
   }

   if(input[*len_index] == ' '){
      (*len_index)++;
   }

}

   return true;
}

//<BOOLFUNC> ::= "LESS" <LIST> <LIST> | "GREATER" <LIST> <LIST> | "EQUAL" <LIST> <LIST>
bool bool_func(char* input, int* len_index, char* instruct, lisp* temp){

   printf("BOOL FUNC - index %i, character - %c \n\n", *len_index, input[*len_index]);


while(input[*len_index] != ')'){

   if(input[*len_index] == ' '){
      (*len_index)++;
   }

   if(strcmp(instruct, "LESS") == 0){

      printf("\n1.bool_func - LESS instruction - len index %i \n", *len_index);

      if(ins_list(input, len_index, temp)){
         
         printf("\n2.bool_func - LESS instruction - len index %i \n", *len_index);

         (*len_index)++;

         if(ins_list(input, len_index, temp)){

            (*len_index)--;

            return true;
         }
      }

   }else if(strcmp(instruct, "GREATER") == 0){

      printf("\n1.bool_func - GREATER instruction - len index %i \n", *len_index);

      if(ins_list(input, len_index, temp)){
         
         printf("\n2.bool_func - GREATER instruction - len index %i \n", *len_index);

         (*len_index)++;

         if(ins_list(input, len_index, temp)){

            (*len_index)--;

            return true;
         }
      }
      
   }else if(strcmp(instruct, "EQUAL") == 0){

      printf("\n1.bool_func - EQUAL instruction - len index %i \n", *len_index);

      if(ins_list(input, len_index, temp)){
         
         printf("\n2.bool_func - EQUAL instruction - len index %i \n", *len_index);

         (*len_index)++;

         if(ins_list(input, len_index, temp)){

            (*len_index)--;

            return true;
         }
      }

   }

}

   return false;
}


//<IF> ::= "IF" "(" <BOOLFUNC> ")" "(" <INSTRCTS> "(" <INSTRCTS>
bool if_func(char* input, int* len_index, lisp* temp){

   printf("Entered IF-func %i, %c \n", *len_index, input[*len_index]);  

   char instruct[MAX_LENGTH];
   int len = 0;
   int flag = 0;
   int flag2 = 0, flag3 = 0, flag4 = 0;

   //spaces in between
   while(input[*len_index] == ' '){
      (*len_index)++;
   }   

   //"(" <BOOLFUNC> ")"
   if(input[*len_index] == '('){
      (*len_index)++;

      while(input[*len_index] != ')'){

         ////<BOOLFUNC> ::= "LESS" <LIST> <LIST> | "GREATER" <LIST> <LIST> | "EQUAL" <LIST> <LIST>
         instruct[len] = input[*len_index];

         if(instruct[len] == ' '){

            instruct[len] = '\0';
            (*len_index)++;
            printf("%s\n", instruct);

            if(bool_func(input, len_index, instruct, temp)){
               
               flag = 1;
               
            }
         }
         
         len++;
         (*len_index)++;
      }

      printf("\n%i, %c ,  flag - %i\n", *len_index, input[*len_index], flag);

   }


   if(flag == 1 && input[*len_index] == ')'){

      printf("--------Flag1 return true----------- \n\n");
      (*len_index)++;
      flag2 = 1;
   }




   //"(" <INSTRCTS>
   if(flag2 == 1){

      printf("--------Flag2 return true----------- \n\n");
      printf("index - %i, %c\n\n", *len_index, input[*len_index]);

      //spaces in between
      while(input[*len_index] == ' '){
         (*len_index)++;
      }

      if(input[*len_index] == '('){
         (*len_index)++;

         if(instructions(input, len_index, temp)){

            printf("IF condition - first instructions return %i", *len_index);
            flag3 = 1;
         }
      }

      //"(" <INSTRCTS>
      if(flag3 == 1){
      printf("\n--------Flag3 return true----------- \n\n");

         while(input[*len_index] == ' '){
            (*len_index)++;
         }

         if(input[*len_index] == '('){
            (*len_index)++;

            if(instructions(input, len_index, temp)){

               printf("IF condition - first instructions return %i\n", *len_index);
               flag4 = 1;
            
            } 
         }

      }else{
         return false;
      }

   }else{
      return false;
   }


   if(flag4 == 1){
      printf("--------Flag4 return true----------- \n\n");
      (*len_index)--;

      return true;
   }

   return false;
}




//<LOOP> ::= "WHILE""(" <BOOLFUNC> ")" "(" <INSTRCTS>
bool loop_func(char* input, int* len_index, lisp* temp){ 

   printf("Entered LOOP-func %i, %c \n", *len_index, input[*len_index]);

   char instruct[MAX_LENGTH];
   int flag = 0, flag2 = 0, flag3 = 0;
   int len = 0;

   //spaces in between
   while(input[*len_index] == ' '){
      (*len_index)++;
   }

      //"(" <BOOLFUNC> ")"
   if(input[*len_index] == '('){
      (*len_index)++;

      while(input[*len_index] != ')'){

         ////<BOOLFUNC> ::= "LESS" <LIST> <LIST> | "GREATER" <LIST> <LIST> | "EQUAL" <LIST> <LIST>
         instruct[len] = input[*len_index];

         if(instruct[len] == ' '){

            instruct[len] = '\0';
            printf("%s\n", instruct);

            if(bool_func(input, len_index, instruct, temp)){
               
               flag = 1;
               
            }
         }

         len++;
         (*len_index)++;
      }
   }

   if(flag == 1 && input[*len_index] == ')'){
      (*len_index)++;

      printf("\n\n\nINDEX - %i\n\n", *len_index);

      printf("--------LOOP - Flag1 return true----------- \n\n");

      flag2 = 1;
   }


   //"(" <INSTRCTS>
   if(flag2 == 1){

      printf("\n\n\nINDEX - %i\n\n", *len_index);
      printf("--------LOOP - Flag2 return true----------- \n\n");

      //spaces in between
      while(input[*len_index] != ')'){
         
         if(input[*len_index] == '('){
            (*len_index)++;
            printf("\n\n\n AFTER FIRST INDEX - %i\n\n", *len_index);

            if(instructions(input, len_index, temp)){

               printf("WHILE condition - first instructions return %i\n", *len_index);
               flag3 = 1;
            }
         }

         (*len_index)++;
      }

   }

   if(flag3 == 1){

      (*len_index) = (*len_index)-2;

      return true;
   }

return false;

}
