TARGET=a.out

CC=g++
CPPFLAGS=-std=c++11

-include makefile.opt

SRC=$(shell ls *.cpp)
HED=$(shell ls *.h)
OBJ=$(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CPPFLAGS) -o $(TARGET) $(OBJ) 


%.o: %.cc
	$(CC) $(CPPFLAGS) -c $< 

dep:
	g++ -MM -MG $(SRC) >makefile.depend

.PHONY: clean

clean:
	rm -f $(TARGET) $(OBJ)

-include makefile.depend
