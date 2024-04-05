memgrind: mymalloc.o memgrind.o
	gcc mymalloc.o memgrind.o -fsanitize=address,undefined -o memgrind

mymalloc.o: mymalloc.c mymalloc.h
	gcc -c -std=c99 -Wall -Wvla mymalloc.c

malloctest.o: malloctest.c mymalloc.h
	gcc -c -std=c99 -Wall -Wvla malloctest.c

malloctest: mymalloc.o malloctest.o
	gcc mymalloc.o malloctest.o -fsanitize=address,undefined -o malloctest

memgrind.o: memgrind.c mymalloc.h
	gcc -c -std=c99 -Wall -Wvla memgrind.c

memtest.o: memtest.c mymalloc.h
	gcc -c -std=c99 -Wall -Wvla memtest.c

memtest: memtest.o mymalloc.o
	gcc mymalloc.o memtest.o -fsanitize=address,undefined -o memtest