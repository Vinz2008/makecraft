CC      = g++
CFLAGS  = -Wall -g
LDFLAGS =

SRCS := $(wildcard *.cpp)
OBJS = $(patsubst %.cpp,%.o,$(SRCS))

.PHONY: all clean

all: clean libnoise.a

%.o: %.cpp
	$(CC) -o $@ $^ -c $(CFLAGS) $(LDFLAGS)

libnoise.a: $(OBJS)
	ar rcs libnoise.a $^

clean:
	rm -rf libnoise.a $(OBJS) *.o *.a