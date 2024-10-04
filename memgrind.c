#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"

void taskOne();
void taskTwo();
void test();

int main(){
    test();
    return EXIT_SUCCESS;
}


void test(){
    //taskOne();
    //taskTwo();
}

//Allocates 1 byte of memory 120 times. The memory is always freed before the next allocation is made. Make sure to update the line number passed through malloc and free whenever the line number changes
void taskOne(){
    for(int i = 0; i < 120; i++){
        char *ptr = (char *)malloc(1); //allocates 1 byte of memory using file name and line number.

        *ptr = 'a';
        //printf("%d. %c\n", i, *ptr); This is for testing
        free(ptr); //frees the 1 byte of memory

    }
}

//
void taskTwo(){
    char *ptrArray[120] = {0};
    
    for(int i = 0; i < 120; i++){
        ptrArray[i] = (char *)malloc(1);
        *(ptrArray[i]) = 'a';
    }

    for(int i = 0; i < 120; i++){
        printf("%d. %c\n", i, *(ptrArray[i])); //This is for testing
        free(ptrArray[i]);
    }
}