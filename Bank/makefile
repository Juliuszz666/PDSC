all: main

main: main.o prompts.o search.o mod_actions.o acc_creation.o file_actions.o
	gcc -fsanitize=undefined -g $^ -o $@ -lm

.c.o: 
	gcc -fsanitize=undefined -g -Wall -pedantic -c  $<

main.o: main.c main.h

prompts.o: prompts.c prompts.h typedefs.h

search.o: search.c search.h typedefs.h prompts.h

mod_actions.o: mod_actions.c mod_actions.h typedefs.h file_actions.h prompts.h

acc_creation.o: acc_creation.c acc_creation.h typedefs.h file_actions.h

file_actions.o: file_actions.c file_actions.h typedefs.h prompts.h

clean:
	-rm main *.o