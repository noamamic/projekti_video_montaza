#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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

    while (getchar() != '\n');  // očisti buffer

    // Rekurzivno pitamo korisnika želi li dodati još jedan projekt
    char nastavak;
    printf("Želite li dodati još jedan projekt? (y/n): ");
    scanf(" %c", &nastavak);
    if (nastavak == 'y' || nastavak == 'Y') {
        unos_projekta(vp); // Rekurzija
    }
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
    printf("Ime datoteke: %s\n", vp->file_name);
    printf("Naziv projekta: %s\n", vp->naziv);
    printf("Ime autora: %s\n", vp->autor);
    printf("Datum: %s\n", vp->datum);
    printf("Broj klipova u timelineu: %d\n", vp->broj_klipova);
    printf("Ukupno trajanje: %.0fs\n", vp->trajanje);
}

void obrisi_projekt(const char* ime_fajla) {
    FILE* fp = fopen(ime_fajla, "rb");
    if (!fp) {
        perror("Ne mogu otvoriti datoteku");
        return;
    }

    char trazeni_file_name[MAX_FILENAME];
    printf("Unesite ime .prproj datoteke projekta za brisanje: ");
    fgets(trazeni_file_name, MAX_FILENAME, stdin);
    trazeni_file_name[strcspn(trazeni_file_name, "\n")] = '\0';

    FILE* temp = fopen("temp.dat", "wb");
    if (!temp) {
        perror("Ne mogu otvoriti privremenu datoteku");
        fclose(fp);
        return;
    }

    VideoProjekt vp;
    int found = 0;
    while (fread(&vp, sizeof(VideoProjekt), 1, fp) == 1) {
        if (strcmp(vp.file_name, trazeni_file_name) == 0) {
            found = 1;
            continue;
        }
        fwrite(&vp, sizeof(VideoProjekt), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    if (found) {
        remove(ime_fajla);
        rename("temp.dat", ime_fajla);
        printf("Projekt je obrisan.\n");
    }
    else {
        remove("temp.dat");
        printf("Projekt s tim imenom nije pronađen.\n");
    }

    // Rekurzivno pitamo korisnika želi li obrisati još jedan projekt
    char nastavak;
    printf("Želite li obrisati još jedan projekt? (y/n): ");
    scanf(" %c", &nastavak);
    if (nastavak == 'y' || nastavak == 'Y') {
        obrisi_projekt(ime_fajla); // Rekurzija
    }
}

void osvjezi_projekt(const char* ime_fajla) {
    FILE* fp = fopen(ime_fajla, "r+b");
    if (!fp) {
        perror("Ne mogu otvoriti datoteku");
        return;
    }

    char trazeni_file_name[MAX_FILENAME];
    printf("Unesite ime .prproj datoteke projekta koji želite osvježiti: ");
    fgets(trazeni_file_name, MAX_FILENAME, stdin);
    trazeni_file_name[strcspn(trazeni_file_name, "\n")] = '\0';

    VideoProjekt vp;
    int found = 0;

    while (fread(&vp, sizeof(VideoProjekt), 1, fp) == 1) {
        if (strcmp(vp.file_name, trazeni_file_name) == 0) {
            found = 1;
            printf("Pronađen projekt. Unesite nove podatke:\n");
            unos_projekta(&vp);

            fseek(fp, -(long)sizeof(VideoProjekt), SEEK_CUR);
            fwrite(&vp, sizeof(VideoProjekt), 1, fp);
            printf("Projekt je osvježen.\n");
            break;
        }
    }

    if (!found) {
        printf("Projekt s tim imenom nije pronađen.\n");
    }

    fclose(fp);
}

int usporedi_projekte(const void* a, const void* b) {
    const VideoProjekt* vp1 = (const VideoProjekt*)a;
    const VideoProjekt* vp2 = (const VideoProjekt*)b;
    return strcmp(vp1->naziv, vp2->naziv);
}

void sortiraj_i_prikazi(const char* ime_fajla) {
    FILE* fp = fopen(ime_fajla, "rb");
    if (!fp) {
        perror("Ne mogu otvoriti datoteku");
        return;
    }

    fseek(fp, 0, SEEK_END);
    long vel = ftell(fp);
    int broj = (int)(vel / sizeof(VideoProjekt));
    rewind(fp);

    if (broj == 0) {
        printf("Nema projekata za sortiranje.\n");
        fclose(fp);
        return;
    }

    VideoProjekt* projekti = (VideoProjekt*)malloc(broj * sizeof(VideoProjekt));
    if (!projekti) {
        printf("Nema dovoljno memorije.\n");
        fclose(fp);
        return;
    }

    fread(projekti, sizeof(VideoProjekt), broj, fp);
    fclose(fp);

    qsort(projekti, broj, sizeof(VideoProjekt), usporedi_projekte);

    printf("\n--- Sortirani projekti po nazivu ---\n");
    for (int i = 0; i < broj; i++) {
        prikazi_projekt(&projekti[i]);
        printf("\n");
    }

    free(projekti);
}

void prikazi_statistiku(const char* ime_fajla) {
    FILE* fp = fopen(ime_fajla, "rb");
    if (!fp) {
        perror("Ne mogu otvoriti datoteku");
        return;
    }

    int broj_projekata = 0;
    int ukupno_klipova = 0;
    float ukupno_trajanje = 0.0f;

    VideoProjekt vp;
    while (fread(&vp, sizeof(VideoProjekt), 1, fp) == 1) {
        broj_projekata++;
        ukupno_klipova += vp.broj_klipova;
        ukupno_trajanje += vp.trajanje;
    }
    fclose(fp);

    if (broj_projekata == 0) {
        printf("Nema projekata za statistiku.\n");
        return;
    }

    printf("\n--- Statistika projekata ---\n");
    printf("Ukupan broj projekata: %d\n", broj_projekata);
    printf("Ukupan broj klipova: %d\n", ukupno_klipova);
    printf("Ukupno trajanje svih projekata: %.2f sekundi\n", ukupno_trajanje);
    printf("Prosječno trajanje projekta: %.2f sekundi\n", ukupno_trajanje / broj_projekata);
}

void eksport_u_tekst(const char* binarna_dat, const char* tekstualna_dat) {
    FILE* bin_fp = fopen(binarna_dat, "rb");
    if (!bin_fp) {
        perror("Ne mogu otvoriti binarnu datoteku");
        return;
    }

    FILE* txt_fp = fopen(tekstualna_dat, "w");
    if (!txt_fp) {
        perror("Ne mogu otvoriti tekstualnu datoteku");
        fclose(bin_fp);
        return;
    }

    VideoProjekt vp;
    while (fread(&vp, sizeof(VideoProjekt), 1, bin_fp) == 1) {
        fprintf(txt_fp, "Ime datoteke: %s\n", vp.file_name);
        fprintf(txt_fp, "Naziv projekta: %s\n", vp.naziv);
        fprintf(txt_fp, "Ime autora: %s\n", vp.autor);
        fprintf(txt_fp, "Datum: %s\n", vp.datum);
        fprintf(txt_fp, "Broj klipova: %d\n", vp.broj_klipova);
        fprintf(txt_fp, "Trajanje: %.0fs\n\n", vp.trajanje);
    }

    fclose(bin_fp);
    fclose(txt_fp);
    printf("Podaci su eksportirani u '%s'.\n", tekstualna_dat);
}
