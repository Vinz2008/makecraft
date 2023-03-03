CC      = gcc
CFLAGS  = -Wall
LDFLAGS =
AR = ar

ifeq ($(PLATFORM),PLATFORM_WEB)
CC = emcc
AR = emar
endif

.PHONY: all clean

all: clean misc.a

misc.o:
	$(CC) -o $@ startswith.c -c $(CCFLAGS) $(LDFLAGS)

misc.a: misc.o
	$(AR) rcs misc.a $^

clean:
	rm -rf misc.a misc.o