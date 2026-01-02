#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED
#include "utils.h"
#include "profil.h"


struct save{
    int player;
    int coup;
    struct save *next;
};
typedef struct save Save;

void getCoup(int isPlayer, int coup, Save **saves);
extern  int nbre;

void saveIntoFile(Save *saves, Profil p, char *pseudo_adv, long score_time);
void newGrid(int table[][2], char *message, int nbre_coups, int line, int col, Profil p);
void loadGame(Profil p);
#endif // FILE_H_INCLUDED
