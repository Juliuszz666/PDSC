headers = rand_malloc.h

sources_normal = text_processing.c

sources_random = text_rand.c rand_malloc.c

executable = text

.PHONY: normal
normal:
	gcc -fsanitize=undefined -g -Wall -pedantic $(sources_normal) -o $(executable)
.PHONY: random
random:
	gcc -fsanitize=undefined -g -Wall -pedantic $(sources_random) -o $(executable)
.PHONY: clean
clean:
	-rm text_processing.o text valgrind.txt
.PHONY: test
test:
	-rm valgrind.txt
	bash test_text.sh
