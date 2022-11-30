/* Boolean(Bit) Array/Vector
   Little-endian: LSB  */


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#define BIGSTR 1000

typedef enum logicalop {or, and, xor} logicalop;


struct boolarr{

   char * binary;
   int length;

};

typedef struct boolarr boolarr;

/* All elements initialised to "0" */
boolarr* boolarr_init(void){
    
    boolarr* temp = (boolarr*)calloc(1, sizeof(boolarr));
    
    temp->binary = (char*)calloc(1, sizeof(char));
    *(temp->binary) = '0';
    temp->length = 0;

    return temp;
}

/* Create boolarr based on string e.g. "1100001" */
boolarr* boolarr_initstr(const char* str){
    
    boolarr* temp = (boolarr*)calloc(1, sizeof(boolarr));
    int length_of_string = strlen(str);

    temp->binary = (char*)calloc(length_of_string + 1, sizeof(char));
    strcpy((temp->binary), str); 

    temp->length = length_of_string;

    return temp;
}



/* Return a deep copy */
boolarr* boolarr_clone(const boolarr* ba){

    boolarr* temp = (boolarr*)calloc(1, sizeof(boolarr));
    temp->binary = (char*)calloc(ba->length, sizeof(char));
    strcpy(temp->binary, ba->binary);
    temp->length = ba->length;

    return temp;
}



/* Get number of bits in array */
unsigned int boolarr_size(const boolarr* ba){

    int counter = 0;

    for(int i = 0; i < ba->length - 1; i++){
        counter++;
    }

    return counter;

}

/* Return number of bits that are set true */
unsigned int boolarr_count1s(const boolarr* ba){

    int counter = 0;
    
    for(int i =0; i < ba->length - 1; i++){
        
        if(ba->binary[i] == true){
            counter++;
        }
    }

    return counter;
}


/* Set nth bit on/off */
bool boolarr_set(boolarr* ba, const unsigned int n, const bool b){

    if(ba == NULL){       
       return false;
    }

    ba->binary[n] = b;

    return true;
    
}

/* Get nth bit */
bool boolarr_get(const boolarr* ba, const unsigned int n, bool* b){

    if(ba == NULL){
        return false;
    }

    *b = ba->binary[n];

    return true;

}

/* Return if two arrays are the same (bitwise) */
bool boolarr_issame(const boolarr* b1, const boolarr* b2){

    
    if(b1 == NULL && b2 == NULL){
        return false;
    }
    
    if(strcmp(b1->binary, b2->binary) == 1){
        return false;
    }

    return true;

}

/* Store to string - rightmost bit is LSB */
bool boolarr_tostring(const boolarr* ba, char* str){

    int index = 0;

    if(ba == NULL){
        return false;
    }

    for(int i = ba->length - 1; i > 0; i--){

        str[index] = ba->binary[i];
        index++;

    }

    return true;
}


/* Print out array & meta info */
bool boolarr_print(const boolarr* ba){

    if(ba == NULL){
        return false;
    }

    for(int i = 0; i < ba->length - 1; i++){
    
        printf("%c ",ba->binary[i]);

    }

    return true;
}

/* Flip all bits */
bool boolarr_negate(boolarr* ba){

    if(ba == NULL){
        return false;
    }

    for(int i = 0; i < ba->length - 1; i++){
        if(ba->binary[i] == true){
            ba->binary[i] = false;
        }else{
            ba->binary[i] = true;
        }
    }

    return true;

}

/* Functions dealing with 2 bitwise-arrays */
/* Must be the same length */
boolarr* boolarr_bitwise(const boolarr* ba1, const boolarr* ba2, const logicalop l){

    boolarr* temp = (boolarr*)calloc(1, sizeof(boolarr));
    temp->binary = (char*)calloc(ba1->length, sizeof(char));

    switch(l){
        //or
        case or:

        //we check the values of both bits, index by index
        for(int i = 0; i < ba1->length; i++){
            if(ba1->binary[i] == true || ba2->binary[i] == true){
                temp->binary[i] = true;
            }else{
                temp->binary[i] = false;
            }
        }

        temp->length = ba1->length;
        break;

        //and
        case and:
        
        for(int i = 0; i < ba1->length; i++){
            if(ba1->binary[i] == true && ba2->binary[i] == true){
                temp->binary[i] = true;
            }else{
                temp->binary[i] = false;
            }
        }
        
        break;

        //xor
        case xor:

        for(int i = 0; i < ba1->length; i++){
            if(ba1->binary[i] == true || ba2->binary[i] == false){
                temp->binary[i] = true;
            }else if(ba1->binary[i] == false || ba2->binary[i] == true){
                temp->binary[i] = true;
            }else{
                temp->binary[i] = false;
            }
        }

        break;
    }


    return temp;

}

