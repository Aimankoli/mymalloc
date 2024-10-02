#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"

#define MEMLENGTH 4096

static int init = 0; //Not Initialized at the start


// Our Memory Array
static union{
    char bytes[MEMLENGTH];
    double not_used;

} heap;

// Blocks to allocate memory
typedef struct header{
    int free;
    size_t size;
    struct header* next;

} header;

static header *head = NULL; // First block is null

void init_heap(){
    
    //code
    init = 1; //mark as initilized once init_heap is called
    return;
}

void *mymalloc(size_t size, char* file, int line){
    if (!init){
        init_heap();
    }
    header *ptr = head;

    return;
}

void coalesce(){
    header *ptr = head;

    
    while (ptr&&ptr->next){

        //if two adjacent blocks are free add their size and deassign the pointer to the second one
        if (ptr->free && ptr->next->free){
            ptr->size = ptr->size+ptr->next->size;
            ptr->next=ptr->next->next;

        }else{
            //we put the ptr.next statement in the else block because if multiple consecutive
            //chunks are free, we want the pointer to stay on the current free block.
            //we only want the pointer to move when the next pointer is not free, so it cannot coalesce
            ptr=ptr->next;
        }
        
    }
    return;
}

void myfree(void *ptr, char* file, int line){

    //If the pointer passed to free is null, we have to report an error saying we have a free pointer
    if (!ptr){
        fprintf(stderr, "free: NULL pointer (%s:%d)\n", file, line);
        exit(2);
    }
    

    coalesce();
    //Coalesce blocks after every free() call

    return;
}

int main(){
    printf("conflict");
}




