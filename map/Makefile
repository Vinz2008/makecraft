CC      = gcc
CFLAGS  = -Wall
LDFLAGS =

SRCS := $(wildcard *.c)
OBJS = $(patsubst %.c,%.o,$(SRCS))

.PHONY: all clean

all: clean libmap.a

%.o: %.c
	$(CC) -o $@ $^ -c $(CFLAGS) $(LDFLAGS)

libmap.a: $(OBJS)
	ar rcs libmap.a $^

clean:
	rm -rf libmap.a $(OBJS) *.o *.a