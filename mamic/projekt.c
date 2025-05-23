#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "projekt.h"

void unos_projekta(VideoProjekt* vp) {
    printf("Unesite ime .prproj datoteke: ");
    fgets(vp->file_name, MAX_FILENAME, stdin);
    vp->file_name[strcspn(vp->file_name, "\n")] = '\0';

    printf("Naziv projekta: ");
    fgets(vp->naziv, MAX_NAZIV, stdin);
    vp->naziv[strcspn(vp->naziv, "\n")] = '\0';

    printf("Ime autora: ");
    fgets(vp->autor, MAX_AUTOR, stdin);
    vp->autor[strcspn(vp->autor, "\n")] = '\0';

    printf("Datum (dd/mm/yyyy): ");
    fgets(vp->datum, MAX_DATUM, stdin);
    vp->datum[strcspn(vp->datum, "\n")] = '\0';

    printf("Broj klipova u timelineu: ");
    scanf("%d", &vp->broj_klipova);

    printf("Ukupno trajanje (u sekundama): ");
    scanf("%f", &vp->trajanje);

    // Očisti ulazni buffer od \n nakon scanf
    while (getchar() != '\n');
}

void spremi_binarno(VideoProjekt* vp, const char* ime_fajla) {
    FILE* fp = fopen(ime_fajla, "ab");
    if (!fp) {
        perror("Ne mogu otvoriti datoteku za spremanje");
        return;
    }
    fwrite(vp, sizeof(VideoProjekt), 1, fp);
    fclose(fp);
}

void ispisi_sve(const char* ime_fajla) {
    FILE* fp = fopen(ime_fajla, "rb");
    if (!fp) {
        perror("Ne mogu otvoriti datoteku za čitanje");
        return;
    }

    VideoProjekt vp;
    printf("\n--- Svi projekti ---\n");
    while (fread(&vp, sizeof(VideoProjekt), 1, fp) == 1) {
        prikazi_projekt(&vp);
        printf("\n");
    }

    fclose(fp);
}

void prikazi_projekt(const VideoProjekt* vp) {
    printf("File name: %s\n", vp->file_name);
    printf("Naziv projekta: %s\n", vp->naziv);
    printf("Ime autora: %s\n", vp->autor);
    printf("Datum: %s\n", vp->datum);
    printf("Broj klipova u timelineu: %d\n", vp->broj_klipova);
    printf("Ukupno trajanje: %.0fs\n", vp->trajanje);
}
