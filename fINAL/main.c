#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "projekt.h"

#define OPCIJA_DODAJ 1
#define OPCIJA_POKAZI 2
#define OPCIJA_OSVJEZI 3
#define OPCIJA_OBRISI 4
#define OPCIJA_SORTIRAJ 5
#define OPCIJA_STATISTIKA 6
#define OPCIJA_EKSPORT 7
#define OPCIJA_IZLAZ 8

void meni() {
    printf("\n--- Video Galerija ---\n");
    printf("1. Dodaj novi projekt\n");
    printf("2. Prikaži sve projekte\n");
    printf("3. Osvježi projekt\n");
    printf("4. Obriši projekt\n");
    printf("5. Sortiraj i prikaži projekte\n");
    printf("6. Prikaži statistiku\n");
    printf("7. Izvezi u tekstualnu datoteku\n");
    printf("8. Izlaz\n");
    printf("Odaberi opciju: ");
}

int main() {
    VideoProjekt vp;
    int izbor;
    const char* datoteka = "projekti.dat";

    do {
        meni();
        scanf("%d", &izbor);
        getchar();  // očisti buffer od novog reda

        switch (izbor) {
        case OPCIJA_DODAJ:
            unos_projekta(&vp);
            spremi_binarno(&vp, datoteka);
            break;
        case OPCIJA_POKAZI:
            ispisi_sve(datoteka);
            break;
        case OPCIJA_OSVJEZI:
            osvjezi_projekt(datoteka);
            break;
        case OPCIJA_OBRISI:
            obrisi_projekt(datoteka);
            break;
        case OPCIJA_SORTIRAJ:
            sortiraj_i_prikazi(datoteka);
            break;
        case OPCIJA_STATISTIKA:
            prikazi_statistiku(datoteka);
            break;
        case OPCIJA_EKSPORT:
            eksport_u_tekst(datoteka, "projekti.txt");
            break;
        case OPCIJA_IZLAZ:
            printf("Izlaz iz programa.\n");
            break;
        default:
            printf("Nepoznata opcija, pokušajte ponovno.\n");
        }
    } while (izbor != OPCIJA_IZLAZ);

    return 0;
}
