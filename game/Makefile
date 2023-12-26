CXX      = g++
CXXFLAGS  = -Wall -g
LDFLAGS =

ifeq ($(PLATFORM),PLATFORM_WEB)
CC = em++
CXXFLAGS += -I/usr/include
endif

SRCS := $(wildcard *.cpp)
OBJS = $(patsubst %.cpp,%.o,$(SRCS))

.PHONY: all clean

all: clean $(OBJS)

%.o: %.cpp
	$(CXX) -o $@ $^ -c $(CXXFLAGS) $(LDFLAGS)


clean:
	rm -rf $(OBJS) *.o