CC=clang
# CFLAGS=-Wall
OBJ_FLAGS=-I.
NCURSE=-lncurses

curses: curses.c curses.h
	$(CC) $(CFLAGS) $(NCURSE) -o $@ $<

test: temp.c
	$(CC) $(CFLAGS)  -o $@ $<

clean:
	rm -rf test curses *.o *.dSYM *.DS_STORE
