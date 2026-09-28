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

int init(Sklad *sklad, int velikost) {
    if (sklad == NULL || velikost <= 0) {
        return 0;
    }

    sklad->elementi = malloc(velikost * sizeof(int));
    if (sklad->elementi == NULL) {
        sklad->vrh = 0;
        sklad->velikost = 0;
        return 0;
    }

    sklad->vrh = 0;
    sklad->velikost = velikost;
    return 1;
}

int push(Sklad *sklad, int x) {
    if (sklad == NULL || sklad->vrh >= sklad->velikost) {
        return 0;
    }

    sklad->elementi[sklad->vrh] = x;
    sklad->vrh++;
    return 1;
}

int pop(Sklad *sklad) {
    if (sklad == NULL || isEmpty(sklad)) {
        return -1;
    }

    sklad->vrh--;
    return sklad->elementi[sklad->vrh];
}

int isEmpty(const Sklad *sklad) {
    return sklad == NULL || sklad->vrh == 0;
}

void destroy(Sklad *sklad) {
    if (sklad != NULL) {
        free(sklad->elementi);
        sklad->elementi = NULL;
        sklad->vrh = 0;
        sklad->velikost = 0;
    }
}