CC=gcc
CFLAGS=-lncurses

curses: curses.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf curses *.o *.dSYM *.DS_STORE
