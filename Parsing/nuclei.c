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

//we want to make an array of lisp structures
struct array_lisp{

   int length; //length of the structure
   lisp* structure;
   char name; //A, B, C, D

};

typedef struct array_lisp array_lisp;

struct prog{
   char input[MAX_LENGTH];
   int len_index;
   char instruction[MAX_LENGTH];
   int condition;
   int numberoflisps;
   
   #ifdef INTERPRET
      array_lisp* lisp_array[MAX_LENGTH];
      lisp* lisp_structure;
      lisp* bool_lisp_structure;
      lisp* temp_lisp_structure;
      bool bool_cond;
      char caller_instruction[MAX_LENGTH];
      int most_no_lisps;
      int cons_element;
   #endif
};

typedef struct prog prog;

bool program(prog* prog);
void white_space(prog* prog);
bool list_car(prog* prog);
bool list_cdr(prog* prog);
bool int_plus(prog* prog);
bool list_cons(prog* prog);
bool list_func(prog* prog);
bool bool_less(prog* prog);
bool bool_greater(prog* prog);
bool int_length(prog* prog);
bool bool_equal(prog* prog);
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
   
   #ifdef INTERPRET
      pro->bool_cond = false;
   #endif

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
         printf("Parsed OK!\n");
      }else{
         printf("Error detected\n");
      }

      free(pro);

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


         //free allocated spaces of memory
         #ifdef INTERPRET
            //free each of the lisp_arrays elements
            for(int i = 0; i < prog->numberoflisps; i++){
               if(prog->lisp_array[i]->structure != NULL){
                  lisp_free(&(prog->lisp_array[i]->structure));
               }
            }

            if(prog->lisp_structure != NULL){
               lisp_free(&(prog->lisp_structure));
            }
            
         #endif

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

      while(prog->input[prog->len_index] == ' '){ 
         (prog->len_index)++;
      }

      if(prog->input[prog->len_index] == '('){
         (prog->len_index)++;
      }

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

         
         }else if(strcmp(prog->instruction, "IF") == 0){
            (prog->len_index)++;
            printf("IF has been called\n");

            if(if_func(prog)){

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
            
            #ifdef INTERPRET

               return false;
            #else
               ON_ERROR("Was Expecting a function name\n");
               flag = 1;
               return false;
            #endif

         }

/*         switch(prog->instruction[MAX_LENGTH])
         {
            case "SET":
            (prog->len_index)++;

            if(in_out_func(prog)){
               clear_string(prog);
               flag = 0;
            }            
            break;

            case "PRINT":
            (prog->len_index)++;

            if(in_out_func(prog)){
               clear_string(prog);
               flag = 0;
            }            
            break;

            case "CAR":
            if(ret_func(prog)){
               clear_string(prog);
               flag = 0;
            }
            break;

            case "CDR":
            if(ret_func(prog)){
               clear_string(prog);
               flag = 0;
            }
            break;

            case "CONS":
            if(ret_func(prog)){
               clear_string(prog);
               flag = 0;
            }
            break;

            case "PLUS":
            if(ret_func(prog)){
               clear_string(prog);
               flag = 0;
            }
            break;

            case "LENGTH":
            if(ret_func(prog)){
               clear_string(prog);
               flag = 0;
            }
            break;       

            case "LESS":
            if(bool_func(prog)){
               clear_string(prog);
               flag = 0;
            }
            break;     

            case "GREATER":
            if(bool_func(prog)){
               clear_string(prog);
               flag = 0;
            }
            break;

            case "EQUAL":
            if(bool_func(prog)){
               clear_string(prog);
               flag = 0;
            }
            break;

            case "IF":
            prog->len_index++;
            if(if_func(prog)){
               clear_string(prog);
               flag = 0;
            }
            break;

            case "WHILE":
            prog->len_index++;
            if(loop_func(prog)){
               clear_string(prog);
               flag = 0;
            }
            break;

            
            
            
            default:

               #ifdef INTERPRET

                  return false;
               #else
                  ON_ERROR("Was Expecting a function name\n");
                  flag = 1;
                  return false;
               #endif

         }
*/

   if(flag == 1){
      ON_ERROR("The instruction is unknown\n");
      return false;
   }

   return true;
}




//<IOFUNC> ::= <SET> | <PRINT>
bool in_out_func(prog* prog){

   //identify the isntruction is SET or PRINT
   if(strcmp(prog->instruction, "PRINT") == 0){

      ins_print(prog);
   
      (prog->len_index)--;
      return true;

   }else if(strcmp(prog->instruction, "SET") == 0){
      
      //pass in the prog struct into set function
      ins_set(prog);

      (prog->len_index)--;
      return true;

   }

   return false;

}

//creates and stores variables of lisps in a lisp_array
bool ins_set(prog* prog){

   #ifdef INTERPRET
      if(prog->lisp_array[prog->numberoflisps] == NULL){
         prog->lisp_array[prog->numberoflisps] = (array_lisp*)ncalloc(1, sizeof(array_lisp));
      }
      strcpy(prog->caller_instruction, prog->instruction);
   #endif

   while(prog->input[prog->len_index] != ')'){

      if(prog->input[prog->len_index] == ' '){
         (prog->len_index)++;
      }

      //checks if character is a variable and includes it in lisp_array
      if(ins_var(prog)){
      
         (prog->len_index)++;

      }else{

         ON_ERROR("Expecting a variable - between A to Z");
         return false;
      }

      //check the value of the element stores is a list
      if(ins_list(prog)){

         return true;

      }else{

         return false;
      }

   }

   return false;

}


void white_space(prog* prog){

   while(prog->input[prog->len_index] == ' '){
      prog->len_index++;
   }
}

