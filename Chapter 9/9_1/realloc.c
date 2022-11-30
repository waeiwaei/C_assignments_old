#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>


struct arr{

   int* members;
   int length;
   int size;

};

typedef struct arr arr;

/* Creates the empty array */
arr* arr_init(void){

    arr* temp;
    temp = (arr*)calloc(1, sizeof(arr));

    temp->members = (int*)calloc(1, sizeof(int));
    temp->length = 2;
    temp->size = 0;

    return temp;

}





/* Similar to l[n] = i, safely resizing if required */
void arr_set(arr *l, int n, int i){

   //if the index is 0, then we add the value (i) inside members array
   if(n == 0){
      l->members[n] = i;
      return;
   }

   //dynamically store elements passed through in the position of a set
   l->size = n;


   //if the size is larger than length, we then allocate new memory
   if(l->size > l->length){

      //calculate the new length of the array
      int new_length = n;

      //creat a temporary list of members, storing the members from the old array
      int * temp_alloc = (int*)realloc(l->members, new_length * sizeof(int));
  
        //check if realloc worked
        if(temp_alloc == NULL){
            exit(-1);
        }

      //store new members list
      l->members = temp_alloc;

      //store the new capacity found
      l->length = new_length;
   }

   l->members[n] = i;
   
   return;
}





/* Similar to = l[n] */
int arr_get(arr *l, int n){

   //want to get elements in the collection at the specific index
   return l->members[n];

}





/* Clears all space used, and sets pointer to NULL */
void arr_free(arr **l){

    *l = NULL;
    free(*l);


}


int main(void)
{

   arr *a;
   printf("Basic Array Tests ... Start\n");


   a = arr_init();
   assert(a != NULL);

   /* Some simple sets & gets */
   arr_set(a, 0, 1);
   assert(arr_get(a, 0) == 1);
   arr_set(a, 1, 2);
   assert(arr_get(a, 1) == 2);
   /* Force a resize/realloc (depending on initial size) */
   arr_set(a, 32, 17);
   assert(arr_get(a, 32) == 17);
   /* Huge (re)alloc */
   arr_set(a, 512000, 3);
   assert(arr_get(a, 512000) == 3);
   /* The 'unset' gaps in the array should return zero */
   assert(arr_get(a, 2) == 0);

   /* Free it all */
   arr_free(&a);
   assert(a==NULL);
   printf("Basic Array Tests ... Stop\n");

   return 0;
}
