#include<ncurses.h>
#include<stdlib.h>

int main(int argc, char* argv[]) {
    initscr();
    printw("hello");
    refresh();
    printw("\npress 'q' to quit");
    char c;
    do { 
        c = getch();
        continue;
    } while (c != 'q' || c != 'Q');

    getch();
    endwin();

    return 0;
}
