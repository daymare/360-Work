

# variables
CC = gcc
CC_FLAGS = -Wfatal-errors -g -w


# main target
project: general.o commands.o
	$(CC) $(CC_FLAGS) general.o commands.o -o project

# link general files together
general.o: main.o parser.o interpreter.o dataStructures.o globals.o inodeUtil.o blockUtil.o mInodeUtil.o getDir.o
	ld -r main.o parser.o interpreter.o dataStructures.o globals.o inodeUtil.o blockUtil.o mInodeUtil.o getDir.o -o general.o

# link commands together
commands.o: mount_root.o cd.o ls.o pwd.o
	ld -r mount_root.o cd.o ls.o pwd.o -o commands.o


# general files
main.o: src/main.c 
	$(CC) $(CC_FLAGS) -c src/main.c

parser.o: src/parser.c header/parser.h
	$(CC) $(CC_FLAGS) -c src/parser.c

interpreter.o: src/interpreter.c header/interpreter.h
	$(CC) $(CC_FLAGS) -c src/interpreter.c

dataStructures.o: src/dataStructures.c header/dataStructures.h
	$(CC) $(CC_FLAGS) -c src/dataStructures.c

globals.o: src/globals.c header/globals.h
	$(CC) $(CC_FLAGS) -c src/globals.c

inodeUtil.o: src/inodeUtil.c header/inodeUtil.h 
	$(CC) $(CC_FLAGS) -c src/inodeUtil.c

blockUtil.o: src/blockUtil.c header/blockUtil.h
	$(CC) $(CC_FLAGS) -c src/blockUtil.c

mInodeUtil.o: src/mInodeUtil.c header/mInodeUtil.h
	$(CC) $(CC_FLAGS) -c src/mInodeUtil.c

getDir.o: src/getDir.c header/getDir.h
	$(CC) $(CC_FLAGS) -c src/getDir.c

# commands
mount_root.o: src/commands/mount_root.c header/commands/mount_root.h
	$(CC) $(CC_FLAGS) -c src/commands/mount_root.c

ls.o: src/commands/ls.c header/commands/ls.h
	$(CC) $(CC_FLAGS) -c src/commands/ls.c

cd.o: src/commands/cd.c header/commands/cd.h
	$(CC) $(CC_FLAGS) -c src/commands/cd.c

pwd.o: src/commands/pwd.c header/commands/pwd.h
	$(CC) $(CC_FLAGS) -c src/commands/pwd.c






clean:
	rm -f *.o lab6
