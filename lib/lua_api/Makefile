CC      = gcc
CFLAGS  =
LDFLAGS = -llua -lm

.PHONY: all clean

all: clean liblua_api.a

lua.o:
	$(CC) -o $@ lua.c -c $(CCFLAGS) $(LDFLAGS)

liblua_api.a: lua.o
	ar rcs liblua_api.a $^

clean:
	rm -rf liblua_api.a lua.o