#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* substring(char* source, int n); 

int main(int argc, char* argv[]) {
    char* test = "hello";
    printf("%s\n", test);
    printf("%lu\n", strlen(test));
    printf("%s\n", test);
    char* str = substring(test, 2); 
    printf("%s\n", str);
    return 0;
}

char* substring(char* source, int n) {
    char* str = malloc(sizeof(char*) * strlen(source));
    printf("here\n");
    printf("res\n");
    strncpy(str, source + n, strlen(source));
    printf("copy\n");
    return str;
}
