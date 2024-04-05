#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include "mymalloc.h"

#define MEMSIZE 4096
#define HEADERSIZE 8

//Create and immediately free 120 1 byte objects
void immediateFree() {
    for (int i = 0; i < 120; i++) {
        char *obj = malloc(1);
        free(obj);
    }
}

//Create 120 1 byte objects, store, then free
void storeFree() {
    char *vals[120];
    for (int i = 0; i < 120; i++) {
        vals[i] = malloc(1);
    }
    for (int i = 0; i < 120; i++) {
        free(vals[i]);
    }
}

//120 pointers, randomly allocate and deallocate until 120 have been allocated
void randAlloc() {
    char *vals[120];
    int allocated = 0;
    int allocIndex = -1;
    srand(time(NULL));
    
    while (allocated < 120) {
        int r = rand() % 2;
        if (r == 0) {
            allocIndex++;
            vals[allocIndex] = malloc(1);
            allocated++;
        } else if (allocIndex > -1){
            free(vals[allocIndex]);
            allocIndex--;
        }
    }

    while (allocIndex >= 0) {
        free(vals[allocIndex]);
        allocIndex--;
    }
}

//Malloc an array of 120 1 byte values then free 40 of them in the middle
//Then allocate 4 8 byte values;
void middleChunk() {
    char *vals[120];
    for (int i = 0; i < 120; i++) {
        vals[i] = malloc(1);
    }

    for (int i = 39; i < 80; i++) {
        free(vals[i]);
    }

    int *numbers[4];
    for (int i = 0; i < 4; i++) {
        numbers[i] = malloc(8);
    }

    for (int i = 0; i < 39; i++) {
        free(vals[i]);
    }

    for(int i = 80; i < 120; i++) {
        free(vals[i]);
    }

    for (int i = 0; i < 4; i++) {
        free(numbers[i]);
    }
}

//Fill the memory with 64 smaller objects, free and refill with 32 bigger objects
void biggerChunks() {
    char *vals[64];
    //Fill the memory with 64 objects
    for (int i = 0; i < 64; i++) {
        vals[i] = malloc(MEMSIZE / 64 - HEADERSIZE);
    }

    //Free them
    for (int i = 0; i < 64; i++) {
        free(vals[i]);
    }

    //Fill it again with 32 objects
    for (int i = 0; i < 32; i++) {
        vals[i] = malloc(MEMSIZE / 32 - HEADERSIZE);
    }

    //Free those
    for (int i = 0; i < 32; i++) {
        free(vals[i]);
    }
}

int main(int argc, char** argv) {
    struct timeval startTime, endTime;
    int startMS, endMS, averageTime;

    gettimeofday(&startTime, NULL);
    for (int i = 0; i < 50; i++) {
        immediateFree();
    }
    gettimeofday(&endTime, NULL);
    startMS = startTime.tv_sec * 1000000 + startTime.tv_usec;
    endMS = endTime.tv_sec * 1000000 + endTime.tv_usec;
    averageTime = (endMS - startMS) / 50;
    printf("Immediate Free Average elapsed time: %d microseconds\n", averageTime); 
    
    gettimeofday(&startTime, NULL);
    for (int i = 0; i < 50; i++) {
        storeFree();
    }
    gettimeofday(&endTime, NULL);
    startMS = startTime.tv_sec * 1000000 + startTime.tv_usec;
    endMS = endTime.tv_sec * 1000000 + endTime.tv_usec;
    averageTime = (endMS - startMS) / 50;
    printf("Store Free Average elapsed time: %d microseconds\n", averageTime);

    gettimeofday(&startTime, NULL);
    for (int i = 0; i < 50; i++) {
        randAlloc();
    }
    gettimeofday(&endTime, NULL);
    startMS = startTime.tv_sec * 1000000 + startTime.tv_usec;
    endMS = endTime.tv_sec * 1000000 + endTime.tv_usec;
    averageTime = (endMS - startMS) / 50;
    printf("Random Allocation Average elapsed time: %d microseconds\n", averageTime);

    gettimeofday(&startTime, NULL);
    for (int i = 0; i < 50; i++) {
        middleChunk();
    }
    gettimeofday(&endTime, NULL);
    startMS = startTime.tv_sec * 1000000 + startTime.tv_usec;
    endMS = endTime.tv_sec * 1000000 + endTime.tv_usec;
    averageTime = (endMS - startMS) / 50;
    printf("Middle Chunk Average elapsed time: %d microseconds\n", averageTime); 

    gettimeofday(&startTime, NULL);
    for (int i = 0; i < 50; i++) {
        biggerChunks();
    }
    gettimeofday(&endTime, NULL);
    startMS = startTime.tv_sec * 1000000 + startTime.tv_usec;
    endMS = endTime.tv_sec * 1000000 + endTime.tv_usec;
    averageTime = (endMS - startMS) / 50;
    printf("Bigger Chunks Average elapsed time: %d microseconds\n", averageTime); 
}