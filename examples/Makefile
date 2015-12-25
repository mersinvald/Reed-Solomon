PWD = $(shell pwd)

INCLUDES = -I$(PWD)/../include/

LIBS  = -L../ -lRS
CXXFLAGS = -std=c++11 -Wall -Werror
CC = g++

SRC=$(wildcard *.cpp)
SRC_PATH=$(wildcard $(PWD)/*.cpp)

OBJ=$(SRC:%.cpp=%.o)
OBJ_PATH=$(SRC_PATH:%.cpp=%.o)

all: obj
	cd $(PWD)/../build/obj/ && $(CC) $(CXXFLAGS) $(OBJ) -o ../example $(LIBS)

obj: $(SRC)
	cd $(PWD)/../build/obj/ && $(CC) $(INCLUDES) $(CXXFLAGS) $(SRC_PATH) -c

clean:
	rm $(PWD)/../build/example
