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

Profil profile_login_or_create();
int profile_validate_pseudo(char* pseudo);
Profil profile_load(char* pseudo);
void profile_save(Profil* p);
Profil profile_create_default(char* pseudo);
void profile_modify_settings(Profil* p);
void profile_initialize_directory();

#endif
