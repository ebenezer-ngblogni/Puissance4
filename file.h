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

// Structure pour sauvegarder l'état d'une partie en pause
struct paused_game {
    char pseudo[50];
    char adversaire[50];  
    int grille_lignes;
    int grille_cols;
    float temps_par_coup;
    int forme_pions;
    int mode_jeu;         // 1=PvP, 0=PvIA
    int niveau_ia;        // 0 pour PvP, 1=FACILE, 2=DIFFICILE
    long temps_ecoule;    // en secondes
    int joueur_actuel;    // 0 ou 1
    char *grille_serialisee;
    Save *saves;
};
typedef struct paused_game PausedGame;

// Sauvegarder une partie en pause
int savePausedGame(char **grid, int ligne, int col, Profil p, char *pseudo_adv, int niveau_ia, long temps_ecoule, Save *saves, int joueur_actuel);

// Charger l'état d'une partie en pause (retourne NULL si aucune partie)
PausedGame* loadPausedGameState(Profil p);

// Reprendre et jouer une partie en pause (retourne 1 si succès, 0 sinon)
int resumePausedGame(Profil p);

// Libérer la mémoire d'une PausedGame
void freePausedGame(PausedGame *pg);

#endif // FILE_H_INCLUDED
