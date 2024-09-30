#include <stdio.h>
#include <stdlib.h>

/*note: when pushing something from a particular branch, use this  
command git branch -branch name-    (no hypens)*/

#define MEMLENGTH 4096

static int init = 0; //Not Initialized


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
    int size; //I'm changing this from size_t to a int to fix alignment. If an int is too small to store size, then use unsigned int.    
    struct header* next;

} header;  //int + int + header pointer = size of 16 bytes

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




