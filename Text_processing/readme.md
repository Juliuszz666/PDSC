# Simple Text Processing

This program takes input text from stdin and reverses the order of words in each line. It then prints the reversed and processed lines.

To avoid memory issues, I have created a simple shell script that enables automatic testing. The script runs `valgrind` multiple times with random `malloc` calls, saves the outputs to a text file, and counts the number of `valgrind` runs without errors.

## Makefile Commands

- `make normal`: Creates a program without random `malloc` calls.
- `make random`: Creates a program with random `NULL` injection.
- `make clean`: Removes the `valgrind` report.
- `make test`: Runs the shell script and removes the previous `valgrind` report.