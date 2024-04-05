#include <stdlib.h>
#include <stdio.h>
#include "mymalloc.h"

#define MEMLENGTH 4096
#define METASIZE 8

typedef struct header{
    int chunkSize;
    int allocated;
} header;

void printHeap(header *start) {
    header *currentHeader = start;
    int counter = 1;
    while (currentHeader != NULL && (char *)currentHeader < (char *)(start + MEMLENGTH)) {
        if (currentHeader->chunkSize > MEMLENGTH || currentHeader->chunkSize <= 0 || currentHeader->allocated > 1 || currentHeader->allocated < 0) {
            //Check that the "header" we are at has valid values for chunkSize and allocated
            break;
        }
        //if(counter > 20) break;    used for debugging
        printf("Chunk %d: size = %d, allocated = %d\n", counter, currentHeader->chunkSize, currentHeader->allocated);
        currentHeader += ((currentHeader->chunkSize + METASIZE) / METASIZE);
        counter++;
    }
}

int main(int argc, char** argv) {
    int *arr[50];

    for (int i = 0; i < 10; i++) {
        arr[i] = malloc(8);
    }

    //I'm awesome so I know this is where the header is
    char *firstHeader = (char *)arr[0] - METASIZE;

    *arr[4] = 15;
    *arr[8] = 21;
    //Place one number before and after the chunks that are about to be freed
    printf("indices before freeing, index 4: %d, index 8: %d\n", *arr[4], *arr[8]);
    printHeap((header *)firstHeader);

    free(arr[5]);
    free(arr[6]);
    free(arr[7]);
    printf("indices after freeing, index 4: %d, index 8: %d\n", *arr[4], *arr[8]);
    printHeap((header *)firstHeader);

    arr[10] = malloc(44);
    *arr[10] = 24;
    //Place a number inside the chunk that was just freed
    printf("index 10 before mallocs: %d\n", *arr[10]);

    arr[11] = malloc(32);
    arr[12] = malloc(100);

    //Make sure malloc didn't move any pointers
    printf("index 10 after mallocs: %d\n", *arr[10]);
    printHeap((header *)firstHeader);

    for (int i = 0; i < 5; i++) {
        free(arr[i]);
    }
    for (int i = 8; i < 13; i++) {
        free(arr[i]);
    }
    printf("Heap after free\n");
    printHeap((header *) firstHeader);
}