all: qs_with_names

qs_with_names: qs_with_names.o
	gcc -g $^ -o sorting -lm
.c.o:
	gcc -g -Wall -pedantic -c $<

qs_with_names.o: qs_with_names.c

clean:
	-rm qs_with_names.o sorting