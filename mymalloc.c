#include <stdio.h>
#include <stdlib.h>


#define MEMLENGTH 4096

static int init = 0; //Not Initialized


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
    return;
}

void myfree(void *ptr, char* file, int line){
    return;
}

int main(){
    printf("conflict");
}




