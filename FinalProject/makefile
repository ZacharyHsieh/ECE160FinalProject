# Definitions for constants
CC=gcc
CFLAGS=-I.

# This will create your final output using .o compiled files
make: main.o board.o startGame.o playingGame.o endingGame.o
	$(CC) $(CFLAGS) -o main.out main.o board.o startGame.o playingGame.o endingGame.o

# This will compile board.o
board.o: board.c board.h pieces.h
	 $(CC) $(CFLAGS) -c board.c

# This will compile startGame.o
startGame.o: startGame.c startGame.h
	 $(CC) $(CFLAGS) -c startGame.c

# This will compile playingGame.o
playingGame.o: playingGame.c playingGame.h
	 $(CC) $(CFLAGS) -c playingGame.c

# This will compile endingGame.o
endingGame.o: endingGame.c endingGame.h
	 $(CC) $(CFLAGS) -c endingGame.c

# This will compile main.c with its dependency
main.o: main.c board.h pieces.h startGame.h playingGame.h endingGame.h
	 $(CC) $(CFLAGS) -c main.c

# This will clean or remove compiled files so you can start fresh
clean:
	rm -f *.o *.out
