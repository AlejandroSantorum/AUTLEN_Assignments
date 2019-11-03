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

main_de_ejemplo: $(LIBS)/afnd.o $(LIBS)/transforma.o $(LIBS)/utils.o $(LIBS)/current_state.o $(LIBS)/stack.o $(LIBS)/linked_list.o
	$(CC) $(CFLAGS) $@.c $? -o $@
