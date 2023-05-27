#include<stdlib.h>
#include<string.h>
#include"curses.h"

int main(int argc, char* argv[]) {
    struct node* gitStatusHead;
    struct node* gitLogHead;
    struct node* gitCommitHistoryHead;
    struct gitWindow git_log; 
    struct gitWindow git_status;
    struct gitWindow git_commit_history;
    git_log.wind = NULL;
    git_status.wind = NULL;
    git_commit_history.wind = NULL;

    int row, col, ch, y, x, start_x, start_y;
    int *p;
    int greetingHeight = 1;
    int quitMsgHeight = 3;;
    char* greeting = "terminal git";
    char* quitMsg = "press 'q' to quit";

    initialize(&row, &col, greeting, quitMsg, greetingHeight, quitMsgHeight);

    refreshGitWindows(quitMsgHeight, col, row, git_log, git_status, git_commit_history);

    // Take user input. Probably implement switch case here?
    do {
        ch = getch();
        cbreak();
        refresh();
    } while (ch != 113 && ch != 81);

    endwin();
    freeList(gitCommitHistoryHead);
    freeList(gitStatusHead);
    // freeList(gitLogHead);
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

void freeList(struct node* head) {
    struct node* tmp = newNodeEmpty();
    tmp = head;

    while (tmp != NULL) {
        tmp = tmp->next;
    }

    free(tmp->line);
    freeList(head);
}

void temp_basicGitWindow(struct gitWindow git, char* status, char* statusCmd) {
    git.head = newNode(status);
    append(git_status.head, EMPTY_STR);
    int res = gitCmd(-1, git_status.head, statusCmd);
    if (res == 0) {
        git.height = getLength(git.head) + 3;
    }
    int y = git.height;
    int x = git.width;
    int start_y = git.start_height;
    int start_x = git.start_width; 
    git.wind = createWindow(y, x, start_y, start_x);
    wPrintList(git.wind, git.head);
    wrefresh(git.wind);
    refresh();
}

void basicGitWindow(struct gitWindow git) {
    int y = git.height;
    int x = git.width;
    int start_y = git.start_height;
    int start_x = git.start_width; 
    git.wind = createWindow(y, x, start_y, start_x);
    wrefresh(git.wind);
    refresh();
}

void refreshGitWindows(int quitMsgHeight, int col, int row, struct gitWindow git_log, struct gitWindow git_status, struct gitWindow git_commit_history) {
    int y, x, start_x, start_y;
    // GIT LOG PRETTY
    git_log.width = col / 7;
    git_log.height = row;
    git_log.start_width = 0;
    git_log.start_height = 0;

    basicGitWindow(git_log);

    // GIT STATUS
    git_status.width = col - (git_log.width + 2);
    git_status.height = row / 4;
    git_status.start_width = git_log.width + 1;
    git_status.start_height = quitMsgHeight + 1;

    char* status ="GIT STATUS";
    char* statusCmd = "git status";
    // gitStatusHead = newNode(status);
    git_status.head = newNode(status);
    // append(gitStatusHead, EMPTY_STR);
    append(git_status.head, EMPTY_STR);
    // int res = gitCmd(-1, gitStatusHead, statusCmd);
    int res = gitCmd(-1, git_status.head, statusCmd);
    if (res == 0) {
        // git_status.height = getLength(gitStatusHead) + 3;
        git_status.height = getLength(git_status.head) + 3;
    }
    y = git_status.height;
    x = git_status.width;
    start_y = git_status.start_height;
    start_x = git_status.start_width; 
    // statusWindow = createWindow(y, x, start_y, start_x);
    git_status.wind = createWindow(y, x, start_y, start_x);
    // wPrintList(statusWindow, gitStatusHead);
    // wPrintList(statusWindow, git_status.head);
    wPrintList(git_status.wind, git_status.head);
    // wrefresh(statusWindow);
    wrefresh(git_status.wind);
    refresh();

    // COMMIT HISTORY
    git_commit_history.width = col - (git_log.width + 2);
    git_commit_history.height = row - (quitMsgHeight + git_status.height + 1);
    git_commit_history.start_width = git_log.width + 1;
    git_commit_history.start_height = git_status.height + 4;

    char* commitHistory = "GIT LOG";
    char* commitHistoryCmd = "git log";
    git_commit_history.head = newNode(commitHistory);
    append(git_commit_history.head, EMPTY_STR);
    res = gitCmd(git_commit_history.height - 3, git_commit_history.head, commitHistoryCmd);
    y = git_commit_history.height;
    x = git_commit_history.width;
    start_y = git_commit_history.start_height;
    start_x = git_commit_history.start_width; 
    // commitHistoryWindow = createWindow(y, x, start_y, start_x);
    git_commit_history.wind = createWindow(y, x, start_y, start_x);
    // nwPrintList(git_commit_history.height, commitHistoryWindow, gitCommitHistoryHead);
    nwPrintList(git_commit_history.height, git_commit_history.wind, git_commit_history.head);
    // wrefresh(commitHistoryWindow);
    wrefresh(git_commit_history.wind);
    refresh();
}

void initialize(int* row, int* col, char* greeting, char* quitMsg, int greetingHeight, int quitMsgHeight) {
    int y, x;
    // Initiallize screen, draw main windows
    initscr();
    keypad(stdscr, TRUE);
    refresh();
    getmaxyx(stdscr, y, x);
    attron(A_BOLD);
    mvprintw(greetingHeight, centeredPosition(greeting, x), "%s\n", greeting);
    attroff(A_BOLD);
    refresh();
    mvprintw(quitMsgHeight, centeredPosition(quitMsg, x), "%s\n", quitMsg);
    refresh();
    *row = y;
    *col = x;
}
