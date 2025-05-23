#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "projekt.h"

void meni() {
    printf("\n--- Video Galerija ---\n");
    printf("1. Dodaj novi projekt\n");
    printf("2. Prikazi sve projekte\n");
    printf("3. Izlaz\n");
    printf("Odaberi opciju: ");
}

int main() {
    VideoProjekt vp;
    int izbor;

    do {
        meni();
        scanf("%d", &izbor);
        getchar();  // očisti buffer

        switch (izbor) {
        case 1:
            unos_projekta(&vp);
            spremi_binarno(&vp, "projekti.dat");
            break;
        case 2:
            ispisi_sve("projekti.dat");
            break;
        case 3:
            printf("Izlaz.\n");
            break;
        default:
            printf("Nepoznata opcija.\n");
        }
    } while (izbor != 3);

    return 0;
}