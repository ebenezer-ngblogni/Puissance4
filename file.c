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

/* Enregistrement de la liste des sauvegardes dans le fichier 
  de configuration associe au joueur.
  Ajout de la date et de l'heure de la partie, du pseudo de l'adversaire, 
  et du temps total de la partie
*/
void saveIntoFile(Save *saves, Profil p, char *pseudo_adv, long score_time, char *message_victoire) {
    char nom_fichier[256];
    sprintf(nom_fichier, "files/%s.config.txt", p.pseudo);

    // Obtention de la date et de l'heure actuelles
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    FILE * f = fopen(nom_fichier, "a+");

    // Vérification de l'ouverture du fichier
    if (f == NULL) {
        printf("Erreur: Impossible d'ouvrir/creer le fichier de profil. Verifiez que le dossier 'files' existe.\n");
        return;
    }

    // Ecriture de l'entete de la sauvegarde avec la date et l'adversaire
    fprintf(f, "Partie jouee contre %s le %02d/%02d/%04d à %02d:%02d:%02d\n",
        pseudo_adv,
        t->tm_mday,
        t->tm_mon + 1,
        t->tm_year + 1900,
        t->tm_hour,
        t->tm_min,
        t->tm_sec);
    

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
        showGrid(grid, line, col, p.forme_pions);
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
    char message_partie[256];
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
                strcpy(message_partie, line);
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

    newGrid(table, message_partie, nb_coups, p.grille_lignes, p.grille_cols,p , message_victoire);
    utils_pause_to_continue();
    return;
}

// Sauvegarder une partie en pause dans <pseudo>.pause.txt
int savePausedGame(char **grid, int ligne, int col, Profil p,
                    char *pseudo_adv, int niveau_ia, long temps_ecoule,
                    Save *saves, int joueur_actuel) {
    char nom_fichier[256];
    sprintf(nom_fichier, "files/%s.pause.txt", p.pseudo);

    // Vérifier si une partie en pause existe déjà
    if (file_exists(nom_fichier)) {
        printf("\n\033[1;33m⚠ ATTENTION ⚠\033[0m\n");
        printf("Vous avez deja une partie en pause.\n");
        printf("Si vous continuez, l'ancienne partie en pause sera \033[1;31mECRASEE\033[0m.\n\n");
        printf("Voulez-vous vraiment ecraser l'ancienne partie ? (O/N) : ");

        char reponse[10];
        utils_get_secure_string(reponse, 10);

        // Si l'utilisateur refuse
        if (reponse[0] != 'O' && reponse[0] != 'o') {
            printf("\n-> Mise en pause annulee. La partie continue.\n");
            utils_pause_to_continue();
            return 0;  // Sauvegarde annulée
        }

        printf("\n-> Ancienne partie en pause ecrasee.\n");
    }

    FILE *f = fopen(nom_fichier, "w");
    if (f == NULL) {
        printf("Erreur: Impossible de sauvegarder la partie en pause.\n");
        return 0;  // Échec de sauvegarde
    }

    // Marqueur de partie en pause
    fprintf(f, "PAUSE_GAME\n");

    // Profil du joueur
    fprintf(f, "%s %d %d %f %d %d\n",
            p.pseudo, ligne, col, p.temps_par_coup, p.forme_pions, p.mode_jeu);

    // Infos de la partie : adversaire, temps écoulé, joueur actuel
    if (p.mode_jeu == 1) {
        fprintf(f, "%s %ld %d\n", pseudo_adv, temps_ecoule, joueur_actuel);
    } else {
        // Mode IA : stocker niveau
        char niveau_str[20];
        sprintf(niveau_str, "IA-%s", niveau_ia == 1 ? "FACILE" : "MOYEN");
        fprintf(f, "%s %ld %d\n", niveau_str, temps_ecoule, joueur_actuel);
    }

    // On sérialise et on écrit la grille
    char *grille_serialisee = serialize_grid(grid, ligne, col);
    if (grille_serialisee) {
        fprintf(f, "%s\n", grille_serialisee);
        free(grille_serialisee);
    }

    // Écriture de  tous les coups
    Save *current = saves;
    while (current != NULL) {
        fprintf(f, "%d %d\n", current->player, current->coup);
        current = current->next;
    }

    // Marqueur de fin
    fprintf(f, "END_PAUSE\n");

    fclose(f);
    return 1;  // Sauvegarde réussie
}

