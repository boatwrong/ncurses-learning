#include<ncurses.h>
#include<stdlib.h>

int main(int argc, char* argv[]) {
    initscr();
    printw("hello");
    refresh();
    getch();
    endwin();

    return 0;
}
