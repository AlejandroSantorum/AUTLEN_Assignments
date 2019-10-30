CC = gcc
CFLAGS = -g -ansi -Wall -pedantic -std=c99
TESTS = stack_test mtx_test translate_from_afnd
LIBS = ../srclib

all: $(TESTS)

stack_test: stack_test.c $(LIBS)/stack.o
	$(CC) $(CFLAGS) $^ -o $@ -lm

mtx_test: mtx_test.c $(LIBS)/utils.o $(LIBS)/afnd.o
	$(CC) $(CFLAGS) $^ -o $@

translate_from_afnd: translate_from_afnd.c $(LIBS)/utils.o $(LIBS)/afnd.o
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf *.o $(TESTS)