// Chargement de l'état d'une partie en pause
// 
PausedGame* loadPausedGameState(Profil p) {
    char nom_fichier[256];
    sprintf(nom_fichier, "files/%s.pause.txt", p.pseudo);

    if (!file_exists(nom_fichier)) {
        return NULL;
    }

    FILE *f = fopen(nom_fichier, "r");
    if (f == NULL) return NULL;

    PausedGame *pg = malloc(sizeof(PausedGame));
    if (!pg) {
        fclose(f);
        return NULL;
    }

    char line[512]; 

    // Lecture du marqueur PAUSE_GAME
    if (!fgets(line, sizeof(line), f) || strncmp(line, "PAUSE_GAME", 10) != 0) {
        fclose(f);
        free(pg);
        return NULL;
    }

    // Lecture du profil avec fgets au lieu de fscanf
    if (fgets(line, sizeof(line), f)) {
        sscanf(line, "%s %d %d %f %d %d",
                pg->pseudo, &pg->grille_lignes, &pg->grille_cols,
                &pg->temps_par_coup, &pg->forme_pions, &pg->mode_jeu);
    }

    // Lecture de adversaire/niveau, temps écoulé, joueur actuel
    if (fgets(line, sizeof(line), f)) {
        sscanf(line, "%s %ld %d", pg->adversaire, &pg->temps_ecoule, &pg->joueur_actuel);
    }

    // Déterminer le niveau IA si mode PvIA
    if (pg->mode_jeu == 0) {
        if (strstr(pg->adversaire, "FACILE")) {
            pg->niveau_ia = 1;
        } else if (strstr(pg->adversaire, "MOYEN")) {
            pg->niveau_ia = 2;
        }
    } else {
        pg->niveau_ia = 0;
    }

    // Lecture de la grille sérialisée
    if (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\n")] = '\0';  // Supprimer le \n
        pg->grille_serialisee = malloc(strlen(line) + 1);
        if (pg->grille_serialisee) {
            strcpy(pg->grille_serialisee, line);
        }
    }

    // Lecture des les coups et reconstruire la liste chaînée
    pg->saves = NULL;
    int player, coup;
    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "END_PAUSE", 9) == 0) break;

        if (sscanf(line, "%d %d", &player, &coup) == 2) {
            getCoup(player, coup, &pg->saves);
        }
    }

    fclose(f);
    return pg;
}

// Libérer la mémoire d'une PausedGame
void freePausedGame(PausedGame *pg) {
    if (!pg) return;

    if (pg->grille_serialisee) {
        free(pg->grille_serialisee);
    }

    // Libérer la liste chaînée des coups
    Save *current = pg->saves;
    while (current != NULL) {
        Save *temp = current;
        current = current->next;
        free(temp);
    }

    free(pg);
}

// Reprendre une partie en pause
int resumePausedGame(Profil p) {
    PausedGame *pg = loadPausedGameState(p);
    if (!pg) {
        return 0;  // Aucune partie en pause
    }

    printf("\n=== CHARGEMENT DE LA PARTIE EN PAUSE ===\n");
    printf("Adversaire: %s\n", pg->adversaire);
    printf("Temps ecoule: %ld secondes\n", pg->temps_ecoule);
    printf("Tour de: %s\n\n", pg->joueur_actuel == 1 ? p.pseudo : pg->adversaire);

    // Recréer la grille
    char **grid = createGrid(pg->grille_lignes, pg->grille_cols);
    deserialize_grid(grid, pg->grille_serialisee, pg->grille_lignes, pg->grille_cols);

    // Suppression du fichier .pause.txt AVANT de relancer (pour éviter double sauvegarde)
    char nom_fichier[256];
    sprintf(nom_fichier, "files/%s.pause.txt", p.pseudo);
    remove(nom_fichier);

    // Restauration des paramètres du profil depuis la sauvegarde
    p.grille_lignes = pg->grille_lignes;
    p.grille_cols = pg->grille_cols;
    p.temps_par_coup = pg->temps_par_coup;
    p.forme_pions = pg->forme_pions;
    p.mode_jeu = pg->mode_jeu;

    // On relance la partie selon le mode
    if (pg->mode_jeu == 1) {
        twoPlayerCore(grid, pg->grille_lignes, pg->grille_cols, p, pg->adversaire, pg->saves, pg->joueur_actuel, pg->temps_ecoule);
    } else {
        NIVEAU lvl = (pg->niveau_ia == 1) ? FACILE : MOYEN;
        playerVsIaCore(grid, pg->grille_lignes, pg->grille_cols, p, lvl, pg->saves, pg->joueur_actuel, pg->temps_ecoule);
    }


    freePausedGame(pg);

    return 1;
}

