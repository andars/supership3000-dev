CC = gcc
CXX = g++
CFLAGS = -Wall
CXXFLAGS = -Wall
LDFLAGS = -L/usr/local/lib -lSDL2 -lSDL2main

TARGET = game.exe

.PHONY: all clean

all: $(TARGET)

$(TARGET): build/Game.o
	$(CXX) -o $@ $^ $(LDFLAGS)

build/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< 

clean:
	rm -f build/*.o *.exe
