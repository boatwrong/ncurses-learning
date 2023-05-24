#include<ncurses.h>

struct node {
    char* line;
    struct node* next;
};

WINDOW *createWindow(int y, int x, int start_y, int start_x);
void destroyWindow(WINDOW *win);
int centeredPosition(char *msg, int col);
int populateList(struct node* head);
int gitStatus(WINDOW* win);
struct node* newNode(char* str);
struct node* newNodeEmpty();
void append(struct node* head, char* str);
void wPrintList(WINDOW *win, struct node* head);
int getLength(struct node* head);
