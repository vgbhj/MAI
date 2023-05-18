CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -g
OBJ = main.o stack.o tree.o
PROG = program

build: main.o stack.o tree.o
	$(CC) $(CFLAGS) -o $(PROG) $(OBJ)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

stack.o: stack.c
	$(CC) $(CFLAGS) -c stack.c

tree.o: tree.c
	$(CC) $(CFLAGS) -c tree.c

clean:
	rm $(PROG) $(OBJ)