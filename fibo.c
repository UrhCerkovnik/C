#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Nepravilno stevilo argumentov. Podaj samo en dodaten argument.\n");
        return 1;
    }

    int n = atoi(argv[1]);
    if (n < 0) {
        printf("Stevilo mora biti nenegativno.\n");
        return 1;
    }

    int prejsnje = 0;
    int trenutno = 1;
    int korak = 0;
    while (korak < n) {
        int naslednje = prejsnje + trenutno;
        prejsnje = trenutno;
        trenutno = naslednje;
        korak++;
    }

    printf("%d\n", prejsnje);

    return 0;
}