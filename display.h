#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED
#include "utils.h"
const

//Creation de la grille de jeu
char **createGrid(int line, int col);
// Fonction d'affichage de la matrice de jeu jeu
void showGrid(char **grid,int line, int col);

//fonction de liberation de la memoire
void freeGrid(char **grid, int line);

//fonction de calcul de position
char **wherePosition(char **grid, int line, int coup);


#endif // DISPLAY_H_INCLUDED
