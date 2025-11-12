#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED
#include "profil.h"

// int menuPrincipal();
// int getChoiceMenuPrincipal();

void menu_afficher_bienvenue();

void menu_demander_pseudo(char* buffer_pseudo);

void menu_afficher_erreur_pseudo();

void menu_afficher_bon_retour(char* pseudo);

void menu_afficher_nouveau_profil(char* pseudo);

int menu_afficher_principal(Profil* p);

void menu_afficher_au_revoir();

int menu_afficher_menu_parametres(Profil* p);

void menu_demander_nouvelle_grille(int* lignes, int* cols);

float menu_demander_nouveau_tp();

int menu_demander_nouvelle_forme();

int menu_demander_nouveau_mode_defaut();

void menu_afficher_erreur_choix_invalide();

int menu_lire_choix_int();

void menu_lire_chaine_securise(char* buffer, int taille);


#endif // MENU_H_INCLUDED
