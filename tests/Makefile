CXX=g++
CXXFLAGS=-std=c++11 -Wall -pedantic
LIBS = -L../build -lRS
INC = -I ../include
BIN=RStest

SRC=$(wildcard *.cpp)
OBJ=$(SRC:%.cpp=%.o)

all: $(OBJ)
	$(CXX) $(OBJ) -o $(BIN) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INC) $< -c

clean:
	rm -f *.o
	rm $(BIN)
