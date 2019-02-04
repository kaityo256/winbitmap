CC=g++
CPPFLAGS=-std=c++11 -Wall -Wextra

-include makefile.opt

SRC=$(shell ls *.cpp)
HED=$(shell ls *.h)
OBJ=$(SRC:.cpp=.o)

all: a.out

a.out: test3d.cpp draw3d.hpp ../winbitmap.hpp
	$(CC) $(CPPFLAGS) $<

.PHONY: clean

clean:
	rm -f a.out test3d.bmp

-include makefile.depend
