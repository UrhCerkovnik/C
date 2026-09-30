#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

#define NAJVEC_TOCK 100

typedef struct {
    char ime[6];
    double x;
    double y;
} Tocka;

double oddaljenost_na_kvadrat(const Tocka *tocka) {
    return tocka->x * tocka->x + tocka->y * tocka->y;
}

void uredi_po_imenu(Tocka tocke[], int stevilo_tock) {
    for (int i = 0; i < stevilo_tock - 1; i++) {
        for (int j = 0; j < stevilo_tock - 1 - i; j++) {
            if (strcmp(tocke[j].ime, tocke[j + 1].ime) > 0) {
                Tocka zacasna = tocke[j];
                tocke[j] = tocke[j + 1];
                tocke[j + 1] = zacasna;
            }
        }
    }
}

void uredi_po_oddaljenosti(Tocka tocke[], int stevilo_tock) {
    for (int i = 0; i < stevilo_tock - 1; i++) {
        for (int j = 0; j < stevilo_tock - 1 - i; j++) {
            if (oddaljenost_na_kvadrat(&tocke[j]) >
                oddaljenost_na_kvadrat(&tocke[j + 1])) {
                Tocka zacasna = tocke[j];
                tocke[j] = tocke[j + 1];
                tocke[j + 1] = zacasna;
            }
        }
    }
}

void izpisi_tocke(const Tocka tocke[], int stevilo_tock) {
    for (int i = 0; i < stevilo_tock; i++) {
        printf("%s %.15g %.15g\n", tocke[i].ime, tocke[i].x, tocke[i].y);
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

    Tocka tocke[NAJVEC_TOCK];
    int stevilo_tock = 0;
    int rezultat;
    while (stevilo_tock < NAJVEC_TOCK) {
        rezultat = fscanf(datoteka, "%5s %lf %lf", tocke[stevilo_tock].ime,
                          &tocke[stevilo_tock].x, &tocke[stevilo_tock].y);
        if (rezultat == EOF) {
            break;
        }
        if (rezultat != 3) {
            fprintf(stderr, "Napaka: datoteka vsebuje neveljaven zapis tocke.\n");
            fclose(datoteka);
            return 1;
        }
        stevilo_tock++;
    }

    if (ferror(datoteka)) {
        fprintf(stderr, "Napaka pri branju datoteke.\n");
        fclose(datoteka);
        return 1;
    }

    if (stevilo_tock == NAJVEC_TOCK) {
        char dodatni_znak[2];
        if (fscanf(datoteka, "%1s", dodatni_znak) == 1) {
            fprintf(stderr, "Napaka: datoteka vsebuje vec kot 100 tock.\n");
            fclose(datoteka);
            return 1;
        }
    }

    if (fclose(datoteka) != 0) {
        fprintf(stderr, "Napaka pri zapiranju datoteke.\n");
        return 1;
    }

    Tocka po_imenu[NAJVEC_TOCK];
    Tocka po_oddaljenosti[NAJVEC_TOCK];
    for (int i = 0; i < stevilo_tock; i++) {
        po_imenu[i] = tocke[i];
        po_oddaljenosti[i] = tocke[i];
    }

    uredi_po_imenu(po_imenu, stevilo_tock);
    uredi_po_oddaljenosti(po_oddaljenosti, stevilo_tock);

    printf("Tocke po abecednem vrstnem redu:\n");
    izpisi_tocke(po_imenu, stevilo_tock);
    printf("Tocke po oddaljenosti od izhodisca:\n");
    izpisi_tocke(po_oddaljenosti, stevilo_tock);

    return 0;
}
