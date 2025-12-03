#ifndef GAMEPLAY_H_INCLUDED
#define GAMEPLAY_H_INCLUDED
#include "utils.h"
#include "profil.h"

int waitToPlay(int *coup, int delay);
void pause_to_display();
typedef struct size
{
    int width;
    int height;
} size;

//void newGame();
struct game
{
    size sizes;
    int grille [5][6];
    // int grille [sizes.height][sizes.width]
};

//long timer();
void twoPlayer(Profil p);


int winPosition(char **grid ,int line, int col, char symbole);

int drawGame(char **grid , int col);

void playerVsIa(Profil p, NIVEAU lvl);

void IAEasy(Profil p, char **grid);

void IAMedium(Profil p, char **grid);

void IAHard(Profil p, char **grid);

int BestChoiceMedium(int line, int col, char **grid);

char **dismissShot(char **grid, int line,  int coup);


#endif // GAMEPLAY_H_INCLUDED
