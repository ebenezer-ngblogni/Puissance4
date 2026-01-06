#include "gameplay.h"
#include "display.h"
#include "profil.h"
#include "utils.h"
#include "file.c"

/*Petite pause pour permettre au joueur de voir les messages
 avant le rafraichissement de l'ecran
*/
void pauseToDisplay(){
    fflush(stdout);
    #ifdef _WIN32
        Sleep(500);
    #else
        usleep(500);
    #endif
}


/* Fonction Booleenne permettant de lire la valeur entree au clavier et
la mettre dans la variable "coup" uniquement dans un delai bref defini par la variable p.temps_par_coup
*/
int waitToPlay(int *coup, int delay){
    #ifdef _WIN32
        time_t debut = time(NULL);
        while(time(NULL)-debut < delay){
            if(_kbhit()){
                scanf("%d", coup);
                return 1;
            }
            Sleep(50);
        }
        return 0;


    #else
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
        usleep(50);
        return 0;
    #endif
}


/*
Fonctionnalité du mode Joueur contre Joueur. La fonction prend en parametre le profil du joueur courant
Avant le debut de la partie, le joueur entre son adversaire.
A chaque tour, un joueur a un temps limité pour jouer son coup.
A la fin de la partie, les informations de la partie sont sauvegardées dans le fichier de configuration du profil.
Et la grille est liberée de la mémoire.
*/

void twoPlayer(Profil p)
{
      int line = p.grille_lignes, col = p.grille_cols;
      char pseudo_adv[50];

      printf("\n %s bonne partie :-)\n", p.pseudo);
      printf("\n Entrer le pseudo de votre adversaire:\n");
      utils_get_secure_string(pseudo_adv, 50);

      char **grid = createGrid(line, col);
      showGrid(grid, line, col, p.forme_pions);
      printf("\n\n");

      Save *saves = NULL;
      twoPlayerCore(grid, line, col, p, pseudo_adv, saves, 1, 0);
}

void twoPlayerCore(char **grid, int line, int col, Profil p, char *pseudo_adv, Save *saves, int joueur_actuel, long temps_deja_ecoule)
{

    int coup, valid = 0, isPlayer1 = joueur_actuel, continu = 1;
    char message_victoire[256];
    p.mode_jeu = 1;


    if (temps_deja_ecoule > 0) {
      printf("\n=== REPRISE DE LA PARTIE ===\n");
      printf("%s VS %s\n", p.pseudo, pseudo_adv);
      printf("C'est au tour de: %s\n\n", isPlayer1 ? p.pseudo : pseudo_adv);
      showGrid(grid, line, col, p.forme_pions);
      printf("\n\n");
      utils_pause_to_continue();
    }

    time_t start_game = time(NULL) - temps_deja_ecoule;

    while (continu)
    {
        do
        {
            time_t start_time = time(NULL);
            coup = -1;

            /*cette boucle affiche en continu le message avec un chrono decroissant
              l'utilisation du '\r' dans notre printf permet ainsi d'effacer la ligne precedente
              et de reecrire sur cette meme ligne
              le 'fflush(stdout)' Force l'affichage immédiat du message du "printf" */

            while (1){
                time_t now = time(NULL);
                long past_time = now - start_time;
                long temps_par_coup = (long)p.temps_par_coup;
                // Si le joueur ne joue pas dans le temps imparti, son tour est passe
                if (past_time > p.temps_par_coup) {
                    isPlayer1 = isPlayer1 ? 1 : 0;
                    break;
                }

                printf("\r %s entrez votre colonne (ou '0' pour pauser) (%ld sec: %ld): ",
                        isPlayer1 ? p.pseudo : pseudo_adv,
                        temps_par_coup, temps_par_coup - past_time);
                fflush(stdout);

                if (waitToPlay(&coup, 1)) {
                    // Vérifier si l'utilisateur veut mettre en pause
                    if (coup == 0) {
                        printf("\n\n=== MISE EN PAUSE ===\n");
                        long temps_ecoule = time(NULL) - start_game;
                        savePausedGame(grid, line, col, p, pseudo_adv, 0,
                                    temps_ecoule, saves, isPlayer1);
                        printf("Partie sauvegardee ! Vous pourrez la reprendre via le menu (option 4).\n");
                        utils_pause_to_continue();
                        freeGrid(grid, line);
                        return;  // Quitter la fonction
                    }
                    break;
                }
            }


            // Validation du coup
            if ((coup < 1 || coup > col) && coup != -1){
                printf("\n Coup invalide\n");
                pauseToDisplay();
                isPlayer1 = isPlayer1 ? 1 : 0;
                valid = 0;

            }
            // Si la colonne est pleine
            else if ((grid[0][coup - 1] != ' ') && coup != -1){
                printf("\n Cette colonne est pleine. Jouez ailleurs!\n");
                pauseToDisplay();
                isPlayer1 = isPlayer1 ? 1 : 0;
                valid = 0;
            } else{
                valid = 1;
                //Enregistrer le coup dans la liste des sauvegardes
                getCoup(isPlayer1, coup, &saves);
            }
        } while (!valid);



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

        showGrid(wherePosition(grid, line, coup, isPlayer1), line, col, p.forme_pions);

        printf("\n\n");

        if (winPosition(grid, line, col, isPlayer1 ? 'X' : 'O'))
        {
            printf("\nLe joueur %s a gagné !!!!\n",  isPlayer1 ? p.pseudo : pseudo_adv);
            sprintf(message_victoire, "Le joueur %s a gagné !!!!",  isPlayer1 ? p.pseudo : pseudo_adv);
            flush_stdin_buffer();
            utils_pause_to_continue();
            break;
        }
        else if (drawGame(grid, col))
        {
            printf("\n Match NULL, Aucun gagnant !!! \n");
            sprintf(message_victoire, " Match NULL, Aucun gagnant !!! ");
            flush_stdin_buffer();
            utils_pause_to_continue();
            break;
        }

        isPlayer1 = isPlayer1 ? 0 : 1;
        valid = 0;
    }

    time_t end_game = time(NULL);
    long score_time = end_game - start_game;
    // Sauvegarde l'ensemble des coups joues dans le fichier de configuration du profil
    saveIntoFile(saves, p, pseudo_adv, score_time, message_victoire);
    freeGrid(grid, line);
}

