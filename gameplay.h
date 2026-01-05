#ifndef GAMEPLAY_H_INCLUDED
#define GAMEPLAY_H_INCLUDED
#include "utils.h"
#include "profil.h"
#include "file.h"

// Lit un coup dans un delai imparti du timer( valeur contenu dans la constante TiMER_PLAY) 
int waitToPlay(int *coup, int delay);

// Pause pour afficher les messages
void pauseToDisplay();


typedef struct size
{
    int width;
    int height;
} size;


struct game
{
    size sizes;
    int grille [5][6];
    // int grille [sizes.height][sizes.width]
};

// Fonctionnalité du mode 2 joueurs
void twoPlayer(Profil p);

// Vérifie si un joueur a gagné la partie
int winPosition(char **grid ,int line, int col, char symbole);

// Vérifie si la grille est pleine (cas de match nul)
int drawGame(char **grid , int col);

// Fonctionnalité du mode joueur vs IA
void playerVsIa(Profil p, NIVEAU lvl);

// Fonctionnalité de l'IA de niveau facile
void IAEasy(Profil p, char **grid, Save **saves);

// Fonctionnalité de l'IA de niveau moyen
void IAMedium(Profil p, char **grid, Save **saves);

// Fonction qui retourne le meilleur coup pour l'IA de niveau moyen
int BestChoiceMedium(int line, int col, char **grid);

// Supprime le dernier coup joué dans une colonne donnée
char **dismissShot(char **grid, int line,  int coup);


#endif // GAMEPLAY_H_INCLUDED
