#ifndef PROFIL_H_INCLUDED
#define PROFIL_H_INCLUDED
#include "utils.h"


struct profil{
    char pseudo[50];
    int grille_lignes;
    int grille_cols;
    float temps_par_coup;
    int forme_pions;
    int mode_par_defaut;
};
typedef struct profil Profil;

Profil profil_login_ou_creer();
int profil_valider_pseudo(char* pseudo);
Profil profil_charger(char* pseudo);
void profil_sauvegarder(Profil* p);
Profil profil_creer_defaut(char* pseudo);
void profil_modifier_parametres(Profil* p);
void profil_initialiser_dossier();

#endif
