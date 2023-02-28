CC      = gcc
CFLAGS  = -Wall
LDFLAGS =

.PHONY: all clean

all: clean misc.a

misc.o:
	$(CC) -o $@ startswith.c -c $(CCFLAGS) $(LDFLAGS)

misc.a: misc.o
	ar rcs misc.a $^

clean:
	rm -rf misc.a misc.o