//<LIST> ::= <VAR> | <LITERAL> | "NIL" | "(" <RETFUNC> ")"
bool ins_list(prog* prog){

   white_space(prog);

   //to store the value of the instruction called
   #ifdef INTERPRET
      static char temp_caller_ins[MAX_LENGTH];

      if(strcmp(prog->instruction, "SET") == 0){   
         strcpy(temp_caller_ins, prog->instruction);
      }
   #endif

   printf("'%c', LIST - len_index - %i \n", prog->input[prog->len_index], prog->len_index);

   //Checks if the input is a literal value (indicated by single quotation mark) 
   if(prog->input[prog->len_index] == 39){

      (prog->len_index)++;

      if(ins_literal(prog)){
         return true;
      }else{
         ON_ERROR("Issue with literal value");
      }

   //Checks if the value is NIL
   }else if(prog->input[prog->len_index] == 'N' && prog->input[prog->len_index + 1] == 'I' && prog->input[prog->len_index + 2] == 'L'){

      //Returns null space in memory to the caller instruction
      #ifdef INTERPRET
         prog->lisp_structure = NULL;
      #endif

      (prog->len_index) = (prog->len_index) + 3;

      printf("LEN INDEX - %i %c\n", prog->len_index, prog->input[prog->len_index]);

      return true;

   //Checks if it is a VAR - if there is a space before the character
   }else if(prog->input[prog->len_index] >= 'A' && prog->input[prog->len_index] <= 'Z'){

      printf("Entered var check %i\n", prog->len_index);
      if(prog->input[prog->len_index - 1] == ' '){
         
         #ifdef INTERPRET
         
            int flag = 0;
            
            //find the variable lisp value from lisp_array
            for(int i = 0; i < prog->numberoflisps; i++){
               if(prog->lisp_array[i]->name == prog->input[prog->len_index]){
                  prog->lisp_structure = prog->lisp_array[i]->structure;
                  flag = 1;
               }
            }

            //set the value of the lisp_array at the index with the variable lisp value
            if(strcmp(prog->instruction, "SET")==0){
               prog->lisp_array[prog->numberoflisps]->structure = lisp_copy(prog->lisp_structure);
               prog->numberoflisps++;
               flag = 1;
            }

            //set the value of the lisp_array at the index with the variable lisp value
            if(strcmp(prog->instruction, "PLUS")==0){
               prog->lisp_array[prog->numberoflisps]->structure = lisp_copy(prog->lisp_structure);
               flag = 1;
            }

            //set the value of the lisp_structure at the index with the variable lisp values
            if(strcmp(prog->caller_instruction, "WHILE") == 0 || strcmp(prog->instruction, "CAR") == 0 
               || strcmp(prog->instruction, "CDR") == 0 || strcmp(prog->instruction, "PRINT") == 0){
               prog->lisp_structure = lisp_copy(prog->lisp_structure);
               flag = 1;
            }
/*
            if(strcmp(prog->instruction, "CAR") == 0 || strcmp(prog->instruction, "CDR") == 0 ){
               prog->lisp_structure = lisp_copy(prog->lisp_structure);
            }

            if(strcmp(prog->instruction, "PRINT") == 0 || strcmp(prog->instruction, "CDR") == 0 ){
               prog->lisp_structure = lisp_copy(prog->lisp_structure);
               flag = 1;
            }
*/

            //Return empty lisp if no matching variable name was found
            if(flag == 0){
               
               prog->lisp_structure = NULL;
            }

         #endif

         (prog->len_index)++;
         return true;
      }else{

         printf("failed var check\n");
      }
   
   
   //Return function call
   }else if(prog->input[prog->len_index] == '('){

      printf("RETFUNC instruction triggered - index %i \n", prog->len_index);

      (prog->len_index)++;

      white_space(prog);

      //store the value of the current instruction      
      #ifdef INTERPRET 
         strcpy(prog->caller_instruction, prog->instruction);
      #endif

      //pass prog into return function
      if(ret_func(prog)){
         
         if(prog->input[prog->len_index] == ')'){
            printf("Value after returning from ret_func - %c  , %i \n", prog->input[prog->len_index], prog->len_index);
            (prog->len_index)++;  

            //return the previously called instruction into caller_instruction element
            #ifdef INTERPRET               
               if(strcmp(temp_caller_ins, "SET") == 0){
                  strcpy(prog->caller_instruction, temp_caller_ins);
               } 
            #endif

            return true;
         }


      }

   }

   return false;

}

//<LITERAL> ::= Single-quoted list e.g. ’(1)’, ’(1 2 3 (4 5))’, or ’2’
bool ins_literal(prog* prog){

   printf("LITERAL - len_index - %i \n", prog->len_index);

   #ifdef INTERPRET


      char str[MAX_LENGTH];
      int len_literal = 0;
      int flag_bracket = 0;

      //checks if the previous input was a LEFT_BRACKET, indicating e.g. (1)
      if(prog->input[prog->len_index] == '('){
         flag_bracket = 1;

         //stores the LEFT_BRACKET in a string which stores literal value
         str[len_literal] = prog->input[prog->len_index];
         prog->len_index++;
         len_literal++;
      }

      //if the value at previous index is not LEFT_BRACKET, we will need to include a LEFT_BRACKET
      if(prog->input[prog->len_index - 1] != '(' && flag_bracket != 1){
         str[len_literal] = '(';
         len_literal++;
      }

      //retrieve literal portion and put it in a string ( (39, ASCII code) = single quotation)
      while(prog->input[prog->len_index] != 39){

         str[len_literal] = prog->input[prog->len_index];
         
         len_literal++;
         prog->len_index++;

      }

      //identify if initial value was bracket - include RIGHT_BRACKET
      if(flag_bracket != 1){
         str[len_literal] = ')';
      }

      len_literal++;
      str[len_literal] = '\0';

   //check for the caller instruction and instructions, and use lisp_fromstring to convert literal to lisp
   if(strcmp(prog->instruction, "SET") == 0){
      
      //checks if lisp_fromstring produces a non-NULL value
      if(lisp_fromstring(str) != NULL){
         prog->lisp_array[prog->numberoflisps]->structure = lisp_fromstring(str);
         prog->lisp_array[prog->numberoflisps]->structure->choice = true;
         prog->lisp_array[prog->numberoflisps]->length = lisp_length(prog->lisp_array[prog->numberoflisps]->structure);
         
         prog->numberoflisps++;
         prog->len_index++;

      }else{
         printf("Unable\n");
         return false;
      }

   }else if(strcmp(prog->instruction, "PLUS") == 0){
      prog->lisp_structure = lisp_fromstring(str);
      prog->len_index++;

   }else if(strcmp(prog->instruction, "CONS") == 0){
      prog->lisp_structure = lisp_fromstring(str);
      prog->len_index++;

   }else if(strcmp(prog->caller_instruction, "WHILE") == 0){
      prog->lisp_structure = lisp_fromstring(str);         

   }else if(strcmp(prog->instruction, "EQUAL") == 0 || (strcmp(prog->instruction, "GREATER") == 0)|| (strcmp(prog->instruction, "LENGTH") == 0)){
      prog->lisp_structure = lisp_fromstring(str);

      if(flag_bracket == 1){
         prog->len_index++; 
      }

   }else if((strcmp(prog->caller_instruction, "EQUAL") == 0)){
      prog->lisp_structure = lisp_fromstring(str);
      
      if(flag_bracket == 1){
         prog->len_index++; 
      }
   }

      return true;

#else

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

   if(flag == 1){
      (prog->len_index)++;
      return false;
   }


#endif

   (prog->len_index)++;
   return true;

}



