#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED
#include "utils.h"


//Creation de la grille de jeu
char **createGrid(int line, int col);
// Fonction d'affichage de la matrice de jeu jeu
void showGrid(char **grid,int line, int col);

//fonction de liberation de la memoire
void freeGrid(char **grid, int line);

//fonction de calcul de position d'un coup dans la grille
char **wherePosition(char **grid, int line, int coup, int isPlayer1);

void rulesGame();


#endif // DISPLAY_H_INCLUDED
