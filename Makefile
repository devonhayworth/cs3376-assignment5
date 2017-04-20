PROG = TwoPipesTwoChildren TwoPipesThreeChildren dynpipe
CC = g++
CFLAGS = -g
OBJS = TwoPipesTwoChildren.o TwoPipesThreeChildren.o DynPipe.o

all : $(OBJS)
	$(CC) -o TwoPipesTwoChildren TwoPipesTwoChildren.o
	$(CC) -o TwoPipesThreeChildren TwoPipesThreeChildren.o
	$(CC) -o dynpipe DynPipe.o
TwoPipesTwoChildren.o : 
	$(CC) $(CFLAGS) -c TwoPipesTwoChildren.cpp
TwoPipesThreeChildren :
	$(CC) $(CFLAGS) -c TwoPipesThreeChildren.cpp
DynPipe.o :
	$(CC) $(CFLAGS) -c DynPipe.cpp
clean :
	rm -f core $(PROG) $(OBJS)
