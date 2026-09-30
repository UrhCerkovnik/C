#define _CRT_SECURE_NO_WARNINGS
#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *ime;
    char *priimek;
    char *telefon;
} oseba;

char *kopiraj_niz(const char *niz) {
    size_t velikost = strlen(niz) + 1;
    char *kopija = malloc(velikost);
    if (kopija != NULL) {
        strcpy(kopija, niz);
    }
    return kopija;
}

void sprosti_osebe(oseba *osebe, size_t stevilo) {
    if (osebe == NULL) {
        return;
    }
    for (size_t i = 0; i < stevilo; i++) {
        free(osebe[i].ime);
        free(osebe[i].priimek);
        free(osebe[i].telefon);
    }
    free(osebe);
}

int primerjaj_osebe(const void *prva, const void *druga) {
    const oseba *oseba1 = prva;
    const oseba *oseba2 = druga;
    int primerjava = strcmp(oseba1->priimek, oseba2->priimek);
    if (primerjava == 0) {
        primerjava = strcmp(oseba1->ime, oseba2->ime);
    }
    return primerjava;
}

void odstrani_konec_vrstice(char *vrstica) {
    size_t dolzina = strlen(vrstica);
    while (dolzina > 0 && (vrstica[dolzina - 1] == '\n' || vrstica[dolzina - 1] == '\r')) {
        vrstica[--dolzina] = '\0';
    }
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

    char vrstica[100];
    if (fgets(vrstica, sizeof(vrstica), datoteka) == NULL) {
        fprintf(stderr, "Napaka: datoteka ne vsebuje stevila oseb.\n");
        fclose(datoteka);
        return 1;
    }

    errno = 0;
    char *konec;
    long stevilo_vrstic = strtol(vrstica, &konec, 10);
    int brez_stevila = konec == vrstica;
    while (isspace((unsigned char)*konec)) {
        konec++;
    }
    if (errno == ERANGE || brez_stevila || *konec != '\0' || stevilo_vrstic < 0 ||
        (unsigned long long)stevilo_vrstic >
            (unsigned long long)(SIZE_MAX / sizeof(oseba))) {
        fprintf(stderr, "Napaka: neveljavno stevilo oseb.\n");
        fclose(datoteka);
        return 1;
    }

    size_t stevilo_oseb = (size_t)stevilo_vrstic;
    oseba *osebe = NULL;
    size_t poraba_pomnilnika = stevilo_oseb * sizeof(oseba);
    if (stevilo_oseb > 0) {
        osebe = calloc(stevilo_oseb, sizeof(oseba));
        if (osebe == NULL) {
            fprintf(stderr, "Napaka: pomnilnika ni bilo mogoce rezervirati.\n");
            fclose(datoteka);
            return 1;
        }
    }

    for (size_t i = 0; i < stevilo_oseb; i++) {
        if (fgets(vrstica, sizeof(vrstica), datoteka) == NULL) {
            fprintf(stderr, "Napaka: v datoteki je premalo oseb.\n");
            sprosti_osebe(osebe, stevilo_oseb);
            fclose(datoteka);
            return 1;
        }
        odstrani_konec_vrstice(vrstica);

        char *locilo1 = strchr(vrstica, ':');
        char *locilo2 = locilo1 == NULL ? NULL : strchr(locilo1 + 1, ':');
        if (locilo1 == NULL || locilo2 == NULL || strchr(locilo2 + 1, ':') != NULL) {
            fprintf(stderr, "Napaka: neveljaven zapis osebe v vrstici %zu.\n", i + 2);
            sprosti_osebe(osebe, stevilo_oseb);
            fclose(datoteka);
            return 1;
        }

        *locilo1 = '\0';
        *locilo2 = '\0';
        const char *ime = vrstica;
        const char *priimek = locilo1 + 1;
        const char *telefon = locilo2 + 1;
        size_t velikosti_nizov[3] = {
            strlen(ime) + 1,
            strlen(priimek) + 1,
            strlen(telefon) + 1
        };
        if (velikosti_nizov[0] > SIZE_MAX - poraba_pomnilnika ||
            velikosti_nizov[1] > SIZE_MAX - poraba_pomnilnika - velikosti_nizov[0] ||
            velikosti_nizov[2] > SIZE_MAX - poraba_pomnilnika - velikosti_nizov[0] -
                                      velikosti_nizov[1]) {
            fprintf(stderr, "Napaka: velikost podatkov presega naslovni prostor.\n");
            sprosti_osebe(osebe, stevilo_oseb);
            fclose(datoteka);
            return 1;
        }

        osebe[i].ime = kopiraj_niz(ime);
        osebe[i].priimek = kopiraj_niz(priimek);
        osebe[i].telefon = kopiraj_niz(telefon);
        if (osebe[i].ime == NULL || osebe[i].priimek == NULL || osebe[i].telefon == NULL) {
            fprintf(stderr, "Napaka: pomnilnika ni bilo mogoce rezervirati.\n");
            sprosti_osebe(osebe, stevilo_oseb);
            fclose(datoteka);
            return 1;
        }
        poraba_pomnilnika += velikosti_nizov[0] + velikosti_nizov[1] + velikosti_nizov[2];
    }

    if (fclose(datoteka) != 0) {
        fprintf(stderr, "Napaka pri zapiranju datoteke.\n");
        sprosti_osebe(osebe, stevilo_oseb);
        return 1;
    }

    printf("Rezervirani pomnilnik: %zu bajtov\n", poraba_pomnilnika);
    if (stevilo_oseb > 1) {
        qsort(osebe, stevilo_oseb, sizeof(oseba), primerjaj_osebe);
    }
    for (size_t i = 0; i < stevilo_oseb; i++) {
        printf("%s:%s:%s\n", osebe[i].ime, osebe[i].priimek, osebe[i].telefon);
    }

    sprosti_osebe(osebe, stevilo_oseb);
    return 0;
}
