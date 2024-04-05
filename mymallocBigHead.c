#include <stdlib.h>
#include <stdio.h>
#include "mymalloc.h"

#define MEMLENGTH 4096
#define DEBUG 0
#define METASIZE sizeof(header)

static double memory[MEMLENGTH];

typedef struct header{
    int chunkSize;
    int allocated;
    struct header *next;
} header;

void *mymalloc(size_t size, char *file, int line) {
    
    if (size < 1) {
        printf("malloc error: invalid size requested (%s:%d)\n", file, line);
        return NULL;
    }
    
    char *heapStart = (char *) memory;
    void *returnPtr = NULL;
    int firstRun = 1;
    //Check if this is the first run (array would be all 0s)
    for (int i = 0; i < MEMLENGTH; i++){
        if (heapStart[i] != 0) {
            firstRun = 0;
            break;
        }
    }
    
    //Initialize the heap if it is the first run
    if (firstRun) {
        if (DEBUG) printf("creating first run header\n");

        header initHeader;
        initHeader.chunkSize = MEMLENGTH - METASIZE;
        initHeader.allocated = 0;
        initHeader.next = NULL;

        header *memHead = (header *) memory;
        *memHead = initHeader;
    }

    int newChunkSize = (size + 7) & ~7;
    header *currentHead = (header *) memory;
    if (DEBUG) printf("Trying to return a chunk of size: %d\n", newChunkSize);
    int counter = 1;

    do {
        //If an unallocated pointer is found with enough space
        if(!currentHead->allocated && currentHead->chunkSize >= newChunkSize) {
            //Create a new header for the end of the chunk
            //IF there is space for the new header
            if (currentHead->chunkSize >= (newChunkSize + METASIZE)) {
                header *newHeader = currentHead + ((newChunkSize + METASIZE) / METASIZE);
                newHeader->chunkSize = currentHead->chunkSize - (newChunkSize + METASIZE);
                newHeader->allocated = 0;
                newHeader->next = currentHead->next;
                currentHead->next = newHeader;
                if (DEBUG) printf("currentHead address: %p\nNew Header address: %p\n%d bytes between.\n", currentHead, newHeader, (int)((char *)newHeader - (char *)currentHead));
            }
            
            //Change the current header to the metadata of the requested chunk
            currentHead->chunkSize = newChunkSize;
            currentHead->allocated = 1;
            returnPtr = currentHead + 1;
            return returnPtr;
        } else {
            if (DEBUG) {
                //printf("Chunk %d not viable.\n", counter);
                counter++;
            }
            currentHead = currentHead->next;
        }
    } while (currentHead != NULL);

    //If we make it here there should be no space left in "memory"
    printf("malloc error: not enough space (%s:%d)\n", file, line);
    return NULL;
}

void myfree(void *ptr, char *file, int line) {

    header *currentHead = (header *) memory;
    //Find if the pointer given is equal to any of the pointers malloced
    while (ptr != (currentHead + 1) && currentHead != NULL) {
        //if (DEBUG) printf("No match between %p and %p.\n", ptr, currentHead);

        currentHead = currentHead->next;
    }

    //Unallocated the pointer if found, otherwise report errors
    if (ptr == (currentHead + 1) && currentHead->allocated == 1) {
        currentHead->allocated = 0;
        if (DEBUG) printf("Freed pointer\n");
    } else if (ptr == (currentHead + 1) && currentHead->allocated == 0) {
        printf("free error: double free (%s:%d)\n", file, line);
    } else {
        printf("free error: invalid pointer (%s:%d)\n", file, line);
    }

    //Coalesence checking
    currentHead = (header *) memory;
    header *prevHead = (header *) memory;

    while (currentHead != NULL) {
        //If 2 chunks are unallocated next to each other, the later chunk gets "deleted"
        if (currentHead != prevHead && !currentHead->allocated && !prevHead->allocated) {
            if (DEBUG) printf("Combining chunks at %p and %p\n", currentHead, prevHead);
            prevHead->chunkSize += currentHead->chunkSize + METASIZE;
            prevHead->next = currentHead->next;
            currentHead = prevHead;
        }
        prevHead = currentHead;
        currentHead = currentHead->next;
    }
}

void printHeap() {
    header *currentHead = (header *)memory;
    int counter = 1;
    while (currentHead != NULL) {
        printf("Chunk %d size: %d, allocated: %d, next address: %p\n", counter, currentHead->chunkSize, currentHead->allocated, currentHead->next);
        counter++;
        currentHead = currentHead->next;
    }
}