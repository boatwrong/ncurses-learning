#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<stdio.h>

struct node {
    char** line;
    struct node* next;
};

struct node* head;

void init(char* str) {
    printf("1\n");
    struct node* tmp = (struct node*) malloc(sizeof(struct node*));
    printf("2\n");
    tmp->line = &str;
    printf("3\n");
    tmp->next = NULL;
    printf("4\n");
    head = tmp;
    printf("5\n");
}

void append(char* str) {
    struct node* tmp = head;

    while (tmp != NULL) {
        tmp = tmp->next;
    }

    struct node* new = (struct node*) malloc(sizeof(struct node*));
    new->line = &str;
    new->next = NULL;
}

void printList() {
    printf("6\n");
    struct node* tmp = (struct node*) malloc(sizeof(struct node*));
    printf("7\n");
    tmp->next = head;

    printf("8\n");
    while (tmp != NULL) {
        printf("9\n");
        printf("10\n");
        if (tmp->next != NULL) {
            printf("11\n");
            if (tmp->line != NULL) {
                printf("%s\n", *tmp->line);
            }
            printf("12\n");
            tmp = tmp->next;
            printf("13\n");
        }
        else {
            printf("14\n");
            break;
        }
    }

}

void gitStatus();
void appendCharStar(); 

int main(int argc, char* argv[]) {
    struct node* head = (struct node*) malloc(sizeof(struct node*));
    printf("testing\n");
    char* a = "testing";
    char b = 'b';
    printf("entering init()\n");
    init(a);
    printList();
    char* c = "hello";
    printList();
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
