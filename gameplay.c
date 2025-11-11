#include <unistd.h>
#include <string.h>
#include "gameplay.h"
#include "gameplay.h"

void twoPlayer()
{

    int line, col, coup = -43, rate = 1, isPlayer1 = 1, continu = 1;
    printf("Entrez le nombre de lignes : ");
    scanf("%d", &line);
    printf("Entrez le nombre de colonnes : ");
    scanf("%d", &col);

    char **grid = createGrid(line, col);
    showGrid(grid, line, col);

    while (continu)
    {
        do
        {
            if ((coup < 1 || coup > col) && coup != -43)
            {
                printf("Coup invalide");
            }

            printf("\nJoueur %d entrez votre colonne : ", isPlayer1 ? 1 : 2);
            scanf("%d", &coup);
        } while (coup < 1 || coup > col);

        if (IS_WIN){
            system("cls");
        }else{
            system("clear");
        }

        showGrid(wherePosition(grid, line, coup, isPlayer1), line, col);
        if (winPosition(grid, line, col, isPlayer1 ? 'X' : 'O'))
        {
            printf("\nLe joueur %d a gagné !!!!\n", isPlayer1 = isPlayer1 ? 1 : 2);
            break;
        }
        isPlayer1 = isPlayer1 ? 0 : 1;
    }

    freeGrid(grid, line);
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