//<VAR> ::= [A-Z]
bool ins_var(prog* prog){

   printf("VARIABLE - len_index - %i \n", prog->len_index);

   //identify if the prog->index is a variable - between A to Z and has a SPACE one index before
   if(prog->input[prog->len_index] >= 'A' && prog->input[prog->len_index] <= 'Z'){
      if(prog->input[prog->len_index - 1] == ' '){
         

         //initialise and give name's to the new index's in the array
         #ifdef INTERPRET

            //stores the number of lisps in the array - as the numberoflisp index may change each time
            //we will need to call an older value in the lisp 
            static int current_numberoflisp;

            if(prog->numberoflisps >= current_numberoflisp){
               current_numberoflisp = prog->numberoflisps;
            }

            //after we have identified that variable, we will store it inside numberoflisp
            for(int i = 0; i <= prog->numberoflisps; i++){
               if(prog->input[prog->len_index] == prog->lisp_array[i]->name){
                  
                  prog->numberoflisps = i;
                  break;

               }else{
                  prog->numberoflisps = current_numberoflisp;
                  prog->most_no_lisps = current_numberoflisp;
               }
            }

            //we will then give a name, to the lisp at index numberoflisp - from VAR name
            prog->lisp_array[prog->numberoflisps]->name = prog->input[prog->len_index];
 

         #else
            (prog->len_index)++;
         #endif

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

   //keep track of the elements we will need to print
   #ifdef INTERPRET
      int initial_index = prog->len_index;
   #endif

   if(ins_string(prog)){
      prog->len_index++;
      flag = 1;
   }

   if(flag == 0){
      if(ins_list(prog)){
         flag = 1;
      }
   }


   #ifdef INTERPRET

      int len_string = 0;
      int flag_string = 0;
      char str[MAX_LENGTH];


      //identify if there is a string whihin the double quotations
      if(prog->input[initial_index] == '"'){
         initial_index++;

            flag_string = 1;

         //popualate the string value - nested within the double quotations
         while(prog->input[initial_index] != '"'){
            str[len_string] = prog->input[initial_index];
            
            len_string++;
            initial_index++;
         }
      }

      str[len_string] = '\0';

      //check whether to print str or lisp_strucutre (which holds the literal of variable value of lisp)
      if(prog->lisp_structure != NULL && flag_string == 0){

         //checks if lisp_structure contains CAR or CDR vales, or if element was derived from CONS - set choice to false
         if(prog->lisp_structure->car != NULL || prog->lisp_structure->cdr != NULL || prog->cons_element == 1){
            prog->lisp_structure->choice = false;
         
         //this element is not a lisp element
         }else{
            prog->lisp_structure->choice = true;
         }

         //use lisp to string to convert it and store it in the string element
         lisp_tostring(prog->lisp_structure, str);
         printf("PRINT FUNCTION (FINAL) - %s\n\n\n", str);
         
      }
      
      //else if there is a string, we will print the string
      else if(flag_string == 1){

         printf("Print FUNCTION (FINAL) - %s\n\n\n", str);

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
  
   int flag = 0;
   
   //identify if it is nested within DOUBLE_QUOTATIONS
   if(prog->input[prog->len_index] == '"'){
      (prog->len_index)++;

      while(prog->input[prog->len_index] != '"'){

         flag = 1;

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
   
   #ifdef INTERPRET
   #else
      char old_function[MAX_LENGTH];
      strcpy(old_function, prog->instruction);
   #endif

   printf("\n\n-----------Return Func---------------\n\n");

   int ret_index = 0;

   clear_string(prog);

   //populate prog->instruction, with the instruction string
   while(prog->input[prog->len_index] != ')'){
      
      prog->instruction[ret_index] = prog->input[prog->len_index];

      if(prog->instruction[ret_index] == ' '){
         break;
      }

      ret_index++;
      (prog->len_index)++;
   }

      prog->instruction[ret_index] = '\0';
      printf("Instruction %s\n\n", prog->instruction);


      //identify the instruction call, based on prog->instruction
      //LIST FUNCTIONS - CAR, CDR, CONS
      if(strcmp(prog->instruction, "CAR") == 0){

         prog->len_index++;
         list_func(prog);

      }else if(strcmp(prog->instruction, "CDR") == 0){
        
         prog->len_index++; 
         list_func(prog);

      }else if(strcmp(prog->instruction, "CONS") == 0){

         prog->len_index++;
         list_func(prog);

      }

      //INTFUNC - PLUS, LENGTH
      else if(strcmp(prog->instruction, "PLUS") == 0){
         
         (prog->len_index)++;
         //#endif
         printf("PLUS instruction triggered - index %i \n", prog->len_index);
         int_func(prog);
      
      }else if(strcmp(prog->instruction, "LENGTH") == 0){
         (prog->len_index)++;
         printf("LENGTH instruction triggered - index %i \n", prog->len_index);
         int_func(prog);

      } 

      //BOOLFUNC - LESS, GREATER, EQUAL
      else if(strcmp(prog->instruction, "LESS") == 0){
         (prog->len_index)++;

         #ifdef INTERPRET
         #else

            if(strcmp(old_function, "SET") == 0){
               prog->condition = 1;
            }

         #endif

         bool_func(prog);

      }else if(strcmp(prog->instruction, "GREATER") == 0){

         bool_func(prog);
      
      }else if(strcmp(prog->instruction,"EQUAL") == 0){
         //(prog->len_index) = (prog->len_index) + 2;
         prog->len_index++;
         bool_func(prog);
      
      }




   printf("Last index - %i   %c\n\n", prog->len_index, prog->input[prog->len_index]);
   printf("------------------EXIT RET_FUNC---------------------\n\n");


   return true;
}

bool list_car(prog* prog){
     
   //first 
   if(ins_list(prog)){
      
      #ifdef INTERPRET
         prog->lisp_structure = lisp_car(prog->lisp_structure);
         printf("%i\n\n\n", lisp_getval(prog->lisp_structure));
      #endif

      return true;
   }

   return false;

}



bool list_cdr(prog* prog){

   if(ins_list(prog)){

      #ifdef INTERPRET
         prog->lisp_structure = lisp_cdr(prog->lisp_structure);
      #endif

      return true;
   }

   return false;  
}


bool list_cons(prog* prog){

#ifdef INTERPRET
   lisp* temp1;
   lisp* temp2;
#endif

   if(ins_list(prog)){

      #ifdef INTERPRET 
         temp1 = lisp_copy(prog->lisp_structure);
         printf("temp 1 - %i", lisp_getval(temp1));
      #endif

         (prog->len_index)++;

         if(ins_list(prog)){

            #ifdef INTERPRET 
               temp2 = lisp_copy(prog->lisp_structure);

            #endif

            #ifdef INTERPRET

               if(strcmp(prog->caller_instruction, "SET") == 0){
                  prog->lisp_structure = lisp_cons(temp1, temp2);

                  if(prog->lisp_array[prog->numberoflisps] != NULL){
                     prog->lisp_array[prog->numberoflisps]->structure = lisp_copy(prog->lisp_structure);
                     prog->cons_element = 1;
                     prog->numberoflisps++;
                  }else{
                     prog->numberoflisps--;
                     prog->lisp_array[prog->numberoflisps]->structure = lisp_copy(prog->lisp_structure);
                     prog->numberoflisps++;
                  }

               }

               if(strcmp(prog->caller_instruction, "CONS") == 0){
                  prog->lisp_structure = lisp_cons(temp1, temp2);
               }
            
            #endif 
         }  

      return true;

   }

   return false;
}



//<LISTFUNC> ::= "CAR" <LIST> | "CDR" <LIST> | "CONS" <LIST> <LIST>
bool list_func(prog* prog){

//identify as long as it is
while(prog->input[prog->len_index] != ')'){

   if(strcmp(prog->instruction, "CAR") == 0){

/*
      printf("\n1.list_func - CAR instruction - len index %i - %c\n", prog->len_index, prog->input[prog->len_index]);

      //first 
      if(ins_list(prog)){
         
         #ifdef INTERPRET
            prog->lisp_structure = lisp_car(prog->lisp_structure);
            printf("%i\n\n\n", lisp_getval(prog->lisp_structure));
         #endif

         printf("\n2.list_func - CAR instruction - len index %i - %c\n", prog->len_index, prog->input[prog->len_index]);

         //(prog->len_index)--;

         return true;
      }

*/

      if(list_car(prog)){
         return true;
      }


   }else if(strcmp(prog->instruction, "CDR") == 0){

/*
      printf("\n1.list_func - CDR instruction - len index %i - %c\n", prog->len_index, prog->input[prog->len_index]);

      if(ins_list(prog)){

         printf("\n2.list_func - CDR instruction - len index %i - %c\n", prog->len_index, prog->input[prog->len_index]);

         #ifdef INTERPRET
            prog->lisp_structure = lisp_cdr(prog->lisp_structure);
            //printf("%i\n\n\n", lisp_getval(prog->lisp_structure));
         #endif

         //(prog->len_index)--;

         return true;
      }
*/


      if(list_cdr(prog)){
         return true;
      }


   }else if(strcmp(prog->instruction, "CONS") == 0){

/*
      printf("\n1.list_func - CONS instruction - len index %i \n", prog->len_index);

      if(ins_list(prog)){

         #ifdef INTERPRET 
            temp1 = lisp_copy(prog->lisp_structure);
            printf("temp 1 - %i", lisp_getval(temp1));
         #endif

         printf("\n2.list_func - CONS instruction - len index %i \n", prog->len_index);
            
            (prog->len_index)++;

            printf("line 816 - %i\n\n", prog->len_index);

               if(ins_list(prog)){

                     #ifdef INTERPRET 
                        temp2 = lisp_copy(prog->lisp_structure);
                        //printf("temp2 - %i", lisp_getval(temp2));

                     #endif

                     //(prog->len_index)--;

                     #ifdef INTERPRET
                        //char str[MAX_LENGTH];

                        if(strcmp(prog->caller_instruction, "SET") == 0){
                           prog->lisp_structure = lisp_cons(temp1, temp2);

                           if(prog->lisp_array[prog->numberoflisps] != NULL){
                              prog->lisp_array[prog->numberoflisps]->structure = lisp_copy(prog->lisp_structure);
                              prog->cons_element = 1;
                              prog->numberoflisps++;
                           }else{
                              prog->numberoflisps--;
                              prog->lisp_array[prog->numberoflisps]->structure = lisp_copy(prog->lisp_structure);
                              prog->numberoflisps++;
                           }

                        }

                        if(strcmp(prog->caller_instruction, "CONS") == 0){
                           //lisp_tostring(prog->lisp_structure, str);

                           prog->lisp_structure = lisp_cons(temp1, temp2);
                        }
                     
                     #endif 
                     }
                     return true;
               }*/

         if(list_cons(prog)){
            return true;
         }

      }

   white_space(prog);

}

   if(prog->input[prog->len_index] == ')'){
     
      printf("list_func returns ') at index %i'\n", prog->len_index);

   }

   return true;
}


bool int_plus(prog* prog){

#ifdef INTERPRET
   lisp* temp1;
   static lisp* temp2;
   static int accum = 0;
#endif

      printf("\n1.int_func - PLUS instruction - len index %i \n", prog->len_index);

      if(ins_list(prog)){
         
         #ifdef INTERPRET
            temp1 = lisp_copy(prog->lisp_structure);
         #endif

         printf("\n2.int_func - PLUS instruction - len index %i \n", prog->len_index);

         (prog->len_index)++;

         if(ins_list(prog)){
            
            #ifdef INTERPRET

               if(strcmp(prog->caller_instruction, "SET") == 0){
                  temp2 = lisp_copy(prog->lisp_structure);

                  //stores the atomtype for both list minus each other
                  accum = temp2->atomtype + temp1->atomtype;
                  
                  prog->lisp_structure = lisp_cons(lisp_atom(accum), NULL);

                  prog->lisp_array[prog->numberoflisps]->structure = lisp_copy(prog->lisp_structure);
                  prog->numberoflisps++;
                  //printf("\n PLUS -1 : %i\n", prog->lisp_structure->atomtype);
               }

            #endif

            return true;
         }
      }

   return false;
}



bool int_length(prog* prog){

   if(ins_list(prog)){

      #ifdef INTERPRET

      if(strcmp(prog->caller_instruction, "SET") == 0){
         prog->lisp_structure->choice = false;
         prog->lisp_array[prog->numberoflisps]->structure = lisp_cons(lisp_atom(lisp_length(prog->lisp_structure)), NULL);
         prog->lisp_array[prog->numberoflisps]->structure->choice = true;
         prog->numberoflisps++;
      }else if(strcmp(prog->caller_instruction, "EQUAL") == 0){

         //check if a exits in the database if not we set the choice to true
         if(prog->lisp_structure != NULL){
            prog->lisp_structure->choice = false;
         }

         prog->lisp_structure = lisp_cons(lisp_atom(lisp_length(prog->lisp_structure)), NULL);
      }

      #endif

      return true;
   }

   return false;
}







//<INTFUNC> ::= "PLUS" <LIST> <LIST> | "LENGTH" <LIST>
bool int_func(prog* prog){

/*
#ifdef INTERPRET
lisp* temp1;
static lisp* temp2;
static int accum = 0;
#endif
*/

while(prog->input[prog->len_index] != ')'){

   if(strcmp(prog->instruction,"PLUS") == 0){

/*     printf("\n1.int_func - PLUS instruction - len index %i \n", prog->len_index);

      if(ins_list(prog)){
         
         #ifdef INTERPRET
            temp1 = lisp_copy(prog->lisp_structure);
         #endif

         printf("\n2.int_func - PLUS instruction - len index %i \n", prog->len_index);

         (prog->len_index)++;

         if(ins_list(prog)){
            
            #ifdef INTERPRET

            if(strcmp(prog->caller_instruction, "SET") == 0){
               temp2 = lisp_copy(prog->lisp_structure);

               //stores the atomtype for both list minus each other
               accum = temp2->atomtype + temp1->atomtype;

               lisp* temp = lisp_cons(lisp_atom(accum), NULL);
               prog->lisp_structure = lisp_copy(temp); 

               prog->lisp_array[prog->numberoflisps]->structure = lisp_copy(prog->lisp_structure);
               prog->numberoflisps++;
               printf("\n PLUS -1 : %i\n", prog->lisp_structure->atomtype);
            }

            #endif

            return true;
         }
      }*/

      if(int_plus(prog)){
         return true;
      }


   }else if(strcmp(prog->instruction,"LENGTH") == 0){

 /*     printf("\n1.int_func - LENGTH instruction - len index %i \n", prog->len_index);

      if(ins_list(prog)){

         #ifdef INTERPRET

         if(strcmp(prog->caller_instruction, "SET") == 0){
            prog->lisp_structure->choice = false;
            prog->lisp_array[prog->numberoflisps]->structure = lisp_cons(lisp_atom(lisp_length(prog->lisp_structure)), NULL);
            prog->lisp_array[prog->numberoflisps]->structure->choice = true;
            prog->numberoflisps++;
         }else if(strcmp(prog->caller_instruction, "EQUAL") == 0){

            //check if a exits in the database if not we set the choice to true
            if(prog->lisp_structure != NULL){
               prog->lisp_structure->choice = false;
            }

            prog->lisp_structure = lisp_cons(lisp_atom(lisp_length(prog->lisp_structure)), NULL);
         }


         #endif

         return true;
      }*/


      if(int_length(prog)){
         return true;
      }
   }

   white_space(prog);

}

   return true;
}



bool bool_less(prog* prog){

#ifdef INTERPRET
   lisp* temp1;
   lisp* temp2;
#endif

   if(ins_list(prog)){
      
      #ifdef INTERPRET
         temp1 = lisp_copy(prog->lisp_structure);

         //go on to the next index after the literal or variable
         prog->len_index++;

         //for eliminating spaces inbetween
         white_space(prog);

      #endif

      if(ins_list(prog)){

         #ifdef INTERPRET
            temp2 = lisp_copy(prog->lisp_structure);
         #endif

         (prog->len_index)--;

         #ifdef INTERPRET

         //should compare the values
        /* if(strcmp(prog->caller_instruction, "WHILE") == 0){

            if(temp1->atomtype < temp2->atomtype){

               prog->lisp_structure = lisp_cons(lisp_atom(1), NULL);

            }else{

               prog->lisp_structure = lisp_cons(lisp_atom(0), NULL);

            }


         //should compare between the two and return output 1 or 0
         }if(strcmp(prog->caller_instruction, "IF") == 0){

            if(temp1->atomtype < temp2->atomtype){

               prog->lisp_structure = lisp_cons(lisp_atom(1), NULL);

            }else{

               prog->lisp_structure = lisp_cons(lisp_atom(0), NULL);

            }


         //should compare between the two and return output 1 or 0
         }*/
         
         
         
         
         
         if(strcmp(prog->caller_instruction, "SET") == 0){
               
            if(temp1->atomtype < temp2->atomtype){

               prog->lisp_array[prog->numberoflisps]->structure = lisp_cons(lisp_atom(1), NULL);

            }else{

               prog->lisp_array[prog->numberoflisps]->structure = lisp_cons(lisp_atom(0), NULL);

            }

            prog->len_index++;
            prog->numberoflisps++;
         
         }else if(strcmp(prog->caller_instruction, "IF") == 0 || strcmp(prog->caller_instruction, "WHILE") == 0){
            if(temp1->atomtype < temp2->atomtype){

               prog->lisp_structure = lisp_cons(lisp_atom(1), NULL);

            }else{

               prog->lisp_structure = lisp_cons(lisp_atom(0), NULL);

            }
         }

         #endif

         #ifdef INTERPRET
         #else
      
         if(prog->condition == 1){
            prog->len_index++;
         }

      #endif

         return true;
      }
   }

   return false;
}

bool bool_greater(prog* prog){

#ifdef INTERPRET
   lisp* temp1;
   lisp* temp2;
#endif

   if(ins_list(prog)){

      #ifdef INTERPRET
         temp1 = lisp_copy(prog->lisp_structure);
      #endif


      (prog->len_index)++;

      //for eliminating spaces inbetween
      white_space(prog);

      if(ins_list(prog)){

         #ifdef INTERPRET

            temp2 = lisp_copy(prog->lisp_structure);

         //should compare the values
         if(strcmp(prog->caller_instruction, "SET") == 0){

            if(temp1->atomtype < temp2->atomtype){
               prog->lisp_array[prog->numberoflisps]->structure = lisp_cons(lisp_atom(1), NULL);
            }else{
               prog->lisp_array[prog->numberoflisps]->structure = lisp_cons(lisp_atom(0), NULL);
            }

            prog->numberoflisps++;
         }else if(strcmp(prog->caller_instruction, "IF") == 0 || strcmp(prog->caller_instruction, "WHILE") == 0){
            
            if(temp1->atomtype > temp2->atomtype){
               prog->lisp_structure = lisp_cons(lisp_atom(1), NULL);
            }else{
               prog->lisp_structure = lisp_cons(lisp_atom(0), NULL);
            }

         }
         #endif

         return true;
      }
   }

   return false;
}


bool bool_equal(prog* prog){

#ifdef INTERPRET
   lisp* temp1;
   lisp* temp2;
#endif

   if(ins_list(prog)){
      
      #ifdef INTERPRET
         temp1 = lisp_copy(prog->lisp_structure);
      #endif         

      (prog->len_index)++;

      //for eliminating spaces inbetween
      white_space(prog);

      if(ins_list(prog)){

         #ifdef INTERPRET

         temp2 = lisp_copy(prog->lisp_structure);

         //should compare the values
         if(strcmp(prog->caller_instruction, "WHILE") == 0 || strcmp(prog->caller_instruction, "IF") == 0){

            if(temp1->atomtype == temp2->atomtype){
               prog->lisp_structure = lisp_cons(lisp_atom(1), NULL);
            }else{
               prog->lisp_structure = lisp_cons(lisp_atom(0), NULL);
            }

         }if(strcmp(prog->caller_instruction, "SET") == 0 && strcmp(prog->instruction, "CAR") == 0 ){

            if(temp1->atomtype == temp2->atomtype){
               prog->lisp_structure = lisp_cons(lisp_atom(1), NULL);
            }else{
               prog->lisp_structure = lisp_cons(lisp_atom(0), NULL);
            }
            
         }else if(strcmp(prog->caller_instruction, "SET") == 0){

            if(temp1->atomtype == temp2->atomtype){
               prog->lisp_array[prog->numberoflisps]->structure = lisp_cons(lisp_atom(1), NULL);
            }else{
               prog->lisp_array[prog->numberoflisps]->structure = lisp_cons(lisp_atom(0), NULL);
            }

               prog->numberoflisps++;
               prog->len_index++;
            
         }else{

            if(temp1->atomtype == temp2->atomtype){
               prog->lisp_structure = lisp_cons(lisp_atom(1), NULL);
            }else{
               prog->lisp_structure = lisp_cons(lisp_atom(0), NULL);
            }
         }

            //prog->len_index++;

         #endif

         return true;
      }
   }

   return false;
}


//<BOOLFUNC> ::= "LESS" <LIST> <LIST> | "GREATER" <LIST> <LIST> | "EQUAL" <LIST> <LIST>
bool bool_func(prog* prog){

/*#ifdef INTERPRET
   lisp* temp1;
   lisp* temp2;
#endif
*/

printf("BOOL FUNC - index %i, character - %c \n\n", prog->len_index, prog->input[prog->len_index]);

while(prog->input[prog->len_index] != ')'){

   if(prog->input[prog->len_index] == ' '){
      (prog->len_index)++;
   }

   if(strcmp(prog->instruction, "LESS") == 0){


/*
      printf("\n1.bool_func - LESS instruction - len index %i \n", prog->len_index);

      if(ins_list(prog)){
         
         #ifdef INTERPRET
               temp1 = lisp_copy(prog->lisp_structure);
         #endif

         printf("\n2.bool_func - LESS instruction - len index %i \n", prog->len_index);

         #ifdef INTERPRET

         //go on to the next index after the literal or variable
         prog->len_index++;

         //for eliminating spaces inbetween
         while(prog->input[prog->len_index] == ' '){
            prog->len_index++;
         }

         #endif

         if(ins_list(prog)){

            #ifdef INTERPRET
               temp2 = lisp_copy(prog->lisp_structure);
            #endif

            (prog->len_index)--;

            #ifdef INTERPRET

            //should compare the values
            if(strcmp(prog->caller_instruction, "WHILE") == 0){

               if(temp1->atomtype < temp2->atomtype){

                  prog->lisp_structure = lisp_cons(lisp_atom(1), NULL);

               }else{

                  prog->lisp_structure = lisp_cons(lisp_atom(0), NULL);

               }


            //should compare between the two and return output 1 or 0
            }if(strcmp(prog->caller_instruction, "IF") == 0){

               if(temp1->atomtype < temp2->atomtype){

                  prog->lisp_structure = lisp_cons(lisp_atom(1), NULL);

               }else{

                  prog->lisp_structure = lisp_cons(lisp_atom(0), NULL);

               }


            //should compare between the two and return output 1 or 0
            }else if(strcmp(prog->caller_instruction, "SET") == 0){
                  
               if(temp1->atomtype < temp2->atomtype){

                  prog->lisp_array[prog->numberoflisps]->structure = lisp_cons(lisp_atom(1), NULL);

               }else{

                  prog->lisp_array[prog->numberoflisps]->structure = lisp_cons(lisp_atom(0), NULL);

               }

               prog->len_index++;

               prog->numberoflisps++;
            }

            #endif

            #ifdef INTERPRET
            #else
         
            if(prog->condition == 1){
               prog->len_index++;
            }

         #endif

            return true;
         }
      }
   */

      if(bool_less(prog)){
         return true;
      }


   }else if(strcmp(prog->instruction, "GREATER") == 0){

 /*     printf("\n1.bool_func - GREATER instruction - len index %i \n", prog->len_index);

      if(ins_list(prog)){
   
         #ifdef INTERPRET
            
            temp1 = lisp_copy(prog->lisp_structure);
 
         #endif

         printf("\n2.bool_func - GREATER instruction - len index %i \n", prog->len_index);

         (prog->len_index)++;

         //for eliminating spaces inbetween
         while(prog->input[prog->len_index] == ' '){
            prog->len_index++;
         }

         if(ins_list(prog)){

            #ifdef INTERPRET

               temp2 = lisp_copy(prog->lisp_structure);

            #endif

            #ifdef INTERPRET
            //should compare the values
            if(strcmp(prog->caller_instruction, "WHILE") == 0){

               if(temp1->atomtype > temp2->atomtype){
                  prog->lisp_structure = lisp_cons(lisp_atom(1), NULL);
               }else{
                  prog->lisp_structure = lisp_cons(lisp_atom(0), NULL);
               }

            }if(strcmp(prog->caller_instruction, "IF") == 0){

               if(temp1->atomtype > temp2->atomtype){
                  prog->lisp_structure = lisp_cons(lisp_atom(1), NULL);
               }else{
                  prog->lisp_structure = lisp_cons(lisp_atom(0), NULL);
               }

            }else if(strcmp(prog->caller_instruction, "SET") == 0){

               if(temp1->atomtype < temp2->atomtype){
                  prog->lisp_array[prog->numberoflisps]->structure = lisp_cons(lisp_atom(1), NULL);
               }else{
                  prog->lisp_array[prog->numberoflisps]->structure = lisp_cons(lisp_atom(0), NULL);
               }

               prog->numberoflisps++;
            }
            #endif

            return true;
         }
      }*/

      if(bool_greater(prog)){
         return true;
      }
      
   }else if(strcmp(prog->instruction, "EQUAL") == 0){

/*      printf("\n1.bool_func - EQUAL instruction - len index %i \n", prog->len_index);

      if(ins_list(prog)){
         
         #ifdef INTERPRET
            temp1 = lisp_copy(prog->lisp_structure);
         #endif         

         printf("\n2.bool_func - EQUAL instruction - len index %i \n", prog->len_index);

         (prog->len_index)++;

         //for eliminating spaces inbetween
         while(prog->input[prog->len_index] == ' '){
            prog->len_index++;
         }

         if(ins_list(prog)){

            #ifdef INTERPRET

            temp2 = lisp_copy(prog->lisp_structure);

            //should compare the values
            if(strcmp(prog->caller_instruction, "WHILE") == 0){

               if(temp1->atomtype == temp2->atomtype){
                  prog->lisp_structure = lisp_cons(lisp_atom(1), NULL);
               }else{
                  prog->lisp_structure = lisp_cons(lisp_atom(0), NULL);
               }

            }if(strcmp(prog->caller_instruction, "IF") == 0){

               if(temp1->atomtype == temp2->atomtype){
                  prog->lisp_structure = lisp_cons(lisp_atom(1), NULL);
               }else{
                  prog->lisp_structure = lisp_cons(lisp_atom(0), NULL);
               }

            }else{

               if(temp1->atomtype == temp2->atomtype){
                  prog->lisp_structure = lisp_cons(lisp_atom(1), NULL);
               }else{
                  prog->lisp_structure = lisp_cons(lisp_atom(0), NULL);
               }
            }

            #endif

            return true;
         }
      }*/

      if(bool_equal(prog)){
         return true;
      }

   }

}

   return false;
}






//         (IF (CONDITION) ((IF-TRUE-INSTRCTS)) ((IF_FALSE_INSTRCTS)))
//<IF> ::= "IF" "(" <BOOLFUNC> ")" "(" <INSTRCTS> "(" <INSTRCTS>
bool if_func(prog* prog){

#ifdef INTERPRET

   int len = 0;
   //int flag = 0;
   lisp* store_cond;
   static int store_len = 0;
   static int lengh;
   int else_cond_index = 0;
   int ending_index = 0;
   //int last_index = 0;
   char str_instruction_bool[MAX_LENGTH];

   strcpy(prog->caller_instruction, prog->instruction);

   //"(" <BOOLFUNC> ")"
   if(prog->input[prog->len_index] == '('){
      (prog->len_index)++;

      clear_string(prog);

      while(prog->input[prog->len_index] != ')'){
         
         ////<BOOLFUNC> ::= "LESS" <LIST> <LIST> | "GREATER" <LIST> <LIST> | "EQUAL" <LIST> <LIST>
         prog->instruction[len] = prog->input[prog->len_index];

         if(prog->instruction[len] == ' '){
            
            prog->instruction[len] = '\0';
            printf("\n\nInstruction passed to bool func - %s\n", prog->instruction);

            if(!bool_func(prog)){
               printf("Error in bool function\n");
            }
         }

         len++;
         (prog->len_index)++;
      }
   }

   store_len = prog->len_index;

   //keep track of the initial store condition of 0
   store_cond = prog->lisp_structure;

   //len_index should be pointing to the next subsequent position
   prog->len_index = prog->len_index + 2;
   //store_len = prog->len_index;

   //I will need to traverse through the string to find the index of the second double nested loop (( 
   //find the second if condition
   int counter = 0;

   while(prog->input[store_len] != '\0'){
      if(prog->input[store_len] == '('){
         if(prog->input[store_len + 1] == '('){
            else_cond_index = store_len;
            counter++;

            if(counter == 2){
               break;
            }
         }
      }
      store_len++;
   }

   int counter2 = 0;
   lengh = store_len;


   while(prog->input[lengh] != '\0'){
      if(prog->input[lengh] == ')'){
         if(prog->input[lengh + 1] == ')'){
            ending_index = lengh;
            counter2++;

            if(counter2 == 2){
               break;
            }
         }
      }
      lengh++;
   }

   
   //keep track of the instruction
   strcpy(str_instruction_bool, prog->instruction);

   printf("Store cond - %i\n\n", store_cond->atomtype);

      //we want to give it the true instruction
      if(store_cond->atomtype == 1){
         
         while(prog->input[prog->len_index] == ' '){
            prog->len_index++;
         }

         //takes care of the first bracket 
         if(prog->input[prog->len_index] == '('){
            prog->len_index++;
         }

         if(instructions(prog)){
            strcpy(prog->instruction, str_instruction_bool);
            prog->len_index = ending_index;
            store_len = lengh;
            return true;
         }else{
            return false;
         }

      //we want to give it the false instruction
      }else{

         prog->len_index = else_cond_index;

         //takes care of the first bracket 
         if(prog->input[prog->len_index] == '('){
            prog->len_index++;
         }

         if(instructions(prog)){
            //prog->len_index = prog->len_index-2;
            //prog->len_index--;
            prog->len_index = ending_index;
            store_len = lengh;
            return true;

         }else{
            return false;
         }
      }

   return false;

#else

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
               prog->len_index--;
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

   #endif
   
}



//<LOOP> ::= "WHILE""(" <BOOLFUNC> ")" "(" <INSTRCTS>
bool loop_func(prog* prog){ 

#ifdef INTERPRET

   int enter_loop = 0;

   int len = 0;
   //int flag = 0;
   int store_cond, store_base;
   //int store_len = 0;
   int last_index = 0;
   int flag4 = 0;
   char str_instruction_bool[MAX_LENGTH];
   int start_of_loop;
   //store the prog->len index
   int prog_after_loop = 0;
   char store[MAX_LENGTH];
   char loop_ins[MAX_LENGTH];
   int lo = 0;
   char var;

   strcpy(prog->caller_instruction, prog->instruction);
   strcpy(loop_ins,prog->caller_instruction);

   
   //keep track of the initial base of 5
   //store_base = prog->lisp_structure;

   //"(" <BOOLFUNC> ")"
   if(prog->input[prog->len_index] == '('){
      (prog->len_index)++;

      clear_string(prog);

      while(prog->input[prog->len_index] != ')'){
         
         ////<BOOLFUNC> ::= "LESS" <LIST> <LIST> | "GREATER" <LIST> <LIST> | "EQUAL" <LIST> <LIST>
         prog->instruction[len] = prog->input[prog->len_index];

         if(prog->instruction[len] == ' '){

            prog->instruction[len] = '\0';
            printf("\n\nInstruction passed to bool func - %s\n", prog->instruction);
            enter_loop = prog->len_index; 
            strcpy(str_instruction_bool, prog->instruction);

            if(bool_func(prog)){
               
               prog->bool_cond = true; 

            }
         }

         len++;
         (prog->len_index)++;
      }
   }

   prog_after_loop = prog->len_index;


   for(int i = enter_loop; i < prog_after_loop; i++){
      if(prog->input[i] == 39){

         i++;

         while(prog->input[i] != 39){
            store[lo] = prog->input[i];
            lo++;
            i++;
         }

         store[lo] = '\0';
      }

      if(prog->input[i] >= 'A' && prog->input[i] <= 'Z'){
         if(prog->input[i - 1] == ' '){
            var = prog->input[i]; 
         }
      }
   }

   printf("store: %s\n\n", store);
   printf("var: %c\n\n", var);

   //keep track of the literal - cannot be changed
   store_cond = atoi(store);

   for(int i = 0; i < prog->numberoflisps; i++){
      if(prog->lisp_array[i]->name == var){
         store_base = prog->lisp_array[i]->structure->atomtype;
      }
   }

   //store the variable - can be changed
   //store_base = prog->lisp_structure->atomtype;

   //int prog_val = prog->numberoflisps - 1;
   
   //printf("Store_cond - %i\n\n", store_cond->atomtype);
   printf("Store_base - %i\n\n", store_base);
   printf("Store_cond - %i\n\n", store_cond);

   //len_index should be pointing to 
   start_of_loop = prog->len_index + 2;

   printf("Start of the loop - %i\n", start_of_loop);
   //last index - beggining index of instructions
   //store_len = prog->len_index;
   
   //keep track of the instruction
   //strcpy(str_instruction_bool, prog->instruction);

   //we will need to carry out the rest of the instructions
   //until the condition inside is false

   while(prog->bool_cond != false){


      //<BOOLFUNC> ::= "LESS" <LIST> <LIST> | "GREATER" <LIST> <LIST> | "EQUAL" <LIST> <LIST>      
      if(strcmp(str_instruction_bool, "LESS") == 0){

         //compare two lists
         //while(store_base >= store_cond){

         while(prog->bool_cond != false){

            strcpy(prog->caller_instruction, loop_ins);
            strcpy(prog->instruction, str_instruction_bool);
            prog->len_index = enter_loop;

            bool_func(prog);
            
            if(prog->lisp_structure->atomtype == 1){

               prog->len_index = start_of_loop;
               instructions(prog);

               last_index = prog->len_index;
               //store_base = prog->lisp_structure->atomtype;


            }else if(prog->lisp_structure->atomtype == 0){

               prog->bool_cond = false;
               prog->len_index = last_index;
               flag4 = 1;
               prog->numberoflisps = prog->most_no_lisps;
            }


            //prog->lisp_structure->atomtype = prog->temp_lisp_structure->atomtype;
            //last_index = prog->len_index;
            //prog->len_index = store_len;

/*
            if(store_base <= store_cond){
               prog->bool_cond = false;
               prog->len_index = last_index;
               flag4 = 1; 
            }
*/
         }

         //}
      
      }else if(strcmp(str_instruction_bool, "GREATER") == 0){

         while(prog->bool_cond != false){

            strcpy(prog->caller_instruction, loop_ins);
            strcpy(prog->instruction, str_instruction_bool);
            prog->len_index = enter_loop;

            bool_func(prog);
            
            if(prog->lisp_structure->atomtype == 1){

               prog->len_index = start_of_loop;
               instructions(prog);

               last_index = prog->len_index;
               //store_base = prog->lisp_structure->atomtype;


            }else if(prog->lisp_structure->atomtype == 0){

               prog->bool_cond = false;
               prog->len_index = last_index;
               flag4 = 1;
               prog->numberoflisps = prog->most_no_lisps;
               
            }
         }

      }else if(strcmp(str_instruction_bool, "EQUAL") == 0){

         while(prog->bool_cond != false){

            strcpy(prog->caller_instruction, loop_ins);
            strcpy(prog->instruction, str_instruction_bool);
            prog->len_index = enter_loop;

            bool_func(prog);
            
            if(prog->lisp_structure->atomtype == 1){

               prog->len_index = start_of_loop;
               instructions(prog);

               last_index = prog->len_index;
               //store_base = prog->lisp_structure->atomtype;


            }else if(prog->lisp_structure->atomtype == 0){

               prog->bool_cond = false;
               prog->len_index = last_index;
               flag4 = 1;
               prog->numberoflisps = prog->most_no_lisps;

            }
         }
      }

   }

   (prog->len_index)--;


   if(flag4 == 1){
      return true;
   }


return false;






#else

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

      
      clear_string(prog);


      while(prog->input[prog->len_index] != ')'){

         
         ////<BOOLFUNC> ::= "LESS" <LIST> <LIST> | "GREATER" <LIST> <LIST> | "EQUAL" <LIST> <LIST>
         prog->instruction[len] = prog->input[prog->len_index];

         if(prog->instruction[len] == ' '){

            prog->instruction[len] = '\0';
            printf("\n\nInstruction passed to bool func - %s\n", prog->instruction);

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

      while(prog->input[prog->len_index] == ' '){
         prog->len_index++;
      }

      printf("\n\n\nINDEX - %i\n\n", prog->len_index);

      printf("--------LOOP - Flag1 return true----------- \n\n");

      flag2 = 1;
   }


   //"(" <INSTRCTS>
   if(flag2 == 1){

      while(prog->input[prog->len_index] == ' '){
         prog->len_index++;
      }

      printf("\n\n\nINDEX - %i\n\n", prog->len_index);
      printf("--------LOOP - Flag2 return true----------- \n\n");

      //spaces in between
      while(prog->input[prog->len_index] != ')'){
         
         if(prog->input[prog->len_index] == '('){

            prog->len_index++;

            while(prog->input[prog->len_index] == ' '){
                prog->len_index++;
            }

//            (prog->len_index)++;
            printf("\n\n\n AFTER FIRST INDEX - %i\n\n", prog->len_index);

            if(instructions(prog)){

               printf("WHILE condition - first instructions return %i\n", prog->len_index);
               flag3 = 1;
               prog->len_index--;
            }

            #ifdef INTERPERT
            #endif

         }

         (prog->len_index)++;
      }

   }

   if(flag3 == 1){

      (prog->len_index)--;

      return true;
   }

return false;


#endif


}
