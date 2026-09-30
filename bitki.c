#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int stevilo_prizganih_bitov(unsigned int stevilo) {
    int stevilo_bitov = 0;

    while (stevilo != 0) {
        stevilo_bitov += stevilo & 1u;
        stevilo >>= 1;
    }
    return stevilo_bitov;
}

void izpisi_8_bitov(unsigned int stevilo) {
    for (int bit = 7; bit >= 0; bit--) {
        printf("%u", (stevilo >> bit) & 1u);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uporaba: %s i (i mora biti med 0 in 8)\n", argv[0]);
        return 1;
    }

    errno = 0;
    char *konec;
    long i = strtol(argv[1], &konec, 10);
    if (errno == ERANGE || konec == argv[1] || *konec != '\0' || i < 0 || i > 8) {
        fprintf(stderr, "Napaka: i mora biti celo stevilo med 0 in 8.\n");
        return 1;
    }

    int stevilo = 0;
    int vsota = 0;
    for (unsigned int vrednost = 0; vrednost < 256; vrednost++) {
        if (stevilo_prizganih_bitov(vrednost) == i) {
            izpisi_8_bitov(vrednost);
            printf(" = %u\n", vrednost);
            stevilo++;
            vsota += (int)vrednost;
        }
    }

    printf("i=%ld, n=%d, vsota=%d\n", i, stevilo, vsota);
    return 0;
}
