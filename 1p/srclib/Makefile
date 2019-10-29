CC = gcc
CFLAGS = -g -ansi -Wall -pedantic
EXE = test_cstate

all : $(EXE)

.PHONY : clean
clean :
	rm -f *.o core $(EXE)

$(EXE) : % : %.o current_state.o
	$(CC) $(CFLAGS) -o $@ $@.o current_state.o

current_state.o : current_state.c current_state.h
	$(CC) $(CFLAGS) -c $<
