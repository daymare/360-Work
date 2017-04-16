

# variables
CC = gcc
CC_FLAGS = -Wfatal-errors -g -w


# main target
project1: Main.o Util.o 
	$(CC) $(CC_FLAGS) Main.o Util.o -o showblock


Main.o: Main.c
	$(CC) $(CC_FLAGS) -c Main.c


# all the other things
Util.o: Util.h Util.c
	$(CC) $(CC_FLAGS) -c Util.c


clean:
	rm -f *.o lab6
