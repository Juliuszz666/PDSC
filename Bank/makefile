all: main

main: main.o list.o prints.o
	gcc -fsanitize=undefined -g $^ -o $@ -lm

.c.o: 
	gcc -fsanitize=undefined -g -Wall -pedantic -c  $<

list.o: list.c list.h

main.o: main.c list.h

prints.o: prints.c prints.h

clean:
	-rm main.o list.o main