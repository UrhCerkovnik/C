#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <n> <name> <m>\n", argv[0]);
        return 1;
    }  
    int n = atoi(argv[1]);
    char *name = argv[2];
    int m = atoi(argv[3]);

    if (n < 0 || m < 0) {
        fprintf(stderr, "n in m morata biti nenegativni stevili.\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i * m; j++) {
            printf(".");
        } 
        printf("%s\n", name);
    }
    return 0;
}