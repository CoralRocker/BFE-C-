CC=g++
CFLAGS=-Wall -Wextra -ggdb -pedantic -pedantic-errors -std=c++17

all: src/Interpreter/brainfuck.cpp src/Interpreter/ScratchPad.cpp
	$(CC) $(CFLAGS) src/Interpreter/brainfuck.cpp src/Interpreter/ScratchPad.cpp -o bfi
