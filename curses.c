#include<stdlib.h>
#include<string.h>
#include"curses.h"

int main(int argc, char* argv[]) {
    struct node* head;
    WINDOW *logWindow;
    WINDOW *statusWindow;
    WINDOW *lastCommitWindow;
    int row, col;
    int center_row;
    int center_col;
    int greetingHeight = 1;
    int quitMsgHeight = 3;;
    char greeting[] = "terminal git";
    char quitMsg[] = "press 'q' to quit";

    int *p;
    int ch;
    int y;
    int x;
    int start_x;
    int start_y;

    int git_log_width;
    int git_log_height;
    int git_log_start_width;
    int git_log_start_height;

    int git_status_width;
    int git_status_height;
    int git_status_start_width;
    int git_status_start_height;

    int git_last_commit_width;
    int git_last_commit_height;
    int git_last_commit_start_width;
    int git_last_commit_start_height;

    // Initiallize screen, draw main windows
    initscr();
    keypad(stdscr, TRUE);

    getmaxyx(stdscr, row, col);
    center_row = row / 2;
    center_col = col / 2;

    git_log_width = col / 7;
    git_log_height = row;
    git_log_start_width = 0;
    git_log_start_height = 0;

    git_status_width = col - (git_log_width + 2);
    git_status_height = row / 4;
    git_status_start_width = git_log_width + 1;
    git_status_start_height = quitMsgHeight + 1;

    git_last_commit_width = col - (git_log_width + 2);
    git_last_commit_height = row - (quitMsgHeight + git_status_height + 1);
    git_last_commit_start_width = git_log_width + 1;
    git_last_commit_start_height = quitMsgHeight + git_status_height + 1;

    attron(A_BOLD);
    mvprintw(greetingHeight, centeredPosition(greeting, col), "%s\n", greeting);
    attroff(A_BOLD);
    refresh();
    mvprintw(quitMsgHeight, centeredPosition(quitMsg, col), "%s\n", quitMsg);
    refresh();
    
    y = git_log_height;
    x = git_log_width;
    start_y = git_log_start_height;
    start_x = git_log_start_width; 
    logWindow = createWindow(y, x, start_y, start_x);
    refresh();

    char* git ="GIT STATUS";
    head = newNode(git);
    int res = populateList(head);
    if (res == 0) {
        y = getLength(head);
    }
    else {
        y = git_status_height;
    }
    x = git_status_width;
    start_y = git_status_start_height;
    start_x = git_status_start_width; 
    statusWindow = createWindow(y, x, start_y, start_x);
    wPrintList(statusWindow, head);
    refresh();

    y = git_last_commit_height;
    x = git_last_commit_width;
    start_y = git_last_commit_start_height;
    start_x = git_last_commit_start_width; 
    lastCommitWindow = createWindow(y, x, start_y, start_x);
    refresh();

    // Take user input. Probably implement switch case here?
    do {
        ch = getch();
        cbreak();
        refresh();
    } while (ch != 113 && ch != 81);

    endwin();
    return 0;
}

WINDOW *createWindow(int y, int x, int start_y, int start_x) {
    WINDOW *myWin = newwin(y, x, start_y, start_x);
    box(myWin, 0, 0);
    wrefresh(myWin);
    return myWin;
}

void destroyWindow(WINDOW *win) {
    wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wrefresh(win);
    delwin(win);
}

// return the centered start position of a centered message.
int centeredPosition(char *msg, int col) {
    return (col - strlen(msg)) / 2;
}

int populateList(struct node* head) {
    int len;
    FILE *fp;
    fp = popen("git status", "r");
    char path[1035];
    char* line = (char*)NULL;

    if (fp == NULL) {
        pclose(fp);
        return 1;
    }

    else {
        int idx = 1;
        do {
                len = strlen(path);
            if (1 > len) {
                strcpy(line,"");
            }

            else {
                line = (char*)malloc(sizeof(char) * len);
                strcpy(line, path);
            }

            if (line != (char*)NULL) {
                append(head, line);
                // free(line);
            }
            idx++;
        } while (fgets(path, sizeof(path), fp) != NULL); 
        pclose(fp);

        return 0;
    }

    return 2;
}

struct node* newNodeEmpty() {
    struct node* tmp = (struct node*) malloc(sizeof(struct node*));
    tmp->next = NULL;
    return tmp;
}

struct node* newNode(char* str) {
    struct node* tmp = newNodeEmpty();
    tmp->line = str;
    return tmp;
}

void append(struct node* head, char* str) {
    struct node* tmp = newNodeEmpty();
    tmp->next = head;

    while (tmp->next != NULL) {
        tmp = tmp->next;
    }

    struct node* nextNode = newNode(str);
    tmp->next = nextNode;
}

void wPrintList(WINDOW *win, struct node* head) {
    struct node* tmp = newNodeEmpty();
    tmp = head;

    int idx = 1;
    while (tmp != NULL) {
        mvwprintw(win,idx,2,"%s\n", tmp->line);
        wrefresh(win);
        tmp = tmp->next;
        idx++;
    }
}

int getLength(struct node* head) {
    int len = 0;
    struct node* tmp = newNodeEmpty();
    tmp = head;

    while (tmp != NULL) {
        len++;
        tmp = tmp->next;
    }
    return len;
}
