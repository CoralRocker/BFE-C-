CFLAGS=-Wall -Wpedantic -ggdb -std=c++20
CC=g++

compiler: compiler.cpp AST/ast.cpp
	$(CC) $(CFLAGS) compiler.cpp AST/ast.cpp -o bfc
