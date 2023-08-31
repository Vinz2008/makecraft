CC      = gcc
CFLAGS  = -Wall
LDFLAGS =
AR = ar

ARCHIVE = libutils.a

ifeq ($(PLATFORM),PLATFORM_WEB)
CC = emcc
AR = emar
CFLAGS += -I/usr/include
endif

SRCS := $(wildcard *.c)
OBJS = $(patsubst %.c,%.o,$(SRCS))

.PHONY: all clean

all: clean $(ARCHIVE)

%.o: %.c
	$(CC) -o $@ $^ -c $(CFLAGS) $(LDFLAGS)

$(ARCHIVE): $(OBJS)
	$(AR) rcs $(ARCHIVE) $^

clean:
	rm -rf $(ARCHIVE) $(OBJS) *.o *.a