/* Clears all space */
bool boolarr_free(boolarr* p){

    if(p != NULL){
        p = NULL;
    }
    
    free(p);

    return true;
}


int main(){

   bool b;
   char str[BIGSTR];
   boolarr* ba;
   boolarr* bb;
   boolarr* bc;
   unsigned int i;
   bool b2;

   assert(boolarr_size(NULL)==0);
   assert(!boolarr_get(NULL, 0, &b));
   assert(!boolarr_set(NULL, 0, true));
   assert(!boolarr_set(NULL, 1, true));
   assert(boolarr_issame(NULL, NULL));
   assert(!boolarr_tostring(NULL, str));
   assert(boolarr_count1s(NULL)==0);
   ba = boolarr_init();
   assert(!boolarr_issame(NULL, ba));
   assert(!boolarr_issame(ba, NULL));
   assert(boolarr_count1s(ba)==0);
   assert(!boolarr_tostring(ba, NULL));
   assert(boolarr_size(ba)==0);
   assert(boolarr_set(ba, 1, true));
   assert(boolarr_get(ba, 0, &b));
   assert(!b);
   assert(boolarr_set(ba, 5, true));
   assert(boolarr_set(ba, 6, false));
   assert(boolarr_tostring(ba, str));
   assert(strcmp(str, "0100010")==0);

   assert(boolarr_set(ba, 0, true));
   assert(boolarr_set(ba, 127, true));
   assert(boolarr_set(ba, 128, true));
   assert(boolarr_set(ba, 191, true));
   assert(boolarr_set(ba, 194, true));

   assert(boolarr_print(ba));

   assert(boolarr_get(ba,0,&b));
   assert(b);
   assert(boolarr_get(ba,2,&b));
   assert(!b);
   assert(boolarr_get(ba,126,&b));
   assert(!b);
   assert(boolarr_get(ba,127,&b));
   assert(b);
   assert(boolarr_get(ba,128,&b));
   assert(b);
   assert(boolarr_get(ba,191,&b));
   assert(b);
   assert(boolarr_get(ba,194,&b));
   assert(b);

   bc = boolarr_clone(ba);
   assert(boolarr_get(bc,0,&b));
   assert(b);
   assert(boolarr_get(bc,2,&b));
   assert(!b);
   assert(boolarr_get(bc,126,&b));
   assert(!b);
   assert(boolarr_get(bc,127,&b));
   assert(b);
   assert(boolarr_get(bc,128,&b));
   assert(b);
   assert(boolarr_get(bc,191,&b));
   assert(b);

   assert(boolarr_count1s(ba) == boolarr_count1s(bc));

   assert(boolarr_issame(ba, ba));
   assert(boolarr_issame(bc, bc));
   assert(boolarr_issame(ba, bc));
   assert(boolarr_issame(bc, ba));

   assert(boolarr_set(bc, 0, false));
   assert(boolarr_get(bc, 0, &b));
   assert(b==false);
   assert(boolarr_set(bc, 1, false));
   assert(boolarr_get(bc, 1, &b));
   assert(b==false);

   bb = boolarr_clone(ba);
   boolarr_negate(bb);
   for(i=0; i<boolarr_size(ba); i++){
      assert(boolarr_get(ba, i, &b ));
      assert(boolarr_get(bb, i, &b2));
      assert(b != b2);
   }
   boolarr_free(ba);
   boolarr_free(bb);
   boolarr_free(bc);

   /* Bitwise */
   ba = boolarr_initstr("100100100");
   assert(ba);
   bb = boolarr_initstr("110110110");
   assert(bb);

   bc = boolarr_bitwise(ba, bb, and);
   assert(bc);
   assert(boolarr_tostring(bc, str));
   assert(strcmp(str, "100100100")==0);
   assert(boolarr_issame(ba, bc));
   boolarr_free(bc);

   bc = boolarr_bitwise(ba, bb, or);
   assert(bc);
   assert(boolarr_tostring(bc, str));
   assert(strcmp(str, "110110110")==0);
   assert(boolarr_issame(bb, bc));
   boolarr_free(bc);

   bc = boolarr_bitwise(ba, bb, xor);
   assert(bc);
   assert(boolarr_tostring(bc, str));
   assert(strcmp(str, "010010010")==0);

   boolarr_free(ba);
   boolarr_free(bb);
   boolarr_free(bc);

   return 0;
}