/*Fonctionnalité du mode Joueur contre IA. La fonction prend en parametre le profil du joueur courant
et le niveau de l'IA (facile, moyen, difficile).

*/
void playerVsIa(Profil p, NIVEAU lvl)
{
      int line = p.grille_lignes, col = p.grille_cols;

      printf("\n %s bonne partie :-)\n", p.pseudo);

      char **grid = createGrid(line, col);
      showGrid(grid, line, col, p.forme_pions);
      printf("\n\n");

      Save *saves = NULL;
      playerVsIaCore(grid, line, col, p, lvl, saves, 1, 0);
}

void playerVsIaCore(char **grid, int line, int col, Profil p, NIVEAU lvl, Save *saves, int joueur_actuel, long temps_deja_ecoule)
{

    int coup, isPlayer1 = joueur_actuel, continu = 1, valid = 0;
    char message_victoire[256];
    p.mode_jeu = 0;

    time_t start_game = time(NULL) - temps_deja_ecoule;
    char pseudo_adv[50];
    if (lvl == FACILE) {
        strcpy(pseudo_adv, "IA - FACILE");
    } else {
        strcpy(pseudo_adv, "IA - MOYEN");
    }

    if (temps_deja_ecoule > 0) {
      printf("\n=== REPRISE DE LA PARTIE ===\n");
      printf("%s VS %s\n", p.pseudo, pseudo_adv);
      printf("C'est au tour de: %s\n\n", isPlayer1 ? p.pseudo : pseudo_adv);
      showGrid(grid, line, col, p.forme_pions);
      printf("\n\n");
      utils_pause_to_continue();
    }


    while (continu)
    {
        if (isPlayer1)
          {

              do
              {
                  printf("\n %s entrez votre colonne (ou '0' pour pauser) : ", p.pseudo);

                  char input[20];
                  fgets(input, sizeof(input), stdin);

                  // Vérifier si pause demandée
                  if (input[0] == '0' || input[0] == 'P' || input[0] == 'p') {
                      printf("\n\n=== MISE EN PAUSE ===\n");
                      long temps_ecoule = time(NULL) - start_game;
                      savePausedGame(grid, line, col, p, pseudo_adv, lvl,
                                    temps_ecoule, saves, isPlayer1);
                      printf("Partie sauvegardee ! Vous pourrez la reprendre via le menu (option 4).\n");
                      utils_pause_to_continue();
                      freeGrid(grid, line);
                      return;
                  }

                  coup = atoi(input);

                  if (coup < 1 || coup > col)
                  {
                      printf("\n Coup invalide\n");
                      valid = 0;
                  }
                  else if (grid[0][coup - 1] != ' ')
                  {
                      printf("\n Cette colonne est pleine. Jouez ailleurs!\n");
                      pauseToDisplay();
                      valid = 0;
                  }else{
                      valid = 1;
                      // Enregistrer le coup dans la liste des sauvegardes
                      getCoup(isPlayer1, coup, &saves);
                  }
              } while (!valid);

            if (IS_WIN)
            {
                system("cls");
            }
            else
            {
                system("clear");
            }

            showGrid(wherePosition(grid, line, coup, isPlayer1), line, col, p.forme_pions);

            if (winPosition(grid, line, col, isPlayer1 ? 'X' : 'O'))
            {
                printf("\nVous avez gagné !!!!\n");
                sprintf(message_victoire, "Vous avez gagné !!!!");

                utils_pause_to_continue();
                flush_stdin_buffer();
                break;
            }
            else if (drawGame(grid, col))
            {
                printf("\n Match NULL, Aucun gagnant !!! \n");
                sprintf(message_victoire, " Match NULL, Aucun gagnant !!! ");
                utils_pause_to_continue();
                flush_stdin_buffer();
                break;
            }

            isPlayer1 = isPlayer1 ? 0 : 1;
        }
        else
        {
            switch (lvl)
            {
            case FACILE:
                IAEasy(p, grid, &saves);
                break;

            case MOYEN:
                IAMedium(p, grid, &saves);
                break;

            default:
                break;
            }

            if (winPosition(grid, line, col, 'O'))
            {
                printf("\nVous avez perdu !\n");
                sprintf(message_victoire, "Vous avez perdu !");
                utils_pause_to_continue();
                flush_stdin_buffer();
                break;
            }
            else if (drawGame(grid, col))
            {
                printf("\n Match NULL, Aucun gagnant !!! \n");
                sprintf(message_victoire, " Match NULL, Aucun gagnant !!! ");
                utils_pause_to_continue();
                flush_stdin_buffer();
                break;
            }
            isPlayer1 = isPlayer1 ? 0 : 1;
        }
        valid = 0;
    }

    time_t end_game = time(NULL);
    long score_time = end_game - start_game;
    // Sauvegarde l'ensemble des coups joues dans le fichier de configuration du profil
    saveIntoFile(saves, p, pseudo_adv, score_time, message_victoire);
    freeGrid(grid, line);
}

