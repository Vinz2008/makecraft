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

all: clean libengine.a

%.o: %.c
	$(CC) -o $@ $^ -c $(CFLAGS) $(LDFLAGS)

libengine.a: $(OBJS)
	$(AR) rcs libengine.a $^

clean:
	rm -rf libengine.a $(OBJS) *.o *.a