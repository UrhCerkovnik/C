#define _CRT_SECURE_NO_WARNINGS
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

typedef struct bes {
    char beseda[MAX];
    struct bes *nasl;
    struct bes *naslCrka;
} beseda;

typedef int fIsci(beseda *, char *);

int vstaviUrejeno(beseda **zac, const char *word) {
    beseda *prejsnja = NULL;
    beseda *trenutna = *zac;

    while (trenutna != NULL && strcmp(trenutna->beseda, word) < 0) {
        prejsnja = trenutna;
        trenutna = trenutna->nasl;
    }

    if (trenutna != NULL && strcmp(trenutna->beseda, word) == 0) {
        return 1;
    }

    beseda *nova = malloc(sizeof(beseda));
    if (nova == NULL) {
        return 0;
    }
    strcpy(nova->beseda, word);
    nova->nasl = trenutna;
    nova->naslCrka = NULL;

    if (prejsnja == NULL) {
        *zac = nova;
    } else {
        prejsnja->nasl = nova;
    }
    return 1;
}

void izpisi(beseda *zac) {
    for (beseda *trenutna = zac; trenutna != NULL; trenutna = trenutna->nasl) {
        printf("%s\n", trenutna->beseda);
    }
}

int primerjajBesedi(const char *prva, const char *druga) {
    while (*prva != '\0' && *druga != '\0') {
        int znak1 = tolower((unsigned char)*prva);
        int znak2 = tolower((unsigned char)*druga);
        if (znak1 != znak2) {
            return znak1 - znak2;
        }
        prva++;
        druga++;
    }
    return tolower((unsigned char)*prva) - tolower((unsigned char)*druga);
}

int poisci(beseda *zac, char *word) {
    int skoki = 0;
    for (beseda *trenutna = zac; trenutna != NULL;) {
        int primerjava = primerjajBesedi(trenutna->beseda, word);
        if (primerjava == 0) {
            return skoki;
        }
        if (primerjava > 0) {
            return -1;
        }
        trenutna = trenutna->nasl;
        skoki++;
    }
    return -1;
}

void dopolniSeznam(beseda *zac) {
    for (beseda *trenutna = zac; trenutna != NULL; trenutna = trenutna->nasl) {
        trenutna->naslCrka = NULL;
    }

    if (zac == NULL) {
        return;
    }

    beseda *prvaCrke = zac;
    beseda *trenutna = zac->nasl;
    while (trenutna != NULL) {
        if (trenutna->beseda[0] != prvaCrke->beseda[0]) {
            prvaCrke->naslCrka = trenutna;
            prvaCrke = trenutna;
        }
        trenutna = trenutna->nasl;
    }
}

int poisciHitreje(beseda *zac, char *word) {
    int skoki = 0;
    int iskanaCrka = tolower((unsigned char)word[0]);
    beseda *trenutna = zac;

    while (trenutna != NULL) {
        int primerjava = primerjajBesedi(trenutna->beseda, word);
        if (primerjava == 0) {
            return skoki;
        }
        if (primerjava > 0) {
            return -1;
        }

        if (trenutna->naslCrka != NULL &&
            iskanaCrka > trenutna->beseda[0]) {
            trenutna = trenutna->naslCrka;
        } else {
            trenutna = trenutna->nasl;
        }
        skoki++;
    }
    return -1;
}

int povprecnoIskanje(fIsci *isci, beseda *zac) {
    long long vsota = 0;
    int stevilo = 0;

    for (beseda *trenutna = zac; trenutna != NULL; trenutna = trenutna->nasl) {
        int skoki = isci(zac, trenutna->beseda);
        if (skoki >= 0) {
            vsota += skoki;
            stevilo++;
        }
    }
    return stevilo == 0 ? 0 : (int)(vsota / stevilo);
}

void pocistiSeznam(beseda *zac) {
    while (zac != NULL) {
        beseda *naslednja = zac->nasl;
        free(zac);
        zac = naslednja;
    }
}

int jeCrka(int znak) {
    return (znak >= 'a' && znak <= 'z') || (znak >= 'A' && znak <= 'Z');
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uporaba: %s ime_datoteke\n", argv[0]);
        return 1;
    }

    FILE *datoteka = fopen(argv[1], "r");
    if (datoteka == NULL) {
        fprintf(stderr, "Napaka: datoteke ni mogoce odpreti.\n");
        return 1;
    }

    beseda *zacetek = NULL;
    char word[MAX];
    size_t dolzina = 0;
    int predolga = 0;
    int napaka = 0;
    int znak;

    while ((znak = fgetc(datoteka)) != EOF) {
        if (jeCrka(znak)) {
            if (dolzina < MAX - 1 && !predolga) {
                word[dolzina++] = (char)tolower((unsigned char)znak);
            } else {
                predolga = 1;
            }
        } else if (dolzina > 0 || predolga) {
            if (predolga) {
                fprintf(stderr, "Napaka: beseda je daljsa od %d znakov.\n", MAX - 1);
                napaka = 1;
                break;
            }
            word[dolzina] = '\0';
            if (!vstaviUrejeno(&zacetek, word)) {
                fprintf(stderr, "Napaka: pomnilnika ni bilo mogoce rezervirati.\n");
                napaka = 1;
                break;
            }
            dolzina = 0;
        }
    }

    if (!napaka && ferror(datoteka)) {
        fprintf(stderr, "Napaka pri branju datoteke.\n");
        napaka = 1;
    }
    if (!napaka && (dolzina > 0 || predolga)) {
        if (predolga) {
            fprintf(stderr, "Napaka: beseda je daljsa od %d znakov.\n", MAX - 1);
            napaka = 1;
        } else {
            word[dolzina] = '\0';
            if (!vstaviUrejeno(&zacetek, word)) {
                fprintf(stderr, "Napaka: pomnilnika ni bilo mogoce rezervirati.\n");
                napaka = 1;
            }
        }
    }
    fclose(datoteka);

    if (napaka) {
        pocistiSeznam(zacetek);
        return 1;
    }

    dopolniSeznam(zacetek);
    printf("Seznam urejenih besed:\n");
    izpisi(zacetek);

    printf("PI1: %d\n", povprecnoIskanje(poisci, zacetek));
    printf("PI2: %d\n", povprecnoIskanje(poisciHitreje, zacetek));

    char iskana[MAX];
    while (scanf("%99s", iskana) == 1) {
        printf("%d\n", poisci(zacetek, iskana));
        printf("%d\n", poisciHitreje(zacetek, iskana));
    }

    pocistiSeznam(zacetek);
    return 0;
}
