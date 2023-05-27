#include<stdlib.h>
#include<string.h>
#include"curses.h"

int main(int argc, char* argv[]) {
    struct node* gitStatusHead;
    struct node* gitLogHead;
    struct node* gitCommitHistoryHead;
    WINDOW *logWindow;
    WINDOW *statusWindow;
    WINDOW *commitHistoryWindow;
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

    int git_commit_history_width;
    int git_commit_history_height;
    int git_commit_history_start_width;
    int git_commit_history_start_height;

    // Initiallize screen, draw main windows
    initscr();
    keypad(stdscr, TRUE);
    refresh();

    getmaxyx(stdscr, row, col);
    center_row = row / 2;
    center_col = col / 2;

    git_log_width = col / 7;
    git_log_height = row;
    git_log_start_width = 0;
    git_log_start_height = 0;

    attron(A_BOLD);
    mvprintw(greetingHeight, centeredPosition(greeting, col), "%s\n", greeting);
    attroff(A_BOLD);
    refresh();
    mvprintw(quitMsgHeight, centeredPosition(quitMsg, col), "%s\n", quitMsg);
    refresh();

    // GIT LOG PRETTY

    y = git_log_height;
    x = git_log_width;
    start_y = git_log_start_height;
    start_x = git_log_start_width; 
    logWindow = createWindow(y, x, start_y, start_x);
    wrefresh(logWindow);
    refresh();

    git_status_width = col - (git_log_width + 2);
    git_status_height = row / 4;
    git_status_start_width = git_log_width + 1;
    git_status_start_height = quitMsgHeight + 1;

    // GIT STATUS

    char* status ="GIT STATUS";
    char* statusCmd = "git status";
    gitStatusHead = newNode(status);
    append(gitStatusHead, EMPTY_STR);
    int res = gitCmd(-1, gitStatusHead, statusCmd);
    if (res == 0) {
        git_status_height = getLength(gitStatusHead) + 3;
    }
    y = git_status_height;
    x = git_status_width;
    start_y = git_status_start_height;
    start_x = git_status_start_width; 
    statusWindow = createWindow(y, x, start_y, start_x);
    wPrintList(statusWindow, gitStatusHead);
    wrefresh(statusWindow);
    refresh();

    // COMMIT HISTORY
    git_commit_history_width = col - (git_log_width + 2);
    git_commit_history_height = row - (quitMsgHeight + git_status_height + 1);
    git_commit_history_start_width = git_log_width + 1;
    git_commit_history_start_height = git_status_height + 4;

    char* commitHistory = "GIT LOG";
    char* commitHistoryCmd = "git log";
    gitCommitHistoryHead = newNode(commitHistory);
    append(gitCommitHistoryHead, EMPTY_STR);
    res = gitCmd(git_commit_history_height - 3, gitCommitHistoryHead, commitHistoryCmd);
    // if (res == 0) {
    //     git_commit_history_height = getLength(gitCommitHistoryHead) + 3;
    // }
    y = git_commit_history_height;
    x = git_commit_history_width;
    start_y = git_commit_history_start_height;
    start_x = git_commit_history_start_width; 
    commitHistoryWindow = createWindow(y, x, start_y, start_x);
    nwPrintList(git_commit_history_height, commitHistoryWindow, gitCommitHistoryHead);
    wrefresh(commitHistoryWindow);
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

int gitCmd(int n, struct node* head, char* cmd) {
    int len;
    FILE *fp;
    fp = popen(cmd, "r");
    char path[1035];
    char* line = (char*)NULL;

    if (fp == NULL) {
        pclose(fp);
        return 1;
    }

    else {
        int idx = 1;
        do {
            if (idx == n) {
                pclose(fp);
                return 3;
            }
            if (1 == idx) {
                idx++;
                continue;
            }
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
        if (strcmp(tmp->line, EMPTY_STR)) {
        mvwprintw(win,idx,3,"///////////////////////////////////");
        }
        mvwprintw(win,idx,3,"%s\n", tmp->line);
        wrefresh(win);
        refresh();
        tmp = tmp->next;
        idx++;
    }
}

void nwPrintList(int n, WINDOW *win, struct node* head) {
    struct node* tmp = newNodeEmpty();
    tmp = head;

    int idx = 1;
    while (tmp != NULL) {
        if (n == idx) {
            return;
        }
        if (strcmp(tmp->line, EMPTY_STR)) {
        mvwprintw(win,idx,3,"///////////////////////////////////");
        }
        mvwprintw(win,idx,3,"%s\n", tmp->line);
        wrefresh(win);
        refresh();
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
