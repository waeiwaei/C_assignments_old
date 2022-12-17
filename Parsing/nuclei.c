#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LINES 100
#define MAX_LENGTH 1000

/*Function definitions*/
bool program(char* input[]);
void instructions(char* input[]);
void instruction(char* input[]);
bool functions(char* input[]);
bool variable(char value);
int length(char* input[], int line);

//we will also need to read a file, and store the input in a string, and then find each word based on "\n"
int main(int argc, char* argv[]){

   char input[MAX_LINES][MAX_LENGTH];

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
         int line;

      /*Populates 2D string with input from file*/
      while(!feof(fp)){
         if(fgets(input[line], MAX_LENGTH, fp) != NULL){
            line++;
         }
      }

      fclose(fp); 

      /*program function for parser and interpreter*/
      program(input);

      }
   }
   

   return 0;
}

bool variable(char value){

   if(value <= 'A' && value <= 'Z'){
      return true
   }

   return false;
}

int length(char* input[],int line_index){

   return strlen(input[line]);
}


bool string(char* input[], int line_index, int len_index){

   char string_array[MAX_LENGTH];
   int in = 0;

   while(input[line_index][len_index] != '"'){
      string_array[in] = input[line_index][len_index];
      in++;
      len_index++;
   }

   return true;
   
}

bool literal(char* input[], int line_index, int len_index){

   char string_array[MAX_LENGTH];
   int in = 0;

   //ascii code for - ' symbol
   while(input[line_index][len_index] !=  39){
      string_array[in] = input[line_index][len_index];
      in++;
      len_index++;
   }

   string_array[in] == '\0';

   return true;

}



bool program(char* input[]){

   //we check if the first character is a '(', then go into instructions
   if(input[0][0] == '('){

      int line_index = 1;
      int len_index = 0;
      //go into the instruction
      instructions(input, line_index, len_index);
      
   }else{
      printf("Input does not conform to formal grammar");
      return false;
   }

   return false;
}

/*
void instructions(char* input[], int line_index, int len_index){

   static int inc = 0;

   //we want to traverse through the input
   while(input[][] != ')'){
      if(input[][] == '('){

      }else{


      }

   }

   if(input[][] == ')'){
      printf("Last value is ')'");
      return;
   }

}

*/
