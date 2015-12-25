PWD = $(shell pwd)
OUT_STATIC = $(PWD)/../build/libRS.a

INCLUDES = -I$(PWD)/../include/

LDFLAGS  =
CXXFLAGS = -std=c++11 -Wall -Werror
CC = g++

SRC=$(wildcard *.cpp)
SRC_PATH=$(wildcard $(PWD)/*.cpp)

OBJ=$(SRC:%.cpp=%.o)
OBJ_PATH=$(SRC_PATH:%.cpp=%.o)

all: obj static

static:
	cd $(PWD)/../build/obj/ && ar rcs ../libRS.a $(OBJ)

obj: $(SRC)
	cd $(PWD)/../build/obj/ && $(CC) $(INCLUDES) $(CXXFLAGS) $(SRC_PATH) -c

clean:
	rm $(OUT_STATIC) $(PWD)/../build/obj/*
