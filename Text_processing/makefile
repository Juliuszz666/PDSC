all: text

text: text_processing.o
	gcc -fsanitize=undefined -g text_processing.o -o text

text_processing.o: text_processing.c
	gcc -fsanitize=undefined -g -c text_processing.c -o text_processing.o

clean:
	-rm text_processing.o text 