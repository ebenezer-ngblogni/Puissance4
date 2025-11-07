#include "gameplay.h"
#include <unistd.h>

void twoPlayer(){
    int line, col, coup, rate=1, isPlayer1 = 1;
    printf("Entrez le nombre de lignes : ");
    scanf("%d", &line);
    printf("Entrez le nombre de colonnes : ");
    scanf("%d", &col);

    char **grid = createGrid(line, col);
    showGrid(grid, line, col);

    while(!winPosition(grid)){
        printf("\nJoueur %d où souhaitez-vous? \n", isPlayer1? 1 : 2);
        scanf("%d", &coup);

        execv("clear");
        showGrid(wherePosition(grid,line,coup, isPlayer1),line,col);
        isPlayer1 = isPlayer1? 0 : 1;
    }


    freeGrid(grid,line);

}

int winPosition(char **grid ){
    return 0;
}

void showPlayer1piece();

void showPlayer2piece();
