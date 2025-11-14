#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED
#include "profil.h"

/* ================================================================== */
/* FONCTIONS UTILITAIRES (INTERNES)                    */
/* ================================================================== */
void menu_clear_screen();

void menu_lire_chaine_securise(char* buffer, int taille);

int menu_lire_choix_int();

float menu_lire_choix_float();


/* ================================================================== */
/* FONCTIONS DE LOGIN / ACCUEIL                       */
/* ================================================================== */
void menu_afficher_bienvenue();

void menu_demander_pseudo(char* buffer_pseudo);

void menu_afficher_erreur_pseudo();

void menu_afficher_bon_retour(char* pseudo);

void menu_afficher_nouveau_profil(char* pseudo);

/* ================================================================== */
/* MENU PRINCIPAL                               */
/* ================================================================== */
int menu_afficher_principal(Profil* p);

void menu_afficher_au_revoir();

/* ================================================================== */
/* SOUS-MENU PARAMETRES                           */
/* ================================================================== */
int menu_afficher_menu_parametres(Profil* p);

void menu_demander_nouvelle_grille(int* lignes, int* cols);

float menu_demander_nouveau_tp();

int menu_demander_nouvelle_forme();

int menu_demander_nouveau_mode_defaut();

/* ================================================================== */
/* MESSAGES D'ERREUR                            */
/* ================================================================== */
void menu_afficher_erreur_choix_invalide();


#endif // MENU_H_INCLUDED
