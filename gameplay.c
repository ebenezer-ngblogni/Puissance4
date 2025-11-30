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
        Si un joueur a gagne la partie (sinon si la personne n'a pas gagne),
        on le declare vainqueur (sinon "Match NULL") et retour au menu */

        showGrid(wherePosition(grid, line, coup, isPlayer1), line, col);
        if (winPosition(grid, line, col, isPlayer1 ? 'X' : 'O'))
        {
            printf("\nLe joueur %s a gagné !!!!\n", isPlayer1 ? p.pseudo : "Adv");
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

void playerVsIa(Profil p, NIVEAU lvl)
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
                printf("\nVous avez gagné !!!!\n");
                utils_pause_to_continue();
                flush_stdin_buffer();
                break;
            }
            else if (drawGame(grid, col))
            {
                printf("\n Match NULL, Aucun gagnant !!! \n");
                utils_pause_to_continue();
                flush_stdin_buffer();
                break;
            }

            isPlayer = isPlayer ? 0 : 1;
        }
        else
        {
            switch (lvl)
            {
            case FACILE:
                IAEasy(p, grid);
                break;

            case MOYEN:
                IAMedium(p, grid);
                break;

            case DIFFICILE:
                //IAHard(p, grid);
                break;

            default:
                break;
            }

            if (winPosition(grid, line, col, 'O'))
            {
                printf("\nVous avez perdu !\n");
                utils_pause_to_continue();
                flush_stdin_buffer();
                break;
            }
            else if (drawGame(grid, col))
            {
                printf("\n Match NULL, Aucun gagnant !!! \n");
                utils_pause_to_continue();
                flush_stdin_buffer();
                break;
            }
            isPlayer = isPlayer ? 0 : 1;
        }
    }

    freeGrid(grid, line);
}

void IAEasy(Profil p, char **grid)
{
    int line = p.grille_lignes, col = p.grille_cols, coup;
    struct timespec t = {1, 500}; // 1 sec et 500 nanosecondes
    

    // Choix du coup
    srand(time(NULL));

    do
    {
        coup = rand() % col + 1;
    } while (grid[0][coup - 1] != ' ');

    nanosleep(&t, NULL);

    if (IS_WIN)
    {
        system("cls");
    }
    else
    {
        system("clear");
    }

    showGrid(wherePosition(grid, line, coup, 0), line, col);

}

void IAMedium(Profil p, char **grid)
{
    int line = p.grille_lignes, col = p.grille_cols, coup;
    struct timespec t = {1, 500}; // 1 sec et 500 nanosecondes

    //wherePosition(grid, line, coup, isPlayer1)

    // Choix du coup
    do
    {
        coup = BestChoiceMedium(line, col, grid);
    } while (grid[0][coup - 1] != ' ');
    nanosleep(&t, NULL);

    if (IS_WIN)
    {
        system("cls");
    }
    else
    {
        system("clear");
    }

    showGrid(wherePosition(grid, line, coup, 0), line, col);

}

int BestChoiceMedium(int line, int col, char** grid){
    int i,j = 0;
    srand(time(NULL));


    char **gridTemp = createGrid(line, col);

    for (i = 0; i < line; i++){
        for (j = 0; j < col; j++){
            gridTemp[i][j] = grid[i][j];
        }
    }

    //on trouve le meilleur coup
    for (i = 1; i <= col; i++){
        if((winPosition(wherePosition(gridTemp, line, i, 0), line, col, 'O'))){
            freeGrid(gridTemp, line);
            return i;
        }else{
            dismissShot(gridTemp, line, i);
        }
    }

    for (i = 1; i <= col; i++){
        if((winPosition(wherePosition(gridTemp, line, i, 1), line, col, 'X'))){
            freeGrid(gridTemp, line);
            return i;
        }else{
            dismissShot(gridTemp, line, i);
        }
    }

    freeGrid(gridTemp, line);

    do
    {
        i = rand() % col + 1;
    } while (grid[0][i - 1] != ' ');

    return i;
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

char **dismissShot(char **grid, int line,  int coup){
    
    for(int i = 0; i < line; i++){
        if (grid[i][coup-1] != ' '){
            grid[i][coup-1] = ' ';
           break;
        }
    }
    return grid;
}
