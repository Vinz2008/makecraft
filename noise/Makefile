CC      = gcc
CFLAGS  = -Wall
LDFLAGS =

SRCS := $(wildcard *.c)
OBJS = $(patsubst %.c,%.o,$(SRCS))

.PHONY: all clean

all: clean libnoise.a

%.o: %.c
	$(CC) -o $@ $^ -c $(CFLAGS) $(LDFLAGS)

libnoise.a: $(OBJS)
	ar rcs libnoise.a $^

clean:
	rm -rf libnoise.a $(OBJS) *.o *.a