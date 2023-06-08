#include<ncurses.h>

#define EMPTY_STR ""

struct node {
    char* line;
    struct node* next;
};

struct gitWindow {
    int height;
    int width;
    int start_height;
    int start_width;
    struct node* head;
    WINDOW *wind;
};

WINDOW *createWindow(int y, int x, int start_y, int start_x);
void destroyWindow(WINDOW *win);
int centeredPosition(char *msg, int col);
int gitCmd(int n, struct node* head, char* cmd); 
int gitStatus(WINDOW* win);
struct node* newNode(char* str);
struct node* newNodeEmpty();
void append(struct node* head, char* str);
void wPrintList(WINDOW *win, struct node* head);
void nwPrintList(int n, WINDOW *win, struct node* head); 
int getLength(struct node* head);
void trimRight(char* str); 
void freeList(struct node* head);
void basicGitWindow(struct gitWindow git); 
void refreshGitWindows(int quitMsgHeight, int col, int row, struct gitWindow git_log, struct gitWindow git_status, struct gitWindow git_commit_history); 
void initialize(int* row, int* col, char* greeting, char* quitMsg, int greetingHeight, int quitMsgHeight); 
void gitDiff(int* row, int* col, char* greeting, char* quitMsg, int greetingHeight, int quitMsgHeight);
