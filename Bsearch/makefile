all: main.o bsearch.o
	gcc -fsanitize=undefined -g -Wall -pedantic main.o bsearch.o -o bsearch.out
.c.o:
	gcc -fsanitize=undefined -g -Wall -pedantic -c $<

main.o: main.c bsearch.h
bsearch.o: bsearch.c bsearch.h