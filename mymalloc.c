//Questions
//File and line??
//Out of bounds pointer
//size_t or int??

#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"
/*note: when pushing something from a particular branch, use this  
command git branch -branch name-    (no hypens)*/

#define MEMLENGTH 4096

static int init = 0; //Not Initialized at the start


// Our Memory Array

/* Marcus's questions: Will a struct automatically align itself with
   8 bytes or not? If not, is there a function that would allow to 
   to fix its size? Also, will the struct pad itself out if the size
   of all of its members add up to a multiple of 8?  

   
   https://www.geeksforgeeks.org/structure-member-alignment-padding-and-data-packing/
   https://stackoverflow.com/questions/1841863/size-of-struct-in-c
   https://stackoverflow.com/questions/364483/determining-the-alignment-of-c-c-structures-in-relation-to-its-members
   https://www.delftstack.com/howto/c/struct-alignment-in-c/

   Based on what I've read, most datatypes' alignments are based on
   their size. Chars are usually 1 byte so they have an alignment of
   1, shorts are usually 2 bytes so they have an alignment of 2, 
   ints are usually 4 bytes so they have alignments of 4, etc. 

   A struct's size (and whether there is any padding or not) is 
   dependent of the members inside of it and the alignment of the 
   biggest member. If a variable immediately follows another variable
   with a different alignment than it and is thus unaligned, extra
   bytes will be "stuffed" between them to ensure that it is aligned.
   For example, if there is an int followed by a short followed by a 
   char pointer, one might expect that it should have a size of 14
   bytes (assuming pointers are usually 8 bytes of data). While the 
   int and short are aligned, the char is not since it lands on byte
   6. To make sure it is aligned with an address thats a multiple of
   8, 2 bytes are stuffed between the short and the char, resulting
   in a total size of 16 bytes. This means that the order in which
   members are stored in a struct will affect the size it takes up.
   Also, a struct has to be aligned according to the size of its 
   largest member to make sure that an array of that struct will have
   all its members be properly aligned. If an int value was stored
   after the char pointer, one might expect the struct to have a 
   total size of 20 bytes since the int is properly aligned. However,
   since the alignment of the largest value is 8, the alignment of the
   struct is 8 as well. 4 bytes are stuffed after the int which 
   results in a total size of 24 bytes for the struct.
*/

static union{
    char bytes[MEMLENGTH];
    double not_used;

} heap;

// Blocks to allocate memory. Make sure it is aligned with 8.
typedef struct header{
    int free;
    size_t size; //size_t or int??
    struct header* next;
} header;  //int + size_t + header* = size of 24 bytes

static header *head = NULL; // First block is null

void leak_detection(){
    header *ptr = head;
    int objects=0;
    size_t mem = 0;
    while (ptr!=NULL){
        if (!(ptr->free)){
            objects++;
            mem+=ptr->size;
        }
        ptr=ptr->next;
    }
    if (objects>0){
        fprintf(stderr, "mymalloc: %zu bytes leaked in %d objects.\n", mem, objects);

    }
    
    return;
}

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
    head -> size=MEMLENGTH;  //head->size=MEMLENGTH-sizeof(header);
    head->next=NULL;
    init = 1; //mark as initilized once init_heap is called
    atexit(leak_detection);
    return;
}


void *mymalloc(size_t size, char* file, int line){
    if (!init){
        init_heap();
    }

    //rounding up requested payload size to a multiple of 8. 0 bytes also counts as a multiple of 8, so we only add 7 so a 0 byte value can be preserved 
    size_t requestedSize = ((size + 7)/8) * 8;

    //If a payload of 0 bytes is requested, return null
    if(requestedSize == 0) return NULL;

    //look for an unallocated chunk that is large enough for the payload. If none can be found, return NULL. 
    header *ptr = head;
    size_t offset = 0;  //Finds what point in the heap that a chunk begins with
    while(ptr != NULL){
        //if the chunk is already allocated or if payload space is too small, move on to the next chunk. 
        if((ptr -> free == 0) || (ptr -> size < requestedSize)){
            offset += ptr -> size;
            ptr = ptr -> next;

        }else if(ptr -> size == requestedSize){    //if the payload size matches perfectly, allocate the chunk return a pointer to its payload.
            ptr -> free = 0;
            ptr -> size = requestedSize + sizeof(header);

            return heap.bytes + offset + sizeof(header); //Finds a pointer to the payload. The pointer is first cast as a char pointer to make sure that we only travel to the end of the metadata.

        } else{//If chunk is bigger than necessary, split it into an allocated and unallocated chunk.
            size_t originalChunkSize = ptr -> size;
            size_t targetChunkSize = requestedSize + sizeof(header);
            size_t unallocatedChunkSize = originalChunkSize - targetChunkSize;

            //finds where to create new unallocated chunk
            header *unallocatedChunk = (header *)(heap.bytes + offset + requestedSize + sizeof(header));

            //creates the allocated and unallocated chunks. Array syntax used to create the unallocated chunk is the same as dereferencing
            header n = {.free = 1, .size = unallocatedChunkSize, .next = (ptr -> next)};
            unallocatedChunk[0] = n;
            ptr -> free = 0;
            ptr -> size = targetChunkSize;
            ptr -> next = unallocatedChunk;

            return heap.bytes + offset + sizeof(header);
        }
    }

    //If there are no chunks big enough, return NULL
    fprintf(stderr, "Unable to allocate %zu bytes (%s, %d)", size, file, line);
    return NULL;
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

    // create a ptr to the metadata 
    header *headptr = (header *)((char *)ptr - sizeof(header));
    if (headptr->free){
        fprintf(stderr, "free: Double free (%s, %d)\n", file, line);
        exit(2);
    }

    //If the pointer is not in the bounds of the array, report and error
    //Clarify memory addresses with prof??
    if ((char *)head > (char *)headptr || (char *)headptr > (char *)head+MEMLENGTH){
        fprintf(stderr, "free: Invalid pointer (%s, %d)\n", file, line);
        exit(2);
    }
    
    headptr->free=1;

    
    

    //Coalesce blocks after every free() call
    coalesce();
    

    return;
}

// int main(){
//     char *test = malloc(sizeof(char));
//     free(test);
//     free(test);
//     printf("successful");
// }