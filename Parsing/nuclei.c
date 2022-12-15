#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LINES 100
#define MAX_LENGTH 1000
#define VAR 'A - Z'

//we need seperate functions to check each of the grammar inside
//we will also need to read a file, and store the input in a string, and then find each word based on "\n"


int main(int argc, char* argv[]){

   char input[MAX_LINES][MAX_LENGTH];

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

      for(int i = 0; i < line; i++){
         printf("%s", input[i]);
      }

      }
   }
   

   return 0;
}