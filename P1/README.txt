My Little Malloc
by Bryan Mulholland (NETID: bcm129)

Compilation Information:
    $ make malloctest
    malloctest went through several iterations, as it sits it tests integrity of the heap and
    coalescense of free chunks as well as includes a function that prints each header in the heap.
    Takes no arguments.

Test Plan
    Throughout testing I tried to think of cases that would break malloc() and edge cases that could affect integrity or
    cause problems. Things such as filling a freed gap with the exact amount of free bytes, in early versions caused a header
    with a 0 size to pop up. Not only causing issues but also wasting space.

    As it sits there should be no way for malloc() to accidentally move pointers or overwrite any data put in them. But as is
    true with real malloc(), there is technically nothing stopping you from writing more data than would fit in the requested
    pointer and breaking things.

Memgrind
    memgrind includes the 3 tests specified in the write up as well as 2 additional tests, middle chunk and bigger chunks.
    - Middle chunk allocates 120 1 byte values, frees a chunk of 40 in the middle of those values, then allocates 4 8 byte
      values which should be allocated within that chunk. Then frees all remaining pointers.
        - This is meant to test the efficiency of the coalescense of the freed chunks
    - Bigger chunks fills the memory with 64 objects, then frees them and refills the memory with 32 objects.
        - This also tests efficiency of free and ensures that after all chunks are combined, the entire memory
          is still usable.
    
    My memgrind reports the time in microseconds because I felt it was more informative than milliseconds.

Design Notes
    - I used a header size of 8 bytes, earlier versions I had used a 16 byte header with a pointer to the next header.
      I previously felt that this was more secure and simpler than using the chunk size to find the next header, but I believe in
      its current state it is just as safe as the bigger header version. Plus the benefit of having half of the header size saved.
    - If malloc encounters a chunk that has an invalid size or allocation status it exits the program. This scenario shouldn't occur
      but if it were to, every future call of malloc would likely no longer work as intended so it should close.