# variables
CC = gcc
CC_FLAGS = -Wfatal-errors -m32 -g -w

project: src/*.c src/commands/*.c header/*.h header/commands/*.h
	$(CC) $(CC_FLAGS) src/*.c src/commands/*.c -o project


clean:
	rm project
