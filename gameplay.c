#include "gameplay.h"
#include "display.h"
#include "profil.h"
#include "utils.h"

#ifdef _WIN32
#include <conio.h>
/* Fonction Booleenne permettant de lire la valeur entree au clavier et 
la mettre dans la variable "coup" uniquement dans un delai bref defini par la constante "TIMER_PLAY"*/ 
int waitToPlay(int *coup, int delay){
    time_t debut = time(NULL);
    while (time(NULL) - debut < delay) {
        // verifie si une touche du clavier est appuyee, lie la valeur et la met dans "coup"
        if (_kbhit()) {
            scanf("%d", coup);
            return 1;
        }
        Sleep(5);
    }
    return 0;
}
#endif

#ifndef _WIN32
#include <sys/select.h>
#include <unistd.h>

int waitToPlay(int *coup, int delay) {
    fd_set ensemble;
    struct timeval temps;
    FD_ZERO(&ensemble);
    FD_SET(STDIN_FILENO, &ensemble);
    temps.tv_sec = delay;
    temps.tv_usec = 0;

    int ret = select(STDIN_FILENO + 1, &ensemble, NULL, NULL, &temps);

    if (ret > 0) {
        scanf("%d", coup);
        return 1;
    }
    Sleep(5);
    return 0;
}
#endif

void twoPlayer(Profil p)
{

    int line = p.grille_lignes, col = p.grille_cols;
    int coup=0, valide = 0, isPlayer1 = 1, continu = 1;
    long lenght = 0;
    char pseudo_adv[50];


    printf("\n %s bonne partie :-)\n", p.pseudo);
    printf("\n Entrer le pseudo de votre adversaire:\n");
    utils_get_secure_string(pseudo_adv, 50);

    char **grid = createGrid(line, col);
    showGrid(grid, line, col);

    while (continu)
    {
        do
        {
            printf("\n %s entrez votre colonne(vous avez %d secondes): ", isPlayer1 ? p.pseudo : pseudo_adv, TIMER_PLAY);
            fflush(stdout);
           coup=-1;
           // Si le joueur ne joue pas dans le temps imparti, son tour est passe
            if (!waitToPlay(&coup, TIMER_PLAY)) {
                printf("\nTemps écoulé ! Tour passé à l’adversaire.\n");
                isPlayer1 = isPlayer1 ? 0 : 1;

            }
            // Validation du coup
            if (coup < 1 || coup > col){
                printf("\n Coup invalide\n");
                isPlayer1 = isPlayer1 ? 0 : 1;
                
            }
            // Si la colonne est pleine
            else if (grid[0][coup - 1] != ' '){
                printf("\n Cette colonne est pleine. Jouez ailleurs!\n");
                isPlayer1 = isPlayer1 ? 0 : 1;
            } else{
                valide = 1;
            }
        } while (!valide);

        if (IS_WIN)
        {
            system("cls");
        }
        else
        {
            system("clear");
        }

        /* Si un joueur a gagne la partie (sinon si la personne n'a pas gagne),
        Si un joueur a gagne la partie (sinon si la personne n'a pas gagne),
        on le declare vainqueur (sinon "Match NULL") et retour au menu */

        showGrid(wherePosition(grid, line, coup, isPlayer1), line, col);
        if (winPosition(grid, line, col, isPlayer1 ? 'X' : 'O'))
        {
            printf("\nLe joueur %s a gagné !!!!\n",  isPlayer1 ? p.pseudo : pseudo_adv);
            flush_stdin_buffer();
            utils_pause_to_continue();
            break;
        }
        else if (drawGame(grid, col))
        {
            printf("\n Match NULL, Aucun gagnant !!! \n");
            flush_stdin_buffer();
            utils_pause_to_continue();
            break;
        }

        isPlayer1 = isPlayer1 ? 0 : 1;
    }
    freeGrid(grid, line);
}

/*void playerVsIa(Profil p, NIVEAU lvl)
{

    int line = p.grille_lignes, col = p.grille_cols;
    int coup, rate = 1, isPlayer = 1, continu = 1;

    printf("\n %s bonne partie :-)\n", p.pseudo);

    char **grid = createGrid(line, col);
    showGrid(grid, line, col);

    while (continu)
    {
        if (isPlayer)
        {

            do
            {
                printf("\n %s entrez votre colonne : ", p.pseudo);
                scanf("%d", &coup);
                if (coup < 1 || coup > col)
                {
                    printf("\n Coup invalide\n");
                }
                else if (grid[0][coup - 1] != ' ')
                {
                    printf("\n Cette colonne est pleine. Jouez ailleurs!\n");
                }
            } while (coup < 1 || coup > col || grid[0][coup - 1] != ' ');

            if (IS_WIN)
            {
                system("cls");
            }
            else
            {
                system("clear");
            }

            showGrid(wherePosition(grid, line, coup, isPlayer), line, col);

            if (winPosition(grid, line, col, isPlayer ? 'X' : 'O'))
            {
                printf("\nLe joueur %s a gagné !!!!\n", isPlayer ? p.pseudo : pseudo_adv);
                isPlayer ? printf("\nLe joueur %s a gagné !!!!\n", p.pseudo) : printf("\nVous avez perdu !\n");
                break;
            }
            else if (drawGame(grid, col))
            {
                printf("\n Match NULL, Aucun gagnant !!! \n");
                break;
            }

            isPlayer = isPlayer ? 0 : 1;
        }else{
            switch (lvl)
            {
            case FACILE :
                IAFacile();
                break;

            case MOYEN :

                break;

            case DIFFICILE :

                break;

            default:
                break;
            }
        }
    }

    freeGrid(grid, line);
}*/

int drawGame(char **grid, int col)
{
    int cp = 0;
    for (int j = 0; j < col; j++)
    {
        if (grid[0][j] != ' ')
            cp += 1;
    }

    if (cp == col)
        return 1;
    return 0;
}

int winPosition(char **grid, int line, int col, char symbole)
{

    for (int i = 0; i < line; i++)
    {
        for (int j = 0; j < col; j++)
        {

            if (grid[i][j] != symbole)
                continue;

            // Vérification horizontale
            if (j + 3 < col && grid[i][j + 1] == symbole && grid[i][j + 2] == symbole && grid[i][j + 3] == symbole)
                return 1;

            // Vérification verticale
            if (i + 3 < line && grid[i + 1][j] == symbole && grid[i + 2][j] == symbole && grid[i + 3][j] == symbole)
                return 1;

            // Diagonale descendante
            if (i + 3 < line && j + 3 < col && grid[i + 1][j + 1] == symbole && grid[i + 2][j + 2] == symbole && grid[i + 3][j + 3] == symbole)
                return 1;

            // Diagonale montante
            if (i - 3 >= 0 && j + 3 < col && grid[i - 1][j + 1] == symbole && grid[i - 2][j + 2] == symbole && grid[i - 3][j + 3] == symbole)
                return 1;
        }
    }
    return 0;
}
