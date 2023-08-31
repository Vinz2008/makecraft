CC      = gcc
CFLAGS  = -Wall -g
LDFLAGS =
AR = ar

ifeq ($(PLATFORM),PLATFORM_WEB)
CC = emcc
AR = emar
CFLAGS += -I/usr/include
endif

SRCS := $(wildcard *.c)
OBJS = $(patsubst %.c,%.o,$(SRCS))

.PHONY: all clean

all: clean libmap.a

%.o: %.c
	$(CC) -o $@ $^ -c $(CFLAGS) $(LDFLAGS)

libmap.a: $(OBJS)
	$(AR) rcs libmap.a $^

clean:
	rm -rf libmap.a $(OBJS) *.o *.a