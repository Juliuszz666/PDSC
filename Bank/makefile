all: main

exe = main
sources = main.c list.c
headers = list.h

main: 
	gcc -fsanitize=undefined -g -Wall -pedantic -lm $(sources) -o $(exe)
clean:
	-rm main