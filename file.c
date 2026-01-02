#include "file.h"
#include "utils.h"



// Ajout du coup à la liste chaînée des sauvegardes
void getCoup(int isPlayer, int coup, Save **saves){
    Save *new_save = malloc(sizeof(struct save));
    new_save->player = isPlayer? 1 : 0;
    if(coup != -1){
        new_save->coup = coup;

    }
    new_save->next = NULL;
    if(*saves == NULL) {
        *saves = new_save;
    }
    else{
        // Ajout à la fin de la liste
        Save *cur = *saves;
        while (cur->next != NULL) {
            cur = cur->next;
        }
        cur->next = new_save;
    }
    // n += 1;
    // return n;
}

// Enregistrement de la liste des sauvegardes dans le fichier de configuration associe au joueur
//
// Ajout de la date et de l'heure de la partie, du pseudo de l'adversaire, et du temps total de la partie


void saveIntoFile(Save *saves, Profil p, char *pseudo_adv, long score_time, char *message_victoire) {
    char nom_fichier[256];
    sprintf(nom_fichier, "files/%s.config.txt", p.pseudo);

    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    FILE * f = fopen(nom_fichier, "a+");

    if (f == NULL) {
        printf("Erreur: Impossible d'ouvrir/creer le fichier de profil. Verifiez que le dossier 'files' existe.\n");
        return;
    }

    // Ecriture de l'entete de la sauvegarde avec la date et l'adversaire
    if(p.mode_jeu){
        fprintf(f, "Partie jouee contre %s le %02d/%02d/%04d à %02d:%02d:%02d\n",
            pseudo_adv,
            t->tm_mday,
            t->tm_mon + 1,
            t->tm_year + 1900,
            t->tm_hour,
            t->tm_min,
            t->tm_sec);
    }
    else{
        fprintf(f, "Partie jouee contre %s le %02d/%02d/%04d à %02d:%02d:%02d\n",
            pseudo_adv,
            t->tm_mday,
            t->tm_mon + 1,
            t->tm_year + 1900,
            t->tm_hour,
            t->tm_min,
            t->tm_sec);
    }

    // Ecriture des coups
    while (saves != NULL) {

        fprintf(f, " %d %d\n",
                saves->player,
                saves->coup);
        saves = saves->next;
    }

    // Ecriture du message de victoire et du temps total de la partie
    fprintf(f, "%d %ld\n", 2, score_time);
    fprintf(f, "%s\n", message_victoire);

    fclose(f);
}

// Affichage coup par coup d'une partie sauvegardée à partir d'un tableau temporaire
// Apres affichage, liberation de la grille
void newGrid(int table[][2],char *message, int nbre_coups,int line, int col, Profil p, char *message_victoire){
    char **grid = createGrid(line, col);
    int i, j= nbre_coups-1;
    utils_clear_screen();

    printf("\n%s\n", message);
    printf("\n %s = \033[1;34mX\033[0m \n", p.pseudo);
    for(i = 0; i < j; i++){
        wherePosition(grid, line, table[i][1], table[i][0]);
        showGrid(grid, line, col);
        printf("\n\n");
    }
    printf("\n%s\n", message_victoire);
    printf("\n Duree de la partie: %ld secondes\n", (long)table[nbre_coups-1][1]);
    freeGrid(grid, line);
}

//Charge une partie sauvegardée depuis le fichier de configuration associe au joueur et l'affiche coup par coup
// Ici on lit le fichier de configuration du profil, on affiche la liste des parties sauvegardées
// L'utilisateur choisit une partie à visualiser, puis on affiche la grille coup par coup
void loadGame(Profil p){
    char nom_fichier[256];
    char line[256];
    char message[256];
    char message_victoire[256];
    int nb_parties = 0;
    int choice, index_choice=0, index=0, table[100][2];
    int nb_coups = 0;


    sprintf(nom_fichier, "files/%s.config.txt", p.pseudo);

    FILE * f = fopen(nom_fichier, "r");

    if (f == NULL) {
        return;
    }

    // Lecture du fichier ligne par ligne en recherchant et en affichant l'intitulees des parties sauvegardées
    // on reconnait une partie sauvegardée par la ligne "Partie ..."
    while (fgets(line, sizeof(line), f)) {

        if(strncmp(line, "Partie", 6) == 0) {
            nb_parties++;
            printf("%d- %s", nb_parties, line);
        }
    }

    if(nb_parties == 0){
        printf("Aucune partie encore jouee \n");
        fclose(f);
        utils_pause_to_continue();
        return;
    }
    int valid;
    do{
        printf("\nEntrez le numéro de la partie à visualiser : ");
        choice = utils_get_int();

        if(choice < 1 || choice > nb_parties){
            printf("\n Choix invalide. Veuillez reessayer.\n");
            valid = 0;
        }else{
            valid = 1;
        }
    }while(!valid);

    // Relecture du fichier pour extraire les coups de la partie choisie
    // On relit le fichier depuis le debut, mais cette fois on s'interesse uniquement aux lignes avec les coups de la partie choisie
    // Puis on compare l'index des parties pour savoir si on est dans la partie choisie
    // Si oui, on enregistre les coups dans un tableau a double entree temporaire
    // Si non, on passe a la ligne suivante

    // repositionne le curseur au début du fichier
    rewind(f);  
    while(fgets(line, sizeof(line), f)) {
        if(strncmp(line, "Partie", 6) == 0) {
            index++;
            index_choice= (index == choice) ? 1 : 0;
            if(index == choice){
                strcpy(message, line);
            }
            continue;
        }

        if(index_choice){
            int joueur, coup, score;
            long score_time;

            // Si c'est une ligne de coups
            if (sscanf(line, "%d %d", &joueur, &coup) == 2) {
                table[nb_coups][0] = joueur;
                table[nb_coups][1] = coup;
                nb_coups++;
            }
            else if (sscanf(line, "%d %ld", &score, &score_time) == 2) {
                table[nb_coups][0] = score;
                table[nb_coups][1] = score_time;
                nb_coups++;
            
            }else {
                strcpy(message_victoire, line);
                // condition d'arrêt : nouvelle partie ou fin de fichier
                break;
            }
        }
    }
    fclose(f);

    newGrid(table, message, nb_coups, p.grille_lignes, p.grille_cols,p , message_victoire);
    utils_pause_to_continue();
    return;
}