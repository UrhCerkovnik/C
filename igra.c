#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define VELIKOST 7

void izpisi_plosco(char plosca[VELIKOST][VELIKOST]) {
    printf("  1 2 3 4 5 6 7\n");
    for (int vrstica = 0; vrstica < VELIKOST; vrstica++) {
        printf("%d ", vrstica + 1);
        for (int stolpec = 0; stolpec < VELIKOST; stolpec++) {
            printf("%c ", plosca[vrstica][stolpec]);
        }
        printf("\n");
    }
}

int stiri_v_vrsti(char plosca[VELIKOST][VELIKOST], int vrstica, int stolpec) {
    const int smeri[4][2] = {{0, 1}, {1, 0}, {1, 1}, {1, -1}};
    char figura = plosca[vrstica][stolpec];

    for (int i = 0; i < 4; i++) {
        int stevilo = 1;
        for (int smer = -1; smer <= 1; smer += 2) {
            int nova_vrstica = vrstica + smer * smeri[i][0];
            int nov_stolpec = stolpec + smer * smeri[i][1];
            while (nova_vrstica >= 0 && nova_vrstica < VELIKOST &&
                   nov_stolpec >= 0 && nov_stolpec < VELIKOST &&
                   plosca[nova_vrstica][nov_stolpec] == figura) {
                stevilo++;
                nova_vrstica += smer * smeri[i][0];
                nov_stolpec += smer * smeri[i][1];
            }
        }
        if (stevilo >= 4) {
            return 1;
        }
    }
    return 0;
}

int main(void) {
    char plosca[VELIKOST][VELIKOST];
    int napake[2] = {0, 0};
    int zapolnjena_polja = 0;
    int igralec = 0;
    const char figure[2] = {'C', 'B'};

    for (int vrstica = 0; vrstica < VELIKOST; vrstica++) {
        for (int stolpec = 0; stolpec < VELIKOST; stolpec++) {
            plosca[vrstica][stolpec] = '.';
        }
    }

    printf("Stiri v vrsto: crni igralec je C, beli igralec je B.\n");
    izpisi_plosco(plosca);

    while (1) {
        int vrstica;
        int stolpec;
        char vnos[100];

        printf("Igralec %c, vnesite vrstico in stolpec (1-7): ", figure[igralec]);
        if (fgets(vnos, sizeof(vnos), stdin) == NULL) {
            printf("Vhod je bil zakljucen.\n");
            break;
        }
        if (sscanf(vnos, "%d %d", &vrstica, &stolpec) != 2) {
            napake[igralec]++;
            printf("Napaka: vnesite dve stevili med 1 in 7 (%d/3).\n", napake[igralec]);
        } else if (vrstica < 1 || vrstica > VELIKOST ||
                   stolpec < 1 || stolpec > VELIKOST ||
                   plosca[vrstica - 1][stolpec - 1] != '.') {
            napake[igralec]++;
            printf("Napaka: polje ni prazno ali koordinate niso veljavne (%d/3).\n",
                   napake[igralec]);
        } else {
            vrstica--;
            stolpec--;
            plosca[vrstica][stolpec] = figure[igralec];
            zapolnjena_polja++;
            izpisi_plosco(plosca);

            if (stiri_v_vrsti(plosca, vrstica, stolpec)) {
                printf("Zmaga igralca %c!\n", figure[igralec]);
                break;
            }
            if (zapolnjena_polja == VELIKOST * VELIKOST) {
                printf("Igra je neodlocena.\n");
                break;
            }
            igralec = 1 - igralec;
            continue;
        }

        if (napake[igralec] >= 3) {
            printf("Igralec %c je trikrat naredil napako. Zmaga igralec %c!\n",
                   figure[igralec], figure[1 - igralec]);
            break;
        }
    }

    return 0;
}