#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define ON_ERROR(S) {fprintf(stderr, "%s",S); \ exit(EXIT_FAILURE);}
#define MAX_LENGTH 1000

void program(char* input);
void instructions(char* input, int* len_index);
void instruction(char* input, int* len_index);
void ins_set(char* input, int* len_index);
bool ins_list(char* input, int* len_index);
bool ins_var(char* input, int* len_index);
bool ins_literal(char* input, int* len_index);
bool ins_print(char* array, int* len_index);
bool ins_string(char* input, int* len_index);
