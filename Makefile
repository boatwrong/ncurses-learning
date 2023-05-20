CC=gcc
CFLAGS=-Wall
OBJ_FLAGS=-I.
NCURSE=-lncurses

curses: curses.c curses.h
	$(CC) $(CFLAGS) $(NCURSE) -o $@ $<

clean:
	rm -rf curses *.o *.dSYM *.DS_STORE
