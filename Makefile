CC=gcc
CFLAGS=-Wall -Wextra -std=c99

build:
	$(CC) $(CFLAGS) main.c functions.c commands.c Hashtable.c -g -o main
	
clean:
	rm -f main