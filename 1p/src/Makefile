CC = gcc
CFLAGS = -g -ansi -Wall -pedantic -std=c99
EXE = main_de_ejemplo
LIBS = ../srclib

all : $(EXE)

.PHONY : clean
clean :
	rm -f *.o core $(EXE)

.PHONY : clean_dot
clean_dot :
	rm -f *.dot

main_de_ejemplo: $(LIBS)/afnd.o
	$(CC) $(CFLAGS) $@.c $? -o $@
