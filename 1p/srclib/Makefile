CC = gcc
CFLAGS = -g -ansi -Wall -pedantic -std=c99
OBJS = afnd.o current_state.o stack.o transforma.o utils.o

all : $(OBJS)

afnd.o: afnd.c afnd.h
	$(CC) -c $<

current_state.o: current_state.c current_state.h
	$(CC) $(CFLAGS) -c $<

stack.o: stack.c stack.h
	$(CC) $(CFLAGS) -c $<

transforma.o: transforma.c transforma.h
	$(CC) $(CFLAGS) -c $<

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c $<


.PHONY : clean
clean :
	rm -f *.o
