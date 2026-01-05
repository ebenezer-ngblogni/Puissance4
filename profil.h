#ifndef PROFIL_H_INCLUDED
#define PROFIL_H_INCLUDED
#include "utils.h"

// Définition de la structure Profil
struct profil{
    char pseudo[50];
    int grille_lignes;
    int grille_cols;
    float temps_par_coup;
    int forme_pions;
    int mode_jeu;
};
typedef struct profil Profil;

// Gère la connexion ou la création de profil du joueur à l'aide de son pseudo
Profil profile_login_or_create();

// Valide le format du pseudo (lettres, chiffres, _)
int profile_validate_pseudo(char* pseudo);

// Charge le profil depuis le fichier de configuration associé au pseudo
Profil profile_load(char* pseudo);

// Sauvegarde le profil dans le fichier de configuration associé
void profile_save(Profil* p);

// Crée un profil par défaut avec les paramètres standards
Profil profile_create_default(char* pseudo);

// Modifie les paramètres du profil actif
void profile_modify_settings(Profil* p);

// Initialise le dossier "files/" pour stocker les profils
void profile_initialize_directory();

#endif
