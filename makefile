all: a.out

CC=g++
CPPFLAGS=-Wall -Wextra

a.out: test.cpp
	$(CC) $(CPPFLAGS) $< 

clean:
	rm -f a.out sample.bmp
