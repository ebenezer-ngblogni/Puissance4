#include "gameplay.h"
#include "display.h"
#include "profil.h"
#include "utils.h"

void twoPlayer(Profil p)
{

    int line = p.grille_lignes, col = p.grille_cols;
    int coup, rate = 1, isPlayer1 = 1, continu = 1;

    /*printf("Entrez le nombre de lignes : ");
    scanf("%d", &line);
    printf("Entrez le nombre de colonnes : ");
    scanf("%d", &col);*/

    printf("\n %s bonne partie :-)\n", p.pseudo);

    char **grid = createGrid(line, col);
    showGrid(grid, line, col);

    while (continu)
    {
        do
        {

            printf("\n %s entrez votre colonne : ", isPlayer1 ? p.pseudo : "Adv");
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

        /* Si un joueur a gagne la partie (sinon si la personne n'a pas gagne),
        on le declare vainqueur (sinon "Match NULL") et retour au menu */
        showGrid(wherePosition(grid, line, coup, isPlayer1), line, col);
        if (winPosition(grid, line, col, isPlayer1 ? 'X' : 'O'))
        {
            printf("\nLe joueur %s a gagné !!!!\n", isPlayer1 ? p.pseudo : "Adv");
            break;
        }
        else if (drawGame(grid, col))
        {
            printf("\n Match NULL, Aucun gagnant !!! \n");
            break;
        }

        isPlayer1 = isPlayer1 ? 0 : 1;
    }

    freeGrid(grid, line);
}

void playerVsIa(Profil p, niveau lvl)
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
                printf("\nLe joueur %s a gagné !!!!\n", isPlayer ? p.pseudo : "Adv");
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
                /* code */
                break;

            case DIFFICILE :
                
                break;
            
            default:
                break;
            }
        }
    }

    freeGrid(grid, line);
}

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

void showPlayer1piece();

void showPlayer2piece();
