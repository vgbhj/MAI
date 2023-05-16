CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -g
OBJ = main.o udt.o
PROG = program

build: main.o udt.o
	$(CC) $(CFLAGS) -o $(PROG) $(OBJ)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

udt.o: udt.c
	$(CC) $(CFLAGS) -c udt.c

clean:
	rm $(PROG) $(OBJ)