// Fonctionnalité de l'IA de niveau facile
void IAEasy(Profil p, char **grid, Save **saves)
{
    int line = p.grille_lignes, col = p.grille_cols, coup;
    int isPlayer1 = 0;
    struct timespec t = {1, 500}; // 1 sec et 500 nanosecondes


    // Choix du coup
    // Initialisation du générateur de nombres aléatoires
    srand(time(NULL));

    do
    {
        coup = rand() % col + 1;
    } while (grid[0][coup - 1] != ' ');
    getCoup(isPlayer1, coup, saves);
    nanosleep(&t, NULL);

    if (IS_WIN)
    {
        system("cls");
    }
    else
    {
        system("clear");
    }

    showGrid(wherePosition(grid, line, coup, 0), line, col, p.forme_pions);

}

// Fonctionnalité de l'IA de niveau moyen
void IAMedium(Profil p, char **grid, Save **saves)
{
    int line = p.grille_lignes, col = p.grille_cols, coup;
    struct timespec t = {1, 500}; // 1 sec et 500 nanosecondes

    //wherePosition(grid, line, coup, isPlayer1)

    // Choix du coup
    do
    {
        coup = BestChoiceMedium(line, col, grid);
    } while (grid[0][coup - 1] != ' ');

    getCoup(0, coup, saves);

    nanosleep(&t, NULL);

    if (IS_WIN)
    {
        system("cls");
    }
    else
    {
        system("clear");
    }

    showGrid(wherePosition(grid, line, coup, 0), line, col, p.forme_pions);

}

// Fonction qui retourne le meilleur coup pour l'IA de niveau moyen
int BestChoiceMedium(int line, int col, char** grid){
    int i,j = 0;
    // Initialisation du générateur de nombres aléatoires pour le choix aléatoire du coup de l'IA
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

// Vérifie si la grille est pleine (match nul)
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

// Vérifie si un joueur a gagné la partie
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

