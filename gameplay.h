#ifndef GAMEPLAY_H_INCLUDED
#define GAMEPLAY_H_INCLUDED
#include "utils.h"

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


void twoPlayer();


int winPosition(char **grid ,int line, int col, char symbole);



#endif // GAMEPLAY_H_INCLUDED
