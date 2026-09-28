#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *elementi;
    int vrh;
    int velikost;
} Sklad;

int init(Sklad *sklad, int velikost);
int push(Sklad *sklad, int x);
int pop(Sklad *sklad);
int isEmpty(const Sklad *sklad);
void destroy(Sklad *sklad);

int preberi_stevilo(const char *poziv, int *stevilo) {
    char vrstica[100];

    printf("%s", poziv);
    if (fgets(vrstica, sizeof(vrstica), stdin) == NULL) {
        return 0;
    }
    return sscanf(vrstica, "%d", stevilo) == 1;
}

void izpisi_sklad(const Sklad *sklad) {
    if (isEmpty(sklad)) {
        printf("Sklad je prazen.\n");
        return;
    }

    printf("Vsebina sklada od vrha navzdol: ");
    for (int i = sklad->vrh - 1; i >= 0; i--) {
        printf("%d", sklad->elementi[i]);
        if (i > 0) {
            printf(", ");
        }
    }
    printf("\n");
}

void izpisi_meni(int trenutni_sklad) {
    printf("\nTrenutni sklad: %d\n", trenutni_sklad + 1);
    printf("0 - konec programa\n");
    printf("1 - dodaj element na sklad\n");
    printf("2 - brisi element s sklada\n");
    printf("3 - izpisi vsebino sklada\n");
    printf("4 - preklopi med skladi\n");
}

int main(void) {
    int stevilo_skladov;
    int velikost_sklada;

    if (!preberi_stevilo("Koliko skladov zelite uporabljati? ", &stevilo_skladov) ||
        stevilo_skladov <= 0) {
        printf("Stevilo skladov mora biti pozitivno.\n");
        return 1;
    }
    if (!preberi_stevilo("Koliko elementov naj lahko vsebuje posamezen sklad? ",
                         &velikost_sklada) || velikost_sklada <= 0) {
        printf("Velikost sklada mora biti pozitivna.\n");
        return 1;
    }

    Sklad *skladi = malloc(stevilo_skladov * sizeof(Sklad));
    if (skladi == NULL) {
        printf("Pomnilnika za sklade ni bilo mogoce rezervirati.\n");
        return 1;
    }

    int uspesna_inicializacija = 1;
    for (int i = 0; i < stevilo_skladov; i++) {
        if (!init(&skladi[i], velikost_sklada)) {
            uspesna_inicializacija = 0;
            for (int j = 0; j < i; j++) {
                destroy(&skladi[j]);
            }
            break;
        }
    }
    if (!uspesna_inicializacija) {
        printf("Skladov ni bilo mogoce inicializirati.\n");
        free(skladi);
        return 1;
    }

    int trenutni_sklad = 0;
    int ukaz;
    while (1) {
        izpisi_meni(trenutni_sklad);
        if (!preberi_stevilo("Vasa izbira: ", &ukaz)) {
            printf("Neveljaven ukaz.\n");
            continue;
        }

        if (ukaz == 0) {
            break;
        } else if (ukaz == 1) {
            int element;
            if (!preberi_stevilo("Vnesite element: ", &element)) {
                printf("Element mora biti celo stevilo.\n");
            } else if (!push(&skladi[trenutni_sklad], element)) {
                printf("Sklad je poln.\n");
            }
        } else if (ukaz == 2) {
            if (isEmpty(&skladi[trenutni_sklad])) {
                printf("Sklad je prazen.\n");
            } else {
                printf("Odstranjen element: %d\n", pop(&skladi[trenutni_sklad]));
            }
        } else if (ukaz == 3) {
            izpisi_sklad(&skladi[trenutni_sklad]);
        } else if (ukaz == 4) {
            int nov_sklad;
            if (!preberi_stevilo("Na kateri sklad zelite preklopiti (1-...)? ",
                                 &nov_sklad) ||
                nov_sklad < 1 || nov_sklad > stevilo_skladov) {
                printf("Neveljavna stevilka sklada.\n");
            } else {
                trenutni_sklad = nov_sklad - 1;
            }
        } else {
            printf("Neveljaven ukaz.\n");
        }
    }

    for (int i = 0; i < stevilo_skladov; i++) {
        destroy(&skladi[i]);
    }
    free(skladi);
    return 0;
}