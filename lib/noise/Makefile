CC      = gcc
CFLAGS  =
LDFLAGS =

.PHONY: all clean

all: clean libnoise.a

noise1234.o:
	$(CC) -o $@ noise1234.c -c $(CCFLAGS) $(LDFLAGS)

libnoise.a: noise1234.o
	ar rcs libnoise.a $^

clean:
	rm -rf libnoise.a noise1234.o