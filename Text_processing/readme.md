Simple text processing
Takes input text from stdin and reverses order of words in line and then prints reversed and processed lines.
To avoid memory I made simple shell script to enable automatic testing (running valgrind multiple times with rand malloc, saving outputs to text file, counting valgrind runs without errors)  
**MAKEFILE COMMANDS**  
*make normal* - creates a program without random malloc  
*make random* - creates a program with random NULL injection  
*make clean* - also removes valgrind report  
*make test* - runs a shell script and removes previous valgrind report