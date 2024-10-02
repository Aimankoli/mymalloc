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
    size_t size; //size_t or int??
    struct header* next;

} header;

static header *head = NULL; // First block is null

void init_heap(){
    //heap.bytes is currently a char array
    //we want head to be a pointer to the first memory address in the heap
    head = (header *)heap.bytes;
    //heap.bytes points to the first index (char *)
    //cast it to a header
    //assign it to head

    head->free=1;
    //the size (bytes taken by metadata) is the length of the array, subtract the space taken
    //by the block of metadata. This will tell us where the actual data will start
    head->size=MEMLENGTH-sizeof(header);
    head->next=NULL;
    init = 1; //mark as initilized once init_heap is called
    return;
    atexit(leak_detection);
}

void leak_detection(){
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
    //If the pointer is not in the bounds of the array, report and error
    //Clarify memory addresses with prof??
    

    //Coalesce blocks after every free() call
    coalesce();
    

    return;
}

int main(){
    printf("conflict");
}




