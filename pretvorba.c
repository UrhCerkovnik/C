#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int pretvori_v_sekunde(const char *niz, long long *sekunde) {
    long long ure = 0;
    int indeks = 0;
    int st_ur = 0;

    while (niz[indeks] >= '0' && niz[indeks] <= '9') {
        long long najvecjeUre = (LLONG_MAX - 3599) / 3600;
        int naslednjaStevka = niz[indeks] - '0';

        if (ure > najvecjeUre / 10) {
            return 0;
        }
        if (ure == najvecjeUre / 10 && naslednjaStevka > najvecjeUre % 10) {
            return 0;
        }
        ure = ure * 10 + naslednjaStevka;
        indeks++;
        st_ur++;
    }

    if (st_ur < 2 || niz[indeks] != ':' ||
        niz[indeks + 1] < '0' || niz[indeks + 1] > '9' ||
        niz[indeks + 2] < '0' || niz[indeks + 2] > '9') {
        return 0;
    }

    int minute = (niz[indeks + 1] - '0') * 10 + (niz[indeks + 2] - '0');
    indeks += 3;

    int sekunde_v_minuti = 0;
    if (niz[indeks] == ':') {
        if (niz[indeks + 1] < '0' || niz[indeks + 1] > '9' ||
            niz[indeks + 2] < '0' || niz[indeks + 2] > '9') {
            return 0;
        }
        sekunde_v_minuti = (niz[indeks + 1] - '0') * 10 + (niz[indeks + 2] - '0');
        indeks += 3;
    }

    if (niz[indeks] != '\0' || minute >= 60 || sekunde_v_minuti >= 60) {
        return 0;
    }

    *sekunde = ure * 3600 + minute * 60 + sekunde_v_minuti;
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 11) {
        fprintf(stderr, "Uporaba: %s vrednost [vrednost ...] (najvec 10 argumentov)\n",
                argv[0]);
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        if (strchr(argv[i], ':') != NULL) {
            long long pretvorjene_sekunde;
            if (!pretvori_v_sekunde(argv[i], &pretvorjene_sekunde)) {
                fprintf(stderr, "Napaka: '%s' ni veljaven cas v obliki HH:MM ali HH:MM:SS.\n",
                        argv[i]);
                return 1;
            }
            printf("%s = %llds\n", argv[i], pretvorjene_sekunde);
        } else {
            errno = 0;
            char *konec;
            long long sekunde = strtoll(argv[i], &konec, 10);
            if (errno == ERANGE || konec == argv[i] || *konec != '\0' || sekunde < 0) {
                fprintf(stderr, "Napaka: '%s' ni nenegativno celo stevilo sekund.\n",
                        argv[i]);
                return 1;
            }

            long long ure = sekunde / 3600;
            printf("%llds = %02lld:%02lld:%02lld\n", sekunde, ure,
                   (sekunde % 3600) / 60, sekunde % 60);
        }
    }

    return 0;
}
