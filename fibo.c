#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if(argc != 2){
        printf("Nepravilno stevilo argumentov. Podaj samo en dodaten argument");
        return 1;
    }
    int n = atoi(argv[1]);
    int a = 0, b = 1;
    int i = 0;
    while(i < n){
        int next = a + b;
        a = b;
        b = next;
        i++;
    }

    printf("%d\n", a);

    return 0;
}