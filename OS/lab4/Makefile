CC = gcc
CFLAGS = -Wall -Werror -lm -Wl,-rpath=.
LDFLAGS = -L. -l1 -l2

LIB_SOURCES = lib1.c lib2.c
LIB_OBJECTS = $(LIB_SOURCES:.c=.o)
LIB_SHARED = lib1.so lib2.so

PROG_SOURCES = prog1.c prog2.c
PROG_OBJECTS = $(PROG_SOURCES:.c=.o)
PROG_BINARIES = prog1 prog2

.PHONY: all libs programs clean

all: libs programs

libs: $(LIB_SHARED)

programs: $(PROG_BINARIES)

$(LIB_SHARED): $(LIB_OBJECTS)
	$(CC) -shared -o lib1.so lib1.o
	$(CC) -shared -o lib2.so lib2.o

$(PROG_BINARIES): $(LIB_SHARED) $(PROG_OBJECTS)
	$(CC) $(CFLAGS) -o prog1 prog1.o $(LDFLAGS)
	$(CC) $(CFLAGS) -o prog2 prog2.o $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -fPIC -c -o $@ $<

clean:
	rm -f $(LIB_OBJECTS) $(LIB_SHARED) $(PROG_OBJECTS) $(PROG_BINARIES)
