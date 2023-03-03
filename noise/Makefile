CC      = g++
CFLAGS  = -Wall -g
LDFLAGS =
AR = ar

ifeq ($(PLATFORM),PLATFORM_WEB)
CC = em++
AR = emar
CFLAGS += -I/usr/local/include -std=c++14
endif

SRCS := $(wildcard *.cpp)
OBJS = $(patsubst %.cpp,%.o,$(SRCS))

.PHONY: all clean

all: clean libnoise.a

%.o: %.cpp
	$(CC) -o $@ $^ -c $(CFLAGS) $(LDFLAGS)

libnoise.a: $(OBJS)
	$(AR) rcs libnoise.a $^

clean:
	rm -rf libnoise.a $(OBJS) *.o *.a