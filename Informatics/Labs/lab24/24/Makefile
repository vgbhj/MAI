all: main

main: bin/main.o bin/stack.o bin/tree.o
	gcc bin/main.o bin/stack.o bin/tree.o

bin/main.o: main.c libs/stack.h libs/tree.h libs/item.h
	gcc main.c -c -o bin/main.o

bin/stack.o: libs/stack.c libs/stack.h libs/item.h
	gcc libs/stack.c -c -o bin/stack.o

bin/tree.o: libs/tree.c libs/tree.h libs/item.h
	gcc libs/tree.c -c -o bin/tree.o