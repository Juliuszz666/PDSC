all: test_std_strtol test_my_strtol tests

.c.o:
	gcc -c -Wall -pedantic -g $<

test_std_strtol: test_strtol.o
	gcc -g test_strtol.o -o test_std_strtol

test_my_strtol: test_strtol.o strtol.o
	gcc -g test_strtol.o strtol.o -o test_my_strtol

.PHONY: tests diff clean

tests: test_std_strtol test_my_strtol
	./test_my_strtol > rep_my.txt
	./test_std_strtol > rep_std.txt
	cmp rep_my.txt rep_std.txt
	
diff: test_std_strtol test_my_strtol
	./test_my_strtol > rep_my.txt
	./test_std_strtol > rep_std.txt
	diff rep_my.txt rep_std.txt	

clean:
	-rm test_strtol.o strtol.o test_std_strtol test_my_strtol rep_my.txt rep_std.txt
