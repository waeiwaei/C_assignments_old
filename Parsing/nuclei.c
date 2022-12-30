#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define ON_ERROR(S) {fprintf(stderr, "%s", S);\
                     exit(EXIT_FAILURE);}

#define MAX_LENGTH 1000


bool program(char* input);
bool instructions(char* input, int* len_index);
bool instruction(char* input, int* len_index);
bool ins_set(char* input, int* len_index);
bool ins_list(char* input, int* len_index);
bool ins_var(char* input, int* len_index);
bool ins_literal(char* input, int* len_index);
bool ins_string(char* input, int* len_index);
bool ins_print(char* array, int* len_index);


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

   //we check if the first character is a '(', then go into instructions
   if(input[0] == '('){

      int len_index = 1;
      //go into the instructions function
      if(instructions(input, &len_index)){
         return true;
      }else{
         return false;
      }

   }
   
   ON_ERROR("Expression does not have an opening bracket");
   return false;

}


//<INSTRCTS> ::= <INSTRCT> <INSTRCTS> | ")"
bool instructions(char* input, int* len_index){

   printf("Instructions - %i, %c \n", *len_index, input[*len_index]);

   //go through the input after the first '(', to identify the subsequent nested '('
   //<INSTRCT> <INSTRCTS>
   while(input[*len_index] != ')'){

      //printf("INSTRUCTIONS - len_index - %i \n", *len_index);
      if(input[*len_index] == '('){
         (*len_index)++;
         instructions(input, len_index);
      }else if(input[*len_index] == ' '){
         //printf("Space\n");
         (*len_index)++;
      }else{
         if(!instruction(input, len_index)){
            return false;
         }
      }
   }

   // | ")"
   if(input[*len_index] == ')'){
      printf("Instructions - ')'\n");
      return true;
   }

   return false;

}

//<INSTRCT> ::= "(" <FUNC> ")"
bool instruction(char* input, int* len_index){

   printf("INSTRUCTION - len_index - %i \n", *len_index);

   char inst[MAX_LENGTH];
   int len = 0;

   while(input[*len_index] != ')'){

      //if there is another nested list - call the instruction function once again
      if(input[*len_index] == '('){
         (*len_index)++;
         instruction(input, len_index);
      }

         //read the instructions given and store in 1D array
         //1. SET & PRINT (IO)
         //2. CAR, CDR, CONS (LIST)
         //3. PLUS & LENGTH (INT)
         //4. LESS, GREATER, EQUAL (BOOL) 
         inst[len] = input[*len_index];

         //IO Functions
         if(strcmp(inst, "SET") == 0){

            (*len_index)++;

            //pass in the rest of the string into the function - and check the values
            ins_set(input, len_index);
            printf("After SET function - %c , %i\n",input[*len_index], *len_index);

         }else if(strcmp(inst, "PRINT") == 0){

            (*len_index) = (*len_index) + 2;

            ins_print(input, len_index);
            printf("After PRINT function - %c , %i\n",input[*len_index], *len_index);

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
      printf("Instructions - Value - %c\n", input[*len_index]);
      return true;
   }else{
      return false;
   }
}


//<SET> ::= "SET" <VAR> <LIST>
bool ins_set(char* input, int* len_index){

   printf("SET - len_index - %i \n", *len_index);

   while(input[*len_index] != ')'){
      if(input[*len_index] == ' '){
         (*len_index)++;
      }

      //check if the VAR grammar is correct
      if(ins_var(input, len_index) == true){

         (*len_index)++;
         //If it is a string literal
         if(ins_list(input, len_index)){
            return true;
         }else{
            return false;
         };

      //error message - input does not conform to grammar
      }else{

         ON_ERROR("Expecting a variable - between A to Z");
         return false;
      }
   }

   return true;

}

//<LIST> ::= <VAR> | <LITERAL> | "NIL" | "(" <RETFUNC> ")"
bool ins_list(char* input, int* len_index){

   printf(" %c, LIST - len_index - %i \n", input[*len_index], *len_index);
   
   //literal
   if(input[*len_index] == 39){
   //pass into a literal function - single quoted list - only numbers brackets and space
      (*len_index)++;
      if(ins_literal(input, len_index)){
         return true;
      }else{
         ON_ERROR("Issue with literal value");
      }

   //check if it is a VAR, NIL
   }else if(input[*len_index] >= 'A' && input[*len_index] <= 'Z'){

      printf("Entered var check\n");
      if(input[*len_index + 1] == ')'){
         printf("VARIABLE\n");
         return true;
      }

   }else if(input[*len_index] == 'N'){
      if(input[*len_index + 1] == 'I'){
         if(input[*len_index + 2] == 'L'){
            printf("it is a NIL\n");
            (*len_index) = (*len_index) + 3;
            return true;
         }
      }
   }

   return false;

}


bool ins_literal(char* input, int* len_index){

   printf("LITERAL - len_index - %i \n", *len_index);

   int flag = 0;

   //only accept space, bracket and numbers
   while(input[*len_index] != 39){
      //if((input[*len_index] >= 48 && input[*len_index] <= 57) || input[*len_index] == 32 || input[*len_index] == 40 || input[*len_index] == 41){
      if((input[*len_index] >= 48 && input[*len_index] <= 57)){         
         flag = 0;
      }else{
         flag = 1;
      }

      (*len_index)++;
   }

   printf("LITERAL FLAG - %i\n", flag);

   if(flag == 1){
      (*len_index)++;
      return false;
   }

   (*len_index)++;
   return true;

}


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
bool ins_print(char* input, int* len_index){

   printf("PRINT - function\n");

   int flag = 0;

   if(ins_string(input, len_index)){
      flag = 1;
   }

   printf("%i\n", flag);

   if(flag == 0){
      if(ins_list(input, len_index)){
         flag = 1;
      }
   }

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
