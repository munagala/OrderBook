CPP=g++
CPPFLAGS=-g -Wall -std=c++11
LDFLAGS=-pthread
BIN=orderBook

SRC=$(wildcard *.cpp)
OBJ=$(SRC:%.cpp=%.o)

all: $(OBJ)
	$(CPP) -o $(BIN) $(LDFLAGS) $^

%.o: %.c
	$(CPP) $@ -c $<

clean:
	rm -f *.o
	rm -f $(BIN)
