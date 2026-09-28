#include <stdio.h>

void prestej(char niz[], int pojavitve[]) {
    for (int i = 0; niz[i] != '\0'; i++) {
        if (niz[i] >= '0' && niz[i] <= '9') {
            pojavitve[niz[i] - '0']++;
        }
    }
}

int main(void) {
    char niz[1000];
    int pojavitve[10] = {0};

    while (fgets(niz, sizeof(niz), stdin) != NULL) {
        if (niz[0] == '\n' || niz[0] == '\0') {
            break;
        }
        prestej(niz, pojavitve);
    }

    for (int i = 0; i < 10; i++) {
        if (i > 0) {
            printf(",");
        }
        printf("%d=%d", i, pojavitve[i]);
    }
    printf("\n");

    return 0;
}