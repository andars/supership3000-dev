CC = gcc
CXX = g++
CFLAGS = -Wall
CXXFLAGS = -Wall
LDFLAGS = -L/usr/local/lib -lSDL2 -lSDL2main

TARGET = game.exe

.PHONY: all clean

all: $(TARGET)

$(TARGET): Game.o
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< 

clean:
	rm -f *.o game
