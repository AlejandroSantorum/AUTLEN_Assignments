CC = gcc
CFLAGS = -g
EXE = main_de_ejemplo

all : $(EXE)

.PHONY : clean
clean :
	rm -f *.o core $(EXE)

.PHONY : clean_dot
clean_dot :
	rm -f *.dot

$(EXE) : % : %.o afnd.o
	$(CC) $(CFLAGS) -o $@ $@.o afnd.o

afnd.o : afnd.c afnd.h
	$(CC) $(CFLAGS) -c $<
