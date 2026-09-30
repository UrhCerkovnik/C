#define _CRT_SECURE_NO_WARNINGS
#include <ctype.h>
#include <stdio.h>

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

    long long vrstice = 0;
    long long besede = 0;
    long long znaki = 0;
    int v_besedi = 0;
    int znak;

    while ((znak = fgetc(datoteka)) != EOF) {
        znaki++;
        if (znak == '\n') {
            vrstice++;
        }

        if (isspace((unsigned char)znak)) {
            v_besedi = 0;
        } else if (!v_besedi) {
            besede++;
            v_besedi = 1;
        }
    }

    if (ferror(datoteka)) {
        fprintf(stderr, "Napaka pri branju datoteke.\n");
        fclose(datoteka);
        return 1;
    }
    fclose(datoteka);

    printf("vrstic: %lld, besed: %lld, znakov: %lld\n", vrstice, besede, znaki);
    return 0;
}
