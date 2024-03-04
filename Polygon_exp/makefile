all: polygon_exp

polygon_exp: polygon_exp.o primlib.o
	gcc -g $^ -o $@  -lSDL2_gfx `sdl2-config --libs` -lm

.c.o: 
	gcc -g -Wall -pedantic `sdl2-config --cflags` -c  $<

primlib.o: primlib.c primlib.h

polygon_exp.o: polygon_exp.c primlib.h

clean:
	-rm primlib.o polygon_exp.o polygon_exp