#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<stdio.h>

typedef struct {
    char** line;
    struct node* next;
} node;

node* head;
node* tail;

void init(char* str) {
    node* tmp;
    tmp->line = &str;
    tmp->next = NULL;
    head = tmp;
    tail = tmp;
}

void append(char* str) {
    node* next; 
    next->line = &str;
    next->next = NULL;

    node* tmp = head;
    while (tmp != NULL) {
        tmp = tmp->next;
    }
}

void gitStatus();
void appendCharStar(); 

int main(int argc, char* argv[]) {
    char* a = "testing";
    char b = 'b';
    init(a);
}

void gitStatus() {
    FILE *fp;
    char* line = (char*)NULL;
    int len;
    char path[1035];
    fp = popen("git status", "r");
    if (fp == NULL) {
        printf("NOT A GIT REPOSITORY\n");
    }
    else {
        int idx = 1;
        do {
            len = strlen(path);
            line = (char*)malloc(sizeof(char) * len);
            strcpy(line, path);
            if (line != (char*)NULL) {
                printf("%d %lu: %s", len,strlen(path), line);
            }
            idx++;
        }
        while (fgets(path, sizeof(path), fp) != NULL); 
    }
    pclose(fp);
}
