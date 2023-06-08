#include<stdlib.h>
#include<string.h>
#include"curses.h"

int main(int argc, char* argv[]) {
    struct node* gitStatusHead;
    struct node* gitCommitHistoryHead;
    struct gitWindow git_log; 
    struct gitWindow git_diff; 
    struct gitWindow git_status;
    struct gitWindow git_commit_history;
    git_log.wind = NULL;
    git_diff.wind = NULL;
    git_status.wind = NULL;
    git_commit_history.wind = NULL;

    int row, col, ch, y, x, start_x, start_y;
    int *p;
    int greetingHeight = 1;
    int quitMsgHeight = 3;;
    char* greeting = "terminal git";
    char* quitMsg = "press 'q' to quit, 'd' for diff, 'ENTER' to refresh";

    initialize(&row, &col, greeting, quitMsg, greetingHeight, quitMsgHeight);

    refreshGitWindows(quitMsgHeight, col, row, git_log, git_status, git_commit_history);

    // Take user input. Probably implement switch case here?
    do {
        ch = getch();
        cbreak();
        refresh();
        switch (ch) {
            case 113: 
                continue;
                break;
            case 100: gitDiff(&row, &col, greeting, quitMsg, greetingHeight, quitMsgHeight, git_diff);
                      break;
            case 10: refreshGitWindows(quitMsgHeight, col, row, git_log, git_status, git_commit_history);
                     break;
        }
    } while (113 != ch);

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
    char* lineFix = (char*)NULL;
    char* authorFix = "Author";

    if (NULL == fp) {
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

            if ((char*)NULL != line) {

                // if line contains author, do some weird workaround 
                if (strstr(line, authorFix)) {
                    lineFix = leftTrimFake(1, strlen(line), line);
                    append(head, lineFix);
                }
                else {
                    append(head, line);
                }

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
        mvwprintw(win,idx,5," %s", tmp->line);
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
        mvwprintw(win,idx,3,"\t%s", tmp->line);
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
    append(git.head, EMPTY_STR);
    int res = gitCmd(-1, git.head, statusCmd);
    if (0 == res) {
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

    // GIT STATUS
    git_status.width = col - 2;
    git_status.height = row / 4;
    git_status.start_width = 0;
    git_status.start_height = quitMsgHeight + 1;

    char* status ="GIT STATUS";
    char* statusCmd = "git status";
    git_status.head = newNode(status);
    append(git_status.head, EMPTY_STR);
    int res = gitCmd(-1, git_status.head, statusCmd);
    if (0 == res) {
        git_status.height = getLength(git_status.head) + 3;
    }
    y = git_status.height;
    x = git_status.width;
    start_y = git_status.start_height;
    start_x = git_status.start_width; 
    git_status.wind = createWindow(y, x, start_y, start_x);
    wPrintList(git_status.wind, git_status.head);
    wrefresh(git_status.wind);
    refresh();

    // COMMIT HISTORY
    git_commit_history.width = git_status.width;
    git_commit_history.height = row - (quitMsgHeight + git_status.height + 1);
    git_commit_history.start_width = git_status.start_width;
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
    git_commit_history.wind = createWindow(y, x, start_y, start_x);
    nwPrintList(git_commit_history.height, git_commit_history.wind, git_commit_history.head);
    // wPrintList(git_commit_history.wind, git_commit_history.head);
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
    mvprintw(greetingHeight, centeredPosition(greeting, x), "%s", greeting);
    attroff(A_BOLD);
    refresh();
    mvprintw(quitMsgHeight, centeredPosition(quitMsg, x), "%s", quitMsg);
    refresh();
    *row = y;
    *col = x;
}

// TODO: implement a 'less' like scroll function... 
//       - 'j','k' to scroll but 'ENTER' to return home and 'q' quits all
void gitDiff(int* row, int* col, char* greeting, char* quitMsg, int greetingHeight, int quitMsgHeight, struct gitWindow git_diff) {
    erase();
    initialize(row, col, greeting, quitMsg, greetingHeight, quitMsgHeight);
    // GIT DIFF
    git_diff.width = *col - 2;
    git_diff.height = *row / 4;
    git_diff.start_width = 0;
    git_diff.start_height = quitMsgHeight + 1;

    char* status ="GIT DIFF";
    char* statusCmd = "git diff";
    git_diff.head = newNode(status);
    append(git_diff.head, EMPTY_STR);
    int res = gitCmd(-1, git_diff.head, statusCmd);

    if (0 == res) {
        git_diff.height = getLength(git_diff.head) + 3;
    }

    git_diff.wind = createWindow(git_diff.height, git_diff.width, git_diff.start_height, git_diff.start_width);
    wPrintList(git_diff.wind, git_diff.head);
    wrefresh(git_diff.wind);
    refresh();

}

char* leftTrimFake(int n, int len, char* str) {
    char* newStr = malloc(sizeof(char*) * len);
    strncpy(newStr, str + n, len);
    return newStr;
}

char* leftTrim(int n, char* str) {
    char* newStr = NULL;
    strncpy(newStr, str + n, sizeof(str));
    return newStr;
}
