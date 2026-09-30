#define _CRT_SECURE_NO_WARNINGS
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ST_INTERVALOV 10
#define SIRINA_STOLPCA 7

void izpisi_oznako(const char *oznaka) {
    int dolzina = (int)strlen(oznaka);
    int levo = (SIRINA_STOLPCA - dolzina) / 2;
    int desno = SIRINA_STOLPCA - dolzina - levo;

    for (int i = 0; i < levo; i++) {
        printf(" ");
    }
    printf("%s", oznaka);
    for (int i = 0; i < desno; i++) {
        printf(" ");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2 && argc != 3) {
        fprintf(stderr, "Uporaba: %s N [M]\n", argv[0]);
        return 1;
    }

    errno = 0;
    char *konec;
    long n = strtol(argv[1], &konec, 10);
    if (errno == ERANGE || konec == argv[1] || *konec != '\0' || n < 0 || n > INT_MAX) {
        fprintf(stderr, "Napaka: N mora biti nenegativno celo stevilo.\n");
        return 1;
    }

    int omeji_visino = argc == 3;
    long m = 0;
    if (omeji_visino) {
        errno = 0;
        m = strtol(argv[2], &konec, 10);
        if (errno == ERANGE || konec == argv[2] || *konec != '\0' || m <= 0 || m > INT_MAX) {
            fprintf(stderr, "Napaka: M mora biti pozitivno celo stevilo.\n");
            return 1;
        }
    }

    int stevci[ST_INTERVALOV] = {0};
    srand((unsigned int)time(NULL));
    for (long i = 0; i < n; i++) {
        int stevilo = rand() % 100 + 1;
        stevci[(stevilo - 1) / 10]++;
    }

    int najvecji_stolpec = 0;
    for (int i = 0; i < ST_INTERVALOV; i++) {
        if (stevci[i] > najvecji_stolpec) {
            najvecji_stolpec = stevci[i];
        }
    }

    int visine[ST_INTERVALOV];
    int najvecja_visina = 0;
    for (int i = 0; i < ST_INTERVALOV; i++) {
        if (!omeji_visino || najvecji_stolpec == 0) {
            visine[i] = stevci[i];
        } else if (stevci[i] == 0) {
            visine[i] = 0;
        } else {
            visine[i] = (int)(((long long)stevci[i] * m + najvecji_stolpec / 2) /
                              najvecji_stolpec);
            if (visine[i] == 0) {
                visine[i] = 1;
            }
        }
        if (visine[i] > najvecja_visina) {
            najvecja_visina = visine[i];
        }
    }

    for (int vrstica = najvecja_visina; vrstica > 0; vrstica--) {
        for (int i = 0; i < ST_INTERVALOV; i++) {
            printf("%s", visine[i] >= vrstica ? "   o   " : "       ");
        }
        printf("\n");
    }

    for (int i = 0; i < ST_INTERVALOV; i++) {
        printf("-------");
    }
    printf("\n");

    const char *oznake[ST_INTERVALOV] = {
        "1-10", "11-20", "21-30", "31-40", "41-50",
        "51-60", "61-70", "71-80", "81-90", "91-100"
    };
    for (int i = 0; i < ST_INTERVALOV; i++) {
        izpisi_oznako(oznake[i]);
    }
    printf("\n");

    return 0;
}
