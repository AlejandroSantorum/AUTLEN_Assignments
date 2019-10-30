CC = gcc
CFLAGS = -g -ansi -Wall -pedantic -std=c99
TESTS = stack_test mtx_test
LIBS = ../srclib

all: $(TESTS)

stack_test: stack_test.c $(LIBS)/stack.o
	$(CC) $(CFLAGS) $^ -o $@ -lm

mtx_test: mtx_test.c $(LIBS)/utils.o
	$(CC) $(CFLAGS) $^ -o $@ -lm

clean:
	rm -rf *.o $(TESTS)
