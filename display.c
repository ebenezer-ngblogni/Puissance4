#include "display.h"


// Creation de la grille de jeu avec allocation dynamique
char **createGrid(int line, int col){
    char **grid = malloc(line * sizeof(char*));
    if(grid == NULL){
        printf("Erreur lors de l'allocation de la ligne\n");
    }
    for(int i = 0; i < line ; i++){
        grid[i] = malloc(col *sizeof(char*));
        if(grid[i] == NULL){
            printf("Erreur lors de l'allocation de la colonne\n");
        }
    }

    for (int i = 0; i < line; i++){
        for(int j = 0; j < col; j++){
            // Initialisation des cases vides avec un espace
            grid[i][j] = ' '; 
        }
    }
    return grid;
}


// Fonction d'affichage de la matrice de jeu
void showGrid(char **grid, int line, int col, int forme_pions){
    int i,j,k;

    printf("\n \033[1;34mOXO--- PUISSANCE 4 ---OXO\033[0m \n\n");
    
    //Affichage des indices des colonnes au-dessus de la grille
    for(i = 1; i <= col; i++){
        printf("  %d ", i);
    }
    printf("\n");


    for(i = 0; i <= line; i++ ){
        //tracage de ligne de la grille
        if(i != 0){
            printf("+---+");
            for(k =0; k< col-1; k++){
                printf("---+");
            }
        }

        if(i < line){
             printf("\n| ");
            for(j =0; j< col; j++){

                if (grid[i][j] == 'X') {
                // Affichage joueur 1 selon le style choisi
                switch(forme_pions) {
                    case 1:  // Style classique (X/O)
                        printf("\033[1;34mX\033[0m");
                        break;
                    case 2:  // Style cœurs (♥/♡)
                        printf("\033[1;34m♥\033[0m");  
                        break;
                    case 3:  // Style étoiles (★/☆)
                        printf("\033[1;34m★\033[0m");  
                        break;
                    default:
                        printf("\033[1;34mX\033[0m");
                }
                } else if (grid[i][j] == 'O') {
                    // Affichage joueur 2 selon le style choisi
                    switch(forme_pions) {
                        case 1:  // Style classique (X/O)
                            printf("\033[1;31mO\033[0m");
                            break;
                        case 2:  // Style cœurs (♥/♡)
                            printf("\033[1;31m♥\033[0m");  
                            break;
                        case 3:  // Style étoiles (★/☆)
                            printf("\033[1;31m☆\033[0m");  
                            break;
                        default:
                            printf("\033[1;31mO\033[0m");
                    }
                } else {
                    printf("%c", grid[i][j]);
                }

                printf(" | ");
            }
            printf("\n");
        }
    }
    printf("\n");
    //Affichage des indices des colonnes en-dessous de la grille
    for(i = 1; i <= col; i++){
        printf("  %d ", i);
    }
}

// Fonction de liberation de la memoire allouee pour la grille
void freeGrid(char **grid, int line){
    for(int i = 0; i <line; i++)
        free(grid[i]);
    free(grid);
}


// Fonction de calcul de position d'un coup dans la grille
char **wherePosition(char **grid, int line,  int coup, int isPlayer1){
    
    for(int i = line-1; i>= 0; i--){
        if (grid[i][coup-1] == ' '){
            if(isPlayer1)
                grid[i][coup-1] = 'X';
            else 
                grid[i][coup-1] = 'O';
           break;
        }
    }
    return grid;
}

// Affichage des regles du jeu
void rulesGame(){
    utils_clear_screen();
    printf("===================================================================\n");
    printf("\t\033[1;34mOXO--- REGLES DU JEU ---OXO\033[0m\n");
    printf("===================================================================\n\n");
    printf("Bienvenue dans le jeu du Puissance 4 ! Voici les règles du jeu :\n\n");

    printf("\t\033[1;31mDESCRIPTION DU JEU :\033[0m\n");

    printf("Le Puissance 4 est un jeu de stratégie pour deux joueurs, chacun ayant un symbole distinct (\033[1;34mX\033[0m et \033[1;31mO\033[0m par defaut).\n");
    printf("1. Le jeu se joue sur une grille, avec pour taille minimale 6 lignes et 7 colonnes.\n");
    printf("2. Le jeu consiste à placer tour à tour un pion dans une colonne de la grille.\n");
    printf("3. Le but du jeu est d'aligner en premier 4 pions de sa couleur horizontalement, verticalement ou en diagonale.\n");
    printf("4. Si la grille est remplie sans qu'aucun joueur n'ait aligné 4 pions, la partie se termine par un match nul.\n");


    printf("\n\t\033[1;31mREGLES DU JEU :\033[0m\n");

    printf("1. Les joueurs jouent à tour de rôle, en commençant par le joueur courant(joueur connecté) utilisant le symbole \033[1;34mX\033[0m.\n");
    printf("2. Lors de son tour, un joueur choisit une colonne où il souhaite placer son pion.\n");
    printf("3. Le pion tombe dans la colonne choisie et occupe la case vide la plus basse disponible.\n");
    printf("4. Un joueur ne peut interrompre qu'une seule partie en cours à la fois. \n S'il tente d'en interrompre une nouvelle, il lui sera demandé soit d'annuler et continuer la partie, soit de valider et supprimer l'ancienne partie interrompue.\n");
   

    printf("\n\t\033[1;31mMODES DE JEU :\033[0m\n");

    printf("1. Mode 2 Joueurs : Deux joueurs humains s'affrontent en tour à tour:\n");
    printf("\t   - Le joueur courant saisit le pseudo du second joueur avant de commencer la partie.\n");
    printf("\t   - Les joueurs disposent chacun de \033[1;31m%d secondes\033[0m (temps par defaut) pour jouer leur coup.\n\n", TIMER_PLAY);

    printf("2. Mode Joueur vs IA : Un joueur humain affronte l'IA(joueur machine)de niveau Facile ou Moyen:\n");
    printf("\t   - Ici, le temps de jeu n'est pas limité pour le joueur humain.\n");
    printf("\t   - Niveau Facile : L'IA choisit un coup aléatoire parmi les colonnes disponibles.\n");
    printf("\t   - Niveau Moyen : L'IA analyse la grille pour bloquer les coups gagnants de l'adversaire et tente \n de créer ses propres opportunités de victoire.\n");

    printf("\n\t\033[1;31mOPTIONS SUPPLEMENTAIRES :\033[0m\n");
    printf("1. Un joueur peut interrompre la partie en cours à tout moment en appuyant sur '\033[1;31m0\033[0m' au clavier pour sauvegarder et quitter.\n");
    printf("2. Sauvegarde de Partie : À la fin de chaque partie, les informations de la partie(coups des joueurs et durée totale) \n sont sauvegardées dans le fichier de configuration du profil du joueur courant.\n");
    printf("3. Chargement de Partie : Les joueurs peuvent charger et reprendre des parties interrompues à partir de leur profil.\n\n");

    printf("4. Une fois connecté, on peut:\n");
    printf("\t   - Modifier ses paramètres de configurations(taille de la grille, temps par coup, forme des pions).\n");
    printf("\t   - Changer d'utilisateur courant sans quitter le jeu.\n");
    printf("\t   - Visualiser les parties sauvegardées coup par coup avec la durée totale de la partie\n");


    printf("\n !!!!! \033[1;34mAmusez-vous bien et que le meilleur gagne\033[0m !!!!!\n\n");
}
 