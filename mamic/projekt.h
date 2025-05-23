#ifndef PROJEKT_H
#define PROJEKT_H

#define MAX_NAZIV 100
#define MAX_AUTOR 100
#define MAX_DATUM 11
#define MAX_FILENAME 100

typedef struct {
    char file_name[MAX_FILENAME];
    char naziv[MAX_NAZIV];
    char autor[MAX_AUTOR];
    char datum[MAX_DATUM];
    int broj_klipova;
    float trajanje; // u sekundama
} VideoProjekt;

void unos_projekta(VideoProjekt* vp);
void spremi_binarno(VideoProjekt* vp, const char* ime_fajla);
void ispisi_sve(const char* ime_fajla);
void prikazi_projekt(const VideoProjekt* vp);

#endif
