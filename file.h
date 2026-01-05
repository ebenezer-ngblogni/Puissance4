#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED

#include "profil.h"

// Définition de la structure Save pour la liste chaînée des coups sauvegardés
struct save{
    int player;
    int coup;
    struct save *next;
};
typedef struct save Save;

// Fonction d'ajout du coup à la liste chaînée des sauvegardes
void getCoup(int isPlayer, int coup, Save **saves);

// Fonction d'enregistrement de la liste des sauvegardes dans le fichier de configuration associe au joueur
void saveIntoFile(Save *saves, Profil p, char *pseudo_adv, long score_time, char *message_victoire);

// Fonction d'affichage coup par coup de la grille de jeu d'une partie sauvegardée à partir d'un tableau temporaire
void newGrid(int table[][2], char *message, int nbre_coups, int line, int col, Profil p, char *message_victoire);

// Fonction de chargement et d'affichage d'une partie sauvegardée depuis le fichier de configuration associe au joueur
void loadGame(Profil p);
#endif // FILE_H_INCLUDED
