#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "mymalloc.h"




//Allocates 1 byte of memory 120 times. The memory is always freed before the next allocation is made. Make sure to update the line number passed through malloc and free whenever the line number changes
void taskOne(){
    for(int i = 0; i < 120; i++){
        char *ptr = (char *)malloc(sizeof(char)); //allocates 1 byte of memory using file name and line number.

        *ptr = 'a';
        //printf("%d. %c\n", i, *ptr); This is for testing
        free(ptr); //frees the 1 byte of memory

    }
}

void taskTwo(){
    char *ptrArray[120] = {0};
    
    for(int i = 0; i < 120; i++){
        ptrArray[i] = (char *)malloc(1);
        *(ptrArray[i]) = 'a';
    }

    for(int i = 0; i < 120; i++){
        // printf("%d. %c\n", i, *(ptrArray[i])); //This is for testing
        free(ptrArray[i]);
    }
}

void taskThree(){
	char *array[120];
    int use_malloc;
	int position = -1; 
    int num_mallocs = 0;

	while (num_mallocs<120){
		int num = (rand() % 2);
		if (num ==1)
			use_malloc = (num_mallocs < (120)) ? 1 : 0;
		else
			use_malloc = (position >= 0) ? 0 : 1;

		if (use_malloc) {
            // printf("malloc\n");
			position++;
			array[position] = malloc(sizeof(char));
			num_mallocs++;
		} else {
            // printf("free\n");
			free(array[position]);
			position--;
		}
	}
    // printf("next loop\n");
    while (position>-1){
        free(array[position]);
        position--;
    }
}

//We want to make sure that free() coalesces free blocks

//We want to see how the program deals when fragmentation occurs

//We want to see how fast free coalesces blocks when alternate pointers are freed, i.e,
//when fragmentation occurs.

//We will allocate 120 bytes of memory then free alternate blocks and then allocate a block of size 4072

void taskFour(){
    char *ptrArray[120] = {0};
    
    for(int i = 0; i < 120; i++){
        ptrArray[i] = (char *)malloc(1);
        *(ptrArray[i]) = 'a';
    }

    int j=60;
    for (int k=59; k>-1; k--){
        free(ptrArray[k]);
        free(ptrArray[j]);
        j++;
    }
    char *final = malloc(4072);
    free(final);


}

//Up till now, we have only tested with byte-sized memory allocations
void taskFive(){
    // printf("Task 5\n");
    char *largeArray[16];
    char *smallArray[64];
    for (int i=0; i<16; i++){
        largeArray[i] = malloc(4096/16 -24); // size of each block minus sizeof(header), which is 24
        // printf("Big allocated\n");
    }

    int k=0;
    for (int i = 0; i<16; i++){
        free(largeArray[i]);
        // printf("Big freed\n");
        for (int j=0; j<4; j++){
            smallArray[k] = malloc(4096/64 -24);
            // printf("Small allocated\n");
            k++;

        }
    }
    for (int i=0; i<k; i++){

        free(smallArray[i]);
        // printf("small freed\n");
    }

}


void test(){
    taskOne();
    taskTwo();
    taskThree();
    taskFour();
    taskFive();
}

// Function to calculate time difference in microseconds
double time_diff(struct timeval start, struct timeval end) {
    return (double)(end.tv_sec - start.tv_sec) * 1000000 + (double)(end.tv_usec - start.tv_usec);
}
int main(){
    struct timeval start, end;
    double totaltime;
    void (*funtuons[5])(void) = {taskOne, taskTwo, taskThree, taskFour, taskFive};

    for (int j = 0; j < 5; j++) {
        totaltime=0;
        for (int i=0; i<50; i++){
            gettimeofday(&start, NULL);
            funtuons[j]();
		    gettimeofday(&end, NULL);
            totaltime+=time_diff(start, end);

        }
        printf("Task %d mean runtime: %f\n", j+1, (totaltime / 50));
    }

		



    
    return EXIT_SUCCESS;
}

