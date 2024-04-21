all: main

main: main.o prints.o
	gcc -fsanitize=undefined -g $^ -o $@ -lm

.c.o: 
	gcc -fsanitize=undefined -g -Wall -pedantic -c  $<

main.o: main.c prints.h typedefs.h

prints.o: prints.c prints.h typedefs.h

clean:
	-rm main.o prints.o main