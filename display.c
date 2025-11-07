#include "display.h"

void showPlayer1piece() {
    printf("\033[1;34m");
    printf("\u2B24");
    printf("\033[0m");
}

void showPlayer2piece() {
    printf("\033[0;31m");
    printf("\u2B24");
    printf("\033[0m");
}

// Creation de la grille de jeu avec allocation dynamique
char **createGrid(int line, int col){
    char **grid = malloc(line * sizeof(char*));
    if(grid == NULL){
        printf("Erreur lors de l'allocation de la ligne\n");
    }
    for(int i = 0; i < line ; i++){
        grid[i] = malloc(col * sizeof(char*));
        if(grid[i] == NULL){
            printf("Erreur lors de l'allocation de la colonne\n");
        }
    }

    for (int i = 0; i < line; i++){
        for(int j = 0; j < col; j++){
            grid[i][j] = ' '; // Initialisation des cases vides avec un espace
        }
    }
    return grid;
}


// Fonction d'affichage de la matrice de jeu
void showGrid(char **grid, int line, int col){
    int i,j,k;

    printf("\033[1;34m--- PUISSANCE 4 ---\033[0m\n");
    for(i = 1; i <= col; i++){
        printf("  %d ", i);
    }
    printf("\n");
    for(i = 0; i <= line; i++ ){
        if(i != 0){
            printf("+---+");
            for(k =0; k< col-1; k++){
                printf("---+");
            }
        }

        if(i < line){
             printf("\n| ");
            for(j =0; j< col; j++){
                //grid[i][j] = ' ';
                printf("%c", grid[i][j]);
                printf(" | ");
                //printf("---");
            }
            printf("\n");
        }
    }
    printf("\n");
    for(i = 1; i <= col; i++){
        printf("  %d ", i);
    }
}

// Fonction de liberation de la memoire
void freeGrid(char **grid, int line){
    for(int i = 0; i <line; i++)
        free(grid[i]);
    free(grid);
}


// Fonction de calcul de position
char **wherePosition(char **grid, int line,  int coup){
    if(grid[0][coup-1] != ' '){
        printf("Cette colonne est pleine. Jouez ailleurs!");
        return grid;
    }
    for(int i = line-1; i>= 0; i--){
        if (grid[i][coup-1] == ' '){
            grid[i][coup-1] = 'X';
           break;
        }
    }
    return grid;
}

int main(){
  int line, col, coup, rate=1;
  printf("Entrez le nombre de lignes : ");
  scanf("%d", &line);
  printf("Entrez le nombre de colonnes : ");
  scanf("%d", &col);

  char **grid = createGrid(line, col);
  showGrid(grid, line, col);


  printf("\n ou souhaitez-vous? \n");
  scanf("%d", &coup);

  showGrid(wherePosition(grid,line,coup),line,col);



  freeGrid(grid,line);
  return 0;
}
