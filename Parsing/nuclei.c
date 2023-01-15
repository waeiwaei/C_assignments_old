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

struct prog{
   char input[MAX_LENGTH];
   int len_index;
   char instruction[MAX_LENGTH];
   
   #ifdef INTERPRET
      lisp* lisp_structure;
   #endif
};

typedef struct prog prog;

bool program(prog* prog);
bool list_func(prog* prog);
bool func(prog* prog);
bool in_out_func(prog* prog);
bool int_func(prog* prog);
bool bool_func(prog* prog);
bool loop_func(prog* prog);
bool if_func(prog* prog);
bool instructions(prog* prog);
bool instruction(prog* prog);
bool ins_set(prog* prog);
bool ins_list(prog* prog);
bool ins_var(prog* prog);
bool ins_literal(prog* prog);
bool ins_string(prog* prog);
bool ins_print(prog* prog);
void clear_string(prog* prog);
bool ret_func(prog* prog);


//read file from argv, and store characters in 1D array
int main(int argc, char* argv[]){

   prog* pro = (prog*)ncalloc(1, sizeof(prog));
   
   //char input[MAX_LENGTH];
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
            pro->input[val] = c;
            val++;
            c = fgetc(fp);
         }

      }

      pro->input[val] = '\0';

      fclose(fp); 

      /*program function for parser*/
      if(program(pro)){
         printf("Parsed OK!");
      }

      }
   }
   
   return 0;
}


//<PROG> ::= "(" <INSTRCTS>
bool program(prog* prog){

   printf("%s\n\n", prog->input);

   //we check if the first character is a '(', then go into instructions
   if(prog->input[0] == '('){

      prog->len_index = 1;
      //go into the instructions function

      if(instructions(prog)){
         return true;
      }else{
         return false;
      }
   }
   
   ON_ERROR("Expression does not have an opening bracket\n");
   return false;

}


//<INSTRCTS> ::= <INSTRCT> <INSTRCTS> | ")"
bool instructions(prog* prog){

   printf("Instructions - %i, %c \n", prog->len_index, prog->input[prog->len_index]);

   while(prog->input[prog->len_index] == ' '){ 
      (prog->len_index)++;
   }

      if(prog->input[prog->len_index] == '('){
         (prog->len_index)++;

         if(!instruction(prog)){            
            return false;
         }

         printf("This is the LEN index here - %i \n\n", prog->len_index);
   
      if(!instructions(prog)){
         return false;
      }else{
         return true;
      }
   }

   // | ")"
   if(prog->input[prog->len_index] == ')'){
      printf("Instructions - ')', %i\n", prog->len_index);
      (prog->len_index)++;
      return true;
   }

   return false;

}

//<INSTRCT> ::= "(" <FUNC> ")"
bool instruction(prog* prog){

   char inst[MAX_LENGTH] = "";
   int len = 0;

   while(prog->input[prog->len_index] != ')'){

         prog->instruction[len] = prog->input[prog->len_index];

         if(prog->input[prog->len_index + 1] == ')'){
            /*printf("inst[len] - %s \n\n", inst);
            len++;
            inst[len] = '\0';
            func(prog, inst);*/

            printf("inst[len] - %s \n\n", inst);
            len++;
            prog->instruction[len] = '\0';
            func(prog);
         }

         if(prog->instruction[len] == ' '){
              
            prog->instruction[len] = '\0';
            func(prog);

         }       

         len++;
         (prog->len_index)++;
   }


   if(prog->input[prog->len_index] == ')'){
      printf("Instruction - Value - %c, %i \n", prog->input[prog->len_index], prog->len_index);
      (prog->len_index)++;
      return true;

   }else{
      return false;
   }

}



