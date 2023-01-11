#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define ON_ERROR(S) {fprintf(stderr, "%s", S);\
                     exit(EXIT_FAILURE);}

#define MAX_LENGTH 1000


bool list_func(char* input, int* len_index, char* instruct);
bool func(char* input, int* len_index, char* inst);
bool program(char* input);
bool in_out_func(char* input, int* len_index, char* instruc);
bool int_func(char* input, int* len_index, char* instruct);
bool bool_func(char* input, int* len_index, char* instruct);
bool if_func(char* input, int* len_index);
bool loop_func(char* input, int* len_index);
bool instructions(char* input, int* len_index);
bool instruction(char* input, int* len_index);
bool ins_set(char* input, int* len_index);
bool ins_list(char* input, int* len_index);
bool ins_var(char* input, int* len_index);
bool ins_literal(char* input, int* len_index);
bool ins_string(char* input, int* len_index);
bool ins_print(char* array, int* len_index);
void clear_string(char* input);
bool ret_func(char* input, int* len_index);



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
   
   ON_ERROR("Expression does not have an opening bracket\n");
   return false;

}


//<INSTRCTS> ::= <INSTRCT> <INSTRCTS> | ")"
bool instructions(char* input, int* len_index){

   printf("Instructions - %i, %c \n", *len_index, input[*len_index]);

   while(input[*len_index] == ' '){ 
      (*len_index)++;
   }

      if(input[*len_index] == '('){
         (*len_index)++;

         if(!instruction(input, len_index)){            
            return false;
         }

         printf("This is the LEN index here - %i \n\n", *len_index);
   
      if(!instructions(input, len_index)){
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
bool instruction(char* input, int* len_index){

   char inst[MAX_LENGTH] = "";
   int len = 0;
   //int flag = 1;

   while(input[*len_index] != ')'){

         inst[len] = input[*len_index];

         //DEMO5.ncl - How to identify e.g. ((GARBAGE))
         if(input[*len_index + 1] == ')'){
            printf("inst[len] - %s \n\n", inst);
            len++;
            inst[len] = '\0';
            func(input, len_index, inst);
         }

         if(inst[len] == ' '){
              
            inst[len] = '\0';
            func(input, len_index, inst);

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
bool func(char* input, int* len_index, char* inst){

   int flag = 1;

         printf("\n\n\n (%i)Instruction - %s \n\n\n", *len_index, inst);

         //IO Functions
         if(strcmp(inst, "SET") == 0){

            (*len_index)++;

            if(in_out_func(input, len_index, inst)){
               clear_string(inst);
               flag = 0;
            }

         }else if(strcmp(inst, "PRINT") == 0){

            (*len_index)++;

            if(in_out_func(input, len_index, inst)){
               clear_string(inst);
               flag = 0;
            }

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
bool in_out_func(char* input, int* len_index, char* instruc){

   if(strcmp(instruc, "PRINT") == 0){

      ins_print(input, len_index);
      printf("After PRINT function - %c , %i\n",input[*len_index], *len_index);
      (*len_index)--;
      return true;

   }else if(strcmp(instruc, "SET") == 0){
            
      //pass in the rest of the string into the function - and check the values
      ins_set(input, len_index);

      //It should exit at this point with the ) from the SET
      printf("After SET function - %c , %i\n",input[*len_index], *len_index);
      (*len_index)--;

      return true;

   }

   return false;

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

   printf("'%c', LIST - len_index - %i \n", input[*len_index], *len_index);
   
   //literal 
   if(input[*len_index] == 39){
   //pass into a literal function - single quoted list - only numbers brackets and space
      (*len_index)++;
      if(ins_literal(input, len_index)){
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

      if(ret_func(input, len_index)){
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
bool ins_literal(char* input, int* len_index){

   printf("LITERAL - len_index - %i \n", *len_index);

   int flag = 0;

   //only accept space, bracket and numbers
   while(input[*len_index] != 39){
      //if((input[*len_index] >= 48 && input[*len_index] <= 57) || input[*len_index] == 32 || input[*len_index] == 40 || input[*len_index] == 41){
      if(!(input[*len_index] >= 65 && input[*len_index] <= 90)){   
         flag = 0;
      }else{
         flag = 1;
      }

      (*len_index)++;
   }

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
bool ins_print(char* input, int* len_index){

   printf("PRINT - function - %i\n", *len_index);

   int flag = 0;

   if(ins_string(input, len_index)){
      flag = 1;
   }

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

/*
   printf("char %c - %i \n",input[*len_index], *len_index);

   int flag = 0;

   if(input[*len_index] == '"'){
      (*len_index)++;

      while(input[*len_index] != '"'){

         if(input[*len_index] == '\0'){
            break;
         }
         
         (*len_index)++;
      }

      if(input[*len_index] == '"'){
         flag = 1;
      }

   }

   if(flag == 1){
      return true;
   }

   //should be some form of error handling for incomplete "" inputs
   //ON_ERROR("Expecting a closing paranthesis\n");
   return false;*/
}


//clear instruction string to SPACE
void clear_string(char* input){

   int length = strlen(input);

   for(int i = 0; i <= length; i++){
      input[i] = ' ';
   }

}




//<RETFUNC> ::= <LISTFUNC> | <INTFUNC> | <BOOLFUNC> - needed for the PRINT Statement
bool ret_func(char* input, int* len_index){

   printf("\n\n-----------Return Func---------------\n\n");

   char array[MAX_LENGTH] = "";
   int index = 0;

   while(input[*len_index] != ')'){
      array[index] = input[*len_index];

      //LISTFUNC
      if(strcmp(array, "CAR") == 0){
         (*len_index) = (*len_index) + 2;
         printf("\n      ---------CAR instruction triggered - index %i - value - %c \n", *len_index, input[*len_index]);
         list_func(input, len_index, array);

      }else if(strcmp(array, "CDR") == 0){
         (*len_index) = (*len_index) + 2;
         printf("CDR instruction triggered \n");
         list_func(input, len_index, array);

      }else if(strcmp(array, "CONS") == 0){
         (*len_index) = (*len_index) + 2;
         printf("CONS instruction triggered - index %i \n", *len_index);
         list_func(input, len_index, array);

      }

      //INTFUNC
      else if(strcmp(array, "PLUS") == 0){
         (*len_index) = (*len_index) + 2;
         printf("PLUS instruction triggered - index %i \n", *len_index);
         int_func(input, len_index, array);
      
      }else if(strcmp(array, "LENGTH") == 0){
         (*len_index) = (*len_index) + 2;
         printf("LENGTH instruction triggered - index %i \n", *len_index);
         int_func(input, len_index, array);

      } 

      //BOOLFUNC
      else if(strcmp(array, "LESS") == 0){
         (*len_index) = (*len_index) + 2;
         printf("LESS instruction triggered - index %i \n", *len_index);
         bool_func(input, len_index, array);

      }else if(strcmp(array, "GREATER") == 0){
         (*len_index) = (*len_index) + 2;
         printf("GREATER instruction triggered - index %i \n", *len_index);
         bool_func(input, len_index, array);
      
      }else if(strcmp(array,"EQUAL") == 0){
         printf("before - EQUAL instruction triggered - index %i \n", *len_index);
         (*len_index) = (*len_index) + 2;
         printf("after - EQUAL instruction triggered - index %i \n", *len_index);
         bool_func(input, len_index, array);
      
      }

      index++;
      (*len_index)++;
   }

   printf("Last index - %i   %c\n\n", *len_index, input[*len_index]);
   printf("------------------EXIT RET_FUNC---------------------\n\n");


   return true;
}



//<LISTFUNC> ::= "CAR" <LIST> | "CDR" <LIST> | "CONS" <LIST> <LIST>
bool list_func(char* input, int* len_index, char* instruct){
         
while(input[*len_index] != ')'){

   if(strcmp(instruct, "CAR") == 0){

      printf("\n1.list_func - CAR instruction - len index %i - %c\n", *len_index, input[*len_index]);

      if(ins_list(input, len_index)){

         printf("\n2.list_func - CAR instruction - len index %i - %c\n", *len_index, input[*len_index]);

         (*len_index)--;

         return true;
      }

   }else if(strcmp(instruct, "CDR") == 0){

      printf("\n1.list_func - CDR instruction - len index %i - %c\n", *len_index, input[*len_index]);

      if(ins_list(input, len_index)){

         printf("\n2.list_func - CDR instruction - len index %i - %c\n", *len_index, input[*len_index]);

         (*len_index)--;

         return true;
      }

   }else if(strcmp(instruct, "CONS") == 0){

      printf("\n1.list_func - CONS instruction - len index %i \n", *len_index);

      if(ins_list(input, len_index)){
         
         printf("\n2.list_func - CONS instruction - len index %i \n", *len_index);
            
            (*len_index)++;

               if(ins_list(input, len_index)){

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
bool int_func(char* input, int* len_index, char* instruct){

while(input[*len_index] != ')'){

   if(strcmp(instruct,"PLUS") == 0){

      printf("\n1.int_func - PLUS instruction - len index %i \n", *len_index);

      if(ins_list(input, len_index)){
         
         printf("\n2.int_func - PLUS instruction - len index %i \n", *len_index);

         (*len_index)++;

         if(ins_list(input, len_index)){

            (*len_index)--;

            return true;
         }
      }

   }else if(strcmp(instruct,"LENGTH") == 0){

      printf("\n1.int_func - LENGTH instruction - len index %i \n", *len_index);

      if(ins_list(input, len_index)){

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
bool bool_func(char* input, int* len_index, char* instruct){

   printf("BOOL FUNC - index %i, character - %c \n\n", *len_index, input[*len_index]);


while(input[*len_index] != ')'){

   if(input[*len_index] == ' '){
      (*len_index)++;
   }

   if(strcmp(instruct, "LESS") == 0){

      printf("\n1.bool_func - LESS instruction - len index %i \n", *len_index);

      if(ins_list(input, len_index)){
         
         printf("\n2.bool_func - LESS instruction - len index %i \n", *len_index);

         (*len_index)++;

         if(ins_list(input, len_index)){

            (*len_index)--;

            return true;
         }
      }

   }else if(strcmp(instruct, "GREATER") == 0){

      printf("\n1.bool_func - GREATER instruction - len index %i \n", *len_index);

      if(ins_list(input, len_index)){
         
         printf("\n2.bool_func - GREATER instruction - len index %i \n", *len_index);

         (*len_index)++;

         if(ins_list(input, len_index)){

            (*len_index)--;

            return true;
         }
      }
      
   }else if(strcmp(instruct, "EQUAL") == 0){

      printf("\n1.bool_func - EQUAL instruction - len index %i \n", *len_index);

      if(ins_list(input, len_index)){
         
         printf("\n2.bool_func - EQUAL instruction - len index %i \n", *len_index);

         (*len_index)++;

         if(ins_list(input, len_index)){

            (*len_index)--;

            return true;
         }
      }

   }

}

   return false;
}


//<IF> ::= "IF" "(" <BOOLFUNC> ")" "(" <INSTRCTS> "(" <INSTRCTS>
bool if_func(char* input, int* len_index){

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

            if(bool_func(input, len_index, instruct)){
               
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

         if(instructions(input, len_index)){

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

            if(instructions(input, len_index)){

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
bool loop_func(char* input, int* len_index){ 

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

            if(bool_func(input, len_index, instruct)){
               
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

            if(instructions(input, len_index)){

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
