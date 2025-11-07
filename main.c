#include "utils.h"
#include "menu.c"
#include "display.c"
#include "gameplay.c"


int main()
{

    /*int cond = 1;
    while(cond = menuPrincipal()){

    }*/
    /*int line, col, coup, rate=1;
    printf("Entrez le nombre de lignes : ");
    scanf("%d", &line);
    printf("Entrez le nombre de colonnes : ");
    scanf("%d", &col);

    char **grid = createGrid(line, col);
    showGrid(grid, line, col);


    printf("\n ou souhaitez-vous? \n");
    scanf("%d", &coup);

    showGrid(wherePosition(grid,line,coup, 0),line,col);

    freeGrid(grid,line);*/
    twoPlayer();

    return 0;
}