//<FUNC> ::= <RETFUNC> | <IOFUNC> | <IF> | <LOOP>
bool func(prog* prog){

   int flag = 1;

         printf("\n (%i)Instruction - %s \n\n\n", prog->len_index, prog->instruction);

         //IO Functions
         if(strcmp(prog->instruction, "SET") == 0){

            (prog->len_index)++;

            if(in_out_func(prog)){
               clear_string(prog);
               flag = 0;
            }

         }else if(strcmp(prog->instruction, "PRINT") == 0){

            (prog->len_index)++;

            if(in_out_func(prog)){
               clear_string(prog);
               flag = 0;
            }



         //LIST Functions
         }else if(strcmp(prog->instruction, "CAR") == 0){
            printf("ENTER - CAR has been called\n");

            if(ret_func(prog)){
               clear_string(prog);
               flag = 0;
            }

         }else if(strcmp(prog->instruction, "CDR") == 0){
            printf("CDR has been called\n");

            if(ret_func(prog)){
               clear_string(prog);
               flag = 0;
            }

         }else if(strcmp(prog->instruction, "CONS") == 0){
            printf("CONS has been called\n");

            if(ret_func(prog)){
               clear_string(prog);
               flag = 0;
            }

         //INT Functions
         }else if(strcmp(prog->instruction, "PLUS") == 0){
            printf("PLUS has been called\n");
            
            if(ret_func(prog)){
               clear_string(prog);
               flag = 0;
            }

         }else if(strcmp(prog->instruction, "LENGTH") == 0){
            printf("LENGTH has been called\n");

            if(ret_func(prog)){
               clear_string(prog);
               flag = 0;
            }

         //BOOL Functions
         }else if(strcmp(prog->instruction, "LESS") == 0){
            printf("LESS has been called\n");

            if(bool_func(prog)){
               clear_string(prog);
               flag = 0;
            }


         }else if(strcmp(prog->instruction, "GREATER") == 0){
            printf("GREATER has been called\n");

            if(bool_func(prog)){
               clear_string(prog);
               flag = 0;
            }


         }else if(strcmp(prog->instruction, "EQUAL") == 0){
            printf("EQUAL has been called\n");

            if(bool_func(prog)){
               clear_string(prog);
               flag = 0;
            }

         //error message - does not conform to grammar
         }else if(strcmp(prog->instruction, "IF") == 0){
            (prog->len_index)++;
            printf("IF has been called\n");

            if(if_func(prog)){
               clear_string(prog);
               flag = 0;
            }
       
         }else if(strcmp(prog->instruction, "WHILE") == 0){
            (prog->len_index)++;
            printf("WHILE has been called\n");

            if(loop_func(prog)){
               clear_string(prog);
               flag = 0;
            }

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
bool in_out_func(prog* prog){

   if(strcmp(prog->instruction, "PRINT") == 0){

      ins_print(prog);
      printf("After PRINT function - %c , %i\n",prog->input[prog->len_index], prog->len_index);
      (prog->len_index)--;
      return true;

   }else if(strcmp(prog->instruction, "SET") == 0){
      
      //pass in the rest of the string into the function - and check the values
      ins_set(prog);

      //It should exit at this point with the ) from the SET
      printf("After SET function - %c , %i\n", prog->input[prog->len_index], prog->len_index);
      (prog->len_index)--;

      return true;

   }

   return false;

}

//we want to set the value of a value to store the list in memory space
//<SET> ::= "SET" <VAR> <LIST>
bool ins_set(prog* prog){

   printf("SET - len_index - %i \n", prog->len_index);

   while(prog->input[prog->len_index] != ')'){

      if(prog->input[prog->len_index] == ' '){
         (prog->len_index)++;
      }

      //check if the VAR grammar is correct
      if(ins_var(prog)){
      
      (prog->len_index)++;

      //error message - input does not conform to grammar
      }else{
         ON_ERROR("Expecting a variable - between A to Z");
         return false;
      }

      //If it is a string literal
      if(ins_list(prog)){
         return true;
      }else{
         return false;
      };

   }

   return false;

}

//<LIST> ::= <VAR> | <LITERAL> | "NIL" | "(" <RETFUNC> ")"
bool ins_list(prog* prog){

   printf("'%c', LIST - len_index - %i \n", prog->input[prog->len_index], prog->len_index);

   //literal 
   if(prog->input[prog->len_index] == 39){
   //pass into a literal function - single quoted list - only numbers brackets and space
      (prog->len_index)++;
      if(ins_literal(prog)){
         return true;
      }else{
         ON_ERROR("Issue with literal value");
      }

   }else if(prog->input[prog->len_index] == 'N' && prog->input[prog->len_index + 1] == 'I' && prog->input[prog->len_index + 2] == 'L'){

      printf("it is a NIL\n");

      #ifdef INTERPRET
         prog->lisp_structure = NULL;
      #endif

      (prog->len_index)++;
      return true;

   //check if it is a VAR, NIL
   }else if(prog->input[prog->len_index] >= 'A' && prog->input[prog->len_index] <= 'Z'){

      printf("Entered var check %i\n", prog->len_index);
      if(prog->input[prog->len_index + 1] == ')'){
         (prog->len_index)++;
         printf("VARIABLE\n");
         return true;
      }else{

         printf("failed var check\n");
      }
   
   
   }else if(prog->input[prog->len_index] == '('){

      printf("RETFUNC instruction triggered - index %i \n", prog->len_index);

      (prog->len_index)++;

      if(ret_func(prog)){
         if(prog->input[prog->len_index] == ')'){
            printf("Value after returning from ret_func - %c  , %i \n", prog->input[prog->len_index], prog->len_index);
            (prog->len_index)++;  
            return true;
         }
      }

   }

   return false;

}

//<LITERAL> ::= Single-quoted list e.g. ’(1)’, ’(1 2 3 (4 5))’, or ’2’
bool ins_literal(prog* prog){

   printf("LITERAL - len_index - %i \n", prog->len_index);

   int flag = 0;
   //only accept space, bracket and numbers
   while(prog->input[prog->len_index] != 39){
      if(!(prog->input[prog->len_index] >= 65 && prog->input[prog->len_index] <= 90)){   
         flag = 0;
      }else{
         flag = 1;
      }

      (prog->len_index)++;
   }

      #ifdef INTERPRET
      
         static int const_index = 0;
         int len_input = const_index;
         int len_newarray = 0;
         int flag2 = 0;
         char temp_array[MAX_LENGTH];

         //retrieve just the literal portion
         while(prog->input[len_input] != '\0'){

            if(prog->input[len_input] == 39){
               printf("len_input - %i\n", len_input);

               if(prog->input[len_input + 1] != '('){

                  temp_array[len_newarray] = '(';
                  len_newarray++;
                  len_input++;

                  while(prog->input[len_input] != 39){
                     temp_array[len_newarray] = prog->input[len_input];
                     len_input++;
                     len_newarray++;
                     flag2 = 1;
                  }
                  break;

               }else{

                  len_input++;

                  while(prog->input[len_input] != 39){
                     temp_array[len_newarray] = prog->input[len_input];

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
            len_newarray++;
            temp_array[len_newarray] = '\0'; 
         }

         temp_array[len_newarray] = '\0';

         const_index = len_input + 1;


         printf("TEMP- ARRAY: %s %i\n\n", temp_array, len_input);

         prog->lisp_structure = (lisp_fromstring(temp_array));


         if(flag2 == 1){
            prog->lisp_structure->choice = true;
         }

         char str[MAX_LENGTH];
         lisp_tostring(prog->lisp_structure, str);
         printf("Printing the list %s\n",str);

         printf("CAR - %i\n", lisp_getval(lisp_car(prog->lisp_structure)));
         printf("LENGTH - %i\n", lisp_length(prog->lisp_structure));

         lisp_tostring(prog->lisp_structure, str);
         printf("Cdr of list %s\n\n\n\n", str);

      #endif

   printf("\nLITERAL FLAG - %i\n", flag);

   if(flag == 1){
      (prog->len_index)++;
      return false;
   }

   (prog->len_index)++;
   return true;

}

//<VAR> ::= [A-Z]
bool ins_var(prog* prog){

   printf("VARIABLE - len_index - %i \n", prog->len_index);

   if(prog->input[prog->len_index] >= 'A' && prog->input[prog->len_index] <= 'Z'){
      if(prog->input[prog->len_index + 1] == ' '){
         (prog->len_index)++;
         return true;
      }
   }else{
      return false;
   }

   return false;
}

//<PRINT> ::= "PRINT" <LIST> | "PRINT" <STRING>  
bool ins_print(prog* prog){

   printf("PRINT - function - %i\n", prog->len_index);
   int flag = 0;

   if(ins_string(prog)){
      flag = 1;
   }

   if(flag == 0){
      if(ins_list(prog)){
         flag = 1;
      }
   }

   #ifdef INTERPRET

   int len = 0;
   //int len1 = 0;
   int flag3 = 0;
   char str[MAX_LENGTH];

   while(prog->input[len] != '\0'){
      if(prog->input[len] == '"'){
         flag3 = 1;
      }
      len++;
   }

   if(prog->lisp_structure != NULL && flag3 == 0){
      
      lisp_tostring(prog->lisp_structure, str);
      printf("PRINT FUNCTION - %s\n\n\n", str);
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
bool ins_string(prog* prog){
   printf("char %c - %i \n",prog->input[prog->len_index], prog->len_index);

   int flag = 0;

   if(prog->input[prog->len_index] == '"'){
      (prog->len_index)++;

      while(prog->input[prog->len_index] != ')'){
         
         if(prog->input[prog->len_index] == '"'){
            flag = 1;
         }

         (prog->len_index)++;
      }

   }

   if(flag == 1){
      return true;
   }

   return false;
}


//clear instruction string to SPACE
void clear_string(prog* prog){

   int length = strlen(prog->instruction);

   for(int i = 0; i <= length; i++){
      prog->instruction[i] = ' ';
   }

}


//<RETFUNC> ::= <LISTFUNC> | <INTFUNC> | <BOOLFUNC> - needed for the PRINT Statement
bool ret_func(prog* prog){
   
   printf("\n\n-----------Return Func---------------\n\n");

   //char array[MAX_LENGTH] = "";
   int index = 0;

   clear_string(prog);

   while(prog->input[prog->len_index] != ')'){
      
      prog->instruction[index] = prog->input[prog->len_index];

      if(prog->instruction[index] == ' '){
         break;
      }

      index++;
      (prog->len_index)++;
   }

      prog->instruction[index] = '\0';
      printf("%s\n\n", prog->instruction);

      //printf("%s\n", prog->instruction);
      //LISTFUNC
      if(strcmp(prog->instruction, "CAR") == 0){
         (prog->len_index) = (prog->len_index) + 2;
         printf("\n ---------CAR instruction triggered - index %i - value - %c \n", prog->len_index, prog->input[prog->len_index]);
         list_func(prog);

      }else if(strcmp(prog->instruction, "CDR") == 0){
         (prog->len_index) = (prog->len_index) + 2;
         printf("CDR instruction triggered \n");
         list_func(prog);

      }else if(strcmp(prog->instruction, "CONS") == 0){
         //printf("Triggered\n");
         //(prog->len_index) = (prog->len_index) + 2;
         printf("CONS - %i\n\n", prog->len_index);
         prog->len_index++;
         printf("CONS instruction triggered - index %i \n", prog->len_index);
         list_func(prog);

      }

      //INTFUNC
      else if(strcmp(prog->instruction, "PLUS") == 0){
         (prog->len_index) = (prog->len_index) + 2;
         printf("PLUS instruction triggered - index %i \n", prog->len_index);
         int_func(prog);
      
      }else if(strcmp(prog->instruction, "LENGTH") == 0){
         (prog->len_index) = (prog->len_index) + 2;
         printf("LENGTH instruction triggered - index %i \n", prog->len_index);
         int_func(prog);

      } 

      //BOOLFUNC
      else if(strcmp(prog->instruction, "LESS") == 0){
         (prog->len_index) = (prog->len_index) + 2;
         printf("LESS instruction triggered - index %i \n", prog->len_index);
         bool_func(prog);

      }else if(strcmp(prog->instruction, "GREATER") == 0){
         (prog->len_index) = (prog->len_index) + 2;
         printf("GREATER instruction triggered - index %i \n", prog->len_index);
         bool_func(prog);
      
      }else if(strcmp(prog->instruction,"EQUAL") == 0){
         printf("before - EQUAL instruction triggered - index %i \n", prog->len_index);
         (prog->len_index) = (prog->len_index) + 2;
         printf("after - EQUAL instruction triggered - index %i \n", prog->len_index);
         bool_func(prog);
      
      }




   printf("Last index - %i   %c\n\n", prog->len_index, prog->input[prog->len_index]);
   printf("------------------EXIT RET_FUNC---------------------\n\n");


   return true;
}



//<LISTFUNC> ::= "CAR" <LIST> | "CDR" <LIST> | "CONS" <LIST> <LIST>
bool list_func(prog* prog){

#ifdef INTERPRET
lisp* temp1;
lisp* temp2;
#endif

printf("list function - index - %i\n\n", prog->len_index);

while(prog->input[prog->len_index] != ')'){

   if(strcmp(prog->instruction, "CAR") == 0){

      printf("\n1.list_func - CAR instruction - len index %i - %c\n", prog->len_index, prog->input[prog->len_index]);

      if(ins_list(prog)){

         printf("\n2.list_func - CAR instruction - len index %i - %c\n", prog->len_index, prog->input[prog->len_index]);

         (prog->len_index)--;

         return true;
      }

   }else if(strcmp(prog->instruction, "CDR") == 0){

      printf("\n1.list_func - CDR instruction - len index %i - %c\n", prog->len_index, prog->input[prog->len_index]);

      if(ins_list(prog)){

         printf("\n2.list_func - CDR instruction - len index %i - %c\n", prog->len_index, prog->input[prog->len_index]);

         (prog->len_index)--;

         return true;
      }

   }else if(strcmp(prog->instruction, "CONS") == 0){

      printf("\n1.list_func - CONS instruction - len index %i \n", prog->len_index);

      if(ins_list(prog)){

         #ifdef INTERPRET 
            temp1 = prog->lisp_structure;
            printf("temp1 - %i", lisp_getval(temp1));
         #endif

         printf("\n2.list_func - CONS instruction - len index %i \n", prog->len_index);
            
            (prog->len_index)++;

            printf("line 816 - %i\n\n", prog->len_index);

               if(ins_list(prog)){

                     #ifdef INTERPRET 
                        temp2 = prog->lisp_structure;
                        printf("temp2 - %i", lisp_getval(temp2));

                     #endif

                     (prog->len_index)--;

                     return true;
               }
      }

   #ifdef INTERPRET
      char str[MAX_LENGTH];

      prog->lisp_structure = lisp_cons(temp1, temp2);

      lisp_tostring(prog->lisp_structure, str);

      printf("CONS function - %s\n\n\n", str);
   }
   #endif 

   if(prog->input[prog->len_index] == ' '){
      (prog->len_index)++;
   }

}

   if(prog->input[prog->len_index] == ')'){
     
      printf("list_func returns ') at index %i'\n", prog->len_index);

   }

   return true;
}


//<INTFUNC> ::= "PLUS" <LIST> <LIST> | "LENGTH" <LIST>
bool int_func(prog* prog){

while(prog->input[prog->len_index] != ')'){

   if(strcmp(prog->instruction,"PLUS") == 0){

      printf("\n1.int_func - PLUS instruction - len index %i \n", prog->len_index);

      if(ins_list(prog)){
         
         printf("\n2.int_func - PLUS instruction - len index %i \n", prog->len_index);

         (prog->len_index)++;

         if(ins_list(prog)){

            (prog->len_index)--;

            return true;
         }
      }

   }else if(strcmp(prog->instruction,"LENGTH") == 0){

      printf("\n1.int_func - LENGTH instruction - len index %i \n", prog->len_index);

      if(ins_list(prog)){

         (prog->len_index)--;

         return true;
      }
   }

   if(prog->input[prog->len_index] == ' '){
      (prog->len_index)++;
   }

}

   return true;
}

//<BOOLFUNC> ::= "LESS" <LIST> <LIST> | "GREATER" <LIST> <LIST> | "EQUAL" <LIST> <LIST>
bool bool_func(prog* prog){

   printf("BOOL FUNC - index %i, character - %c \n\n", prog->len_index, prog->input[prog->len_index]);

while(prog->input[prog->len_index] != ')'){

   if(prog->input[prog->len_index] == ' '){
      (prog->len_index)++;
   }

   if(strcmp(prog->instruction, "LESS") == 0){

      printf("\n1.bool_func - LESS instruction - len index %i \n", prog->len_index);

      if(ins_list(prog)){
         
         printf("\n2.bool_func - LESS instruction - len index %i \n", prog->len_index);

         (prog->len_index)++;

         if(ins_list(prog)){

            (prog->len_index)--;

            return true;
         }
      }

   }else if(strcmp(prog->instruction, "GREATER") == 0){

      printf("\n1.bool_func - GREATER instruction - len index %i \n", prog->len_index);

      if(ins_list(prog)){
         
         printf("\n2.bool_func - GREATER instruction - len index %i \n", prog->len_index);

         (prog->len_index)++;

         if(ins_list(prog)){

            (prog->len_index)--;

            return true;
         }
      }
      
   }else if(strcmp(prog->instruction, "EQUAL") == 0){

      printf("\n1.bool_func - EQUAL instruction - len index %i \n", prog->len_index);

      if(ins_list(prog)){
         
         printf("\n2.bool_func - EQUAL instruction - len index %i \n", prog->len_index);

         (prog->len_index)++;

         if(ins_list(prog)){

            (prog->len_index)--;

            return true;
         }
      }

   }

}

   return false;
}


//<IF> ::= "IF" "(" <BOOLFUNC> ")" "(" <INSTRCTS> "(" <INSTRCTS>
bool if_func(prog* prog){

   printf("Entered IF-func %i, %c \n", prog->len_index, prog->input[prog->len_index]);  

   //char instruct[MAX_LENGTH];
   int len = 0;
   int flag = 0;
   int flag2 = 0, flag3 = 0, flag4 = 0;

   //spaces in between
   while(prog->input[prog->len_index] == ' '){
      (prog->len_index)++;
   }   

   //"(" <BOOLFUNC> ")"
   if(prog->input[prog->len_index] == '('){
      (prog->len_index)++;

      while(prog->input[prog->len_index] != ')'){

         ////<BOOLFUNC> ::= "LESS" <LIST> <LIST> | "GREATER" <LIST> <LIST> | "EQUAL" <LIST> <LIST>
         prog->instruction[len] = prog->input[prog->len_index];

         if(prog->instruction[len] == ' '){

            prog->instruction[len] = '\0';
            (prog->len_index)++;
            printf("%s\n", prog->instruction);

            if(bool_func(prog)){
               
               flag = 1;
               
            }
         }
         
         len++;
         (prog->len_index)++;
      }

      printf("\n%i, %c ,  flag - %i\n", prog->len_index, prog->input[prog->len_index], flag);

   }


   if(flag == 1 && prog->input[prog->len_index] == ')'){

      printf("--------Flag1 return true----------- \n\n");
      (prog->len_index)++;
      flag2 = 1;
   }




   //"(" <INSTRCTS>
   if(flag2 == 1){

      printf("--------Flag2 return true----------- \n\n");
      printf("index - %i, %c\n\n", prog->len_index, prog->input[prog->len_index]);

      //spaces in between
      while(prog->input[prog->len_index] == ' '){
         (prog->len_index)++;
      }

      if(prog->input[prog->len_index] == '('){
         (prog->len_index)++;

         if(instructions(prog)){

            printf("IF condition - first instructions return %i", prog->len_index);
            flag3 = 1;
         }
      }

      //"(" <INSTRCTS>
      if(flag3 == 1){
      printf("\n--------Flag3 return true----------- \n\n");

         while(prog->input[prog->len_index] == ' '){
            (prog->len_index)++;
         }

         if(prog->input[prog->len_index] == '('){
            (prog->len_index)++;

            if(instructions(prog)){

               printf("IF condition - first instructions return %i\n", prog->len_index);
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
      (prog->len_index)--;

      return true;
   }

   return false;
}



//<LOOP> ::= "WHILE""(" <BOOLFUNC> ")" "(" <INSTRCTS>
bool loop_func(prog* prog){ 

   printf("Entered LOOP-func %i, %c \n", prog->len_index, prog->input[prog->len_index]);

   //char instruct[MAX_LENGTH];
   int flag = 0, flag2 = 0, flag3 = 0;
   int len = 0;

   //spaces in between
   while(prog->input[prog->len_index] == ' '){
      (prog->len_index)++;
   }

      //"(" <BOOLFUNC> ")"
   if(prog->input[prog->len_index] == '('){
      (prog->len_index)++;

      while(prog->input[prog->len_index] != ')'){

         ////<BOOLFUNC> ::= "LESS" <LIST> <LIST> | "GREATER" <LIST> <LIST> | "EQUAL" <LIST> <LIST>
         prog->instruction[len] = prog->input[prog->len_index];

         if(prog->instruction[len] == ' '){

            prog->instruction[len] = '\0';
            printf("%s\n", prog->instruction);

            if(bool_func(prog)){
               
               flag = 1;
               
            }
         }

         len++;
         (prog->len_index)++;
      }
   }

   if(flag == 1 && prog->input[prog->len_index] == ')'){
      (prog->len_index)++;

      printf("\n\n\nINDEX - %i\n\n", prog->len_index);

      printf("--------LOOP - Flag1 return true----------- \n\n");

      flag2 = 1;
   }


   //"(" <INSTRCTS>
   if(flag2 == 1){

      printf("\n\n\nINDEX - %i\n\n", prog->len_index);
      printf("--------LOOP - Flag2 return true----------- \n\n");

      //spaces in between
      while(prog->input[prog->len_index] != ')'){
         
         if(prog->input[prog->len_index] == '('){
            (prog->len_index)++;
            printf("\n\n\n AFTER FIRST INDEX - %i\n\n", prog->len_index);

            if(instructions(prog)){

               printf("WHILE condition - first instructions return %i\n", prog->len_index);
               flag3 = 1;
            }
         }

         (prog->len_index)++;
      }

   }

   if(flag3 == 1){

      (prog->len_index) = (prog->len_index)-2;

      return true;
   }

return false;

}
