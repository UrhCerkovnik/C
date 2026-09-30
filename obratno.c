#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

#define NAJVEC_BESED 100
#define NAJDALJSA_BESEDA 50

int main(void) {
    char besede[NAJVEC_BESED][NAJDALJSA_BESEDA + 1];
    int stevilo_besed = 0;

    while (stevilo_besed < NAJVEC_BESED &&
           scanf("%50s", besede[stevilo_besed]) == 1) {
        if (strcmp(besede[stevilo_besed], "EOF") == 0) {
            break;
        }
        stevilo_besed++;
    }

    for (int i = stevilo_besed - 1; i >= 0; i--) {
        printf("%s", besede[i]);
        if (i > 0) {
            printf(" ");
        }
    }
    printf("\n");

    return 0;
}
