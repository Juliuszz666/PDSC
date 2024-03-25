all: main

main: main.o
	gcc -fsanitize=undefined -g -Wall -pedantic -lm -o main main.o
main.o: main.c
	gcc -fsanitize=undefined -g -Wall -pedantic -lm -c main.c  -o main.o
clean:
	-rm main.o main