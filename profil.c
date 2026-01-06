#include "profil.h"

void profile_initialize_directory(void) {
    #ifdef _WIN32
        // Syntaxe Windows
        system("mkdir files > NUL 2>&1");
    #else
        // Syntaxe Linux/macOS
        // L'option '-p' signifie "ne pas échouer si existe déjà"
        system("mkdir -p files");
    #endif
}

int profile_validate_pseudo(char* pseudo) {

    if (pseudo == NULL || strlen(pseudo) == 0) {
        return 0;
    }

    while (*pseudo != '\0') {

        if (!isalpha(*pseudo) && !isdigit(*pseudo) && *pseudo != '_') {
            return 0;
        }

        pseudo++;
    }

    return 1;
}

// Sauvegarde le profil dans un fichier txt du dossier "files/ sous le nom "<pseudo>.config.txt"
void profile_save(Profil * p) {
    char nom_fichier[256];
    char buffer[256];
    int nb_lignes = 0;


    // Construction du nom du fichier
    sprintf(nom_fichier, "files/%s.config.txt", p->pseudo);

    FILE * f = fopen(nom_fichier, "r");

    // Si le fichier n'existe pas, on le crée et on écrit directement les données
    if (f == NULL) {
        //printf("Erreur: Impossible d'ouvrir/creer le fichier de profil. Verifiez que le dossier 'files' existe.\n");
        f = fopen(nom_fichier, "w");
        fprintf(f, "%s %d %d %.2f %d %d\n",
            p->pseudo,
            p->grille_lignes,
            p->grille_cols,
            p->temps_par_coup,
            p->forme_pions,
            p->mode_jeu);
        fclose(f);
        return;
    }
    
    // Compte le nombre de lignes dans le fichier
    // Si plus d'une ligne, on cree un fichier temporaire pour les modifications 
    // et on remplace l'ancien fichier par le nouveau une fois les modifications faites.
    // Sinon, on ecrase directement le fichier existant
    while (fgets(buffer, sizeof(buffer), f)) {
        nb_lignes++;
        if (nb_lignes > 1) {
            break;
        }
    }
    rewind(f);

    if (nb_lignes > 1) {
        FILE *tmp = fopen("temp.txt", "w");
        int found = 1;

        
        while(fgets(buffer, sizeof(buffer), f)) {
            if(found){
                fprintf(tmp, "%s %d %d %.2f %d %d\n",
                    p->pseudo,
                    p->grille_lignes,
                    p->grille_cols,
                    p->temps_par_coup,
                    p->forme_pions,
                    p->mode_jeu);
                found = 0;
            }
            else{
                fputs(buffer, tmp);
            }
        }
        fclose(f);
        fclose(tmp);
        remove(nom_fichier);
        rename("temp.txt", nom_fichier);
    } else {
        f = fopen(nom_fichier, "w");
        fprintf(f, "%s %d %d %.2f %d %d\n",
            p->pseudo,
            p->grille_lignes,
            p->grille_cols,
            p->temps_par_coup,
            p->forme_pions,
            p->mode_jeu);
        fclose(f);
    }

   
}

// Charge le profil depuis un fichier txt du dossier "files/" sous le nom "<pseudo>.config.txt"
Profil profile_load(char* pseudo) {
    Profil p;

    p.pseudo[0] = '\0';

    char nom_fichier[256];

    sprintf(nom_fichier, "files/%s.config.txt", pseudo);

    FILE * f = fopen(nom_fichier, "r");

    if (f == NULL) {
        return p;
    }

    
    if (fscanf(f, "%s %d %d %f %d %d",
               p.pseudo,
               &p.grille_lignes,
               &p.grille_cols,
               &p.temps_par_coup,
               &p.forme_pions,
               &p.mode_jeu) == 6)
    {

    }
    else {
        p.pseudo[0] = '\0';
    }

    fclose(f);

    return p;
}

// Crée un profil par défaut avec les paramètres standards
Profil profile_create_default(char* pseudo) {

    Profil p;

    strcpy(p.pseudo, pseudo);

    p.grille_cols = MIN_COL;
    p.grille_lignes = MIN_LINE;
    p.temps_par_coup = TIMER_PLAY;
    p.forme_pions = 1;
    p.mode_jeu = 1;

    profile_save(&p);

    return p;
}

// Gère la connexion ou la création de profil du joueur à l'aide de son pseudo
Profil profile_login_or_create() {
    Profil profil_actif;
    char pseudo_saisi[50];

    utils_clear_screen();
    printf("==============================\n");
    printf("  BIENVENUE DANS PUISSANCE 4 !\n");
    printf("==============================\n\n");

    while (1) {
        printf("Veuillez entrer votre pseudo (lettres, chiffres, _) : ");
        utils_get_secure_string(pseudo_saisi, 50);

        if (profile_validate_pseudo(pseudo_saisi) == 0) {
            printf("-> Erreur : Pseudo invalide. Le format est incorrect.\n\n");
            continue;
        }

        profil_actif = profile_load(pseudo_saisi);

        if (profil_actif.pseudo[0] != '\0') {
            printf("-> Bon retour, %s !\n", profil_actif.pseudo);
            // Vérifier si une partie en pause existe
            char pause_file[256];
            sprintf(pause_file, "files/%s.pause.txt", profil_actif.pseudo);
            if (file_exists(pause_file)) {
                printf("\n>>> ATTENTION: Vous avez une partie en pause !\n");
                printf(">>> Allez a l'option 4 du menu pour la reprendre.\n\n");
            }
        } else {
            printf("-> Bienvenue, %s ! Creation de votre profil par defaut...\n", pseudo_saisi);
            profil_actif = profile_create_default(pseudo_saisi);
        }
        // Pause avant de continuer
        utils_pause_to_continue();
        break;
    }
    return profil_actif;
}

// Modifie les paramètres du profil actif
void profile_modify_settings(Profil* p) {
    int choice = 0;
    do {
        utils_clear_screen();
        printf("=== PARAMETRES (Profil: %s) ===\n", p->pseudo);
        printf(" 1. Modifier la Grille (Actuel: %dx%d)\n", p->grille_lignes, p->grille_cols);
        printf(" 2. Modifier le Temps/coup (Actuel: %.1fs)\n", p->temps_par_coup);
        printf(" 3. Modifier la Forme des Pions (Actuel: %d)\n", p->forme_pions);
        printf(" 4. Modifier le Mode par defaut (Actuel: %s)\n", (p->mode_jeu == 1 ? "PvP" : "PvIA"));
        printf(" 5. Retour au menu principal\n");
        printf("\nVotre choix : ");

        // Lecture du choix utilisateur
        choice = utils_get_int();

        switch (choice) {
            case 1: // taille de la Grille 
                printf("\n--- Modification Grille (Min 6x7, Max 20x20) ---\n");
                do {
                    printf("Nouvelle hauteur (lignes) : ");
                    p->grille_lignes = utils_get_int();
                } while (p->grille_lignes < MIN_LINE || p->grille_lignes > 20);
                do {
                    printf("Nouvelle largeur (colonnes) : ");
                    p->grille_cols = utils_get_int();
                } while (p->grille_cols < MIN_COL || p->grille_cols > 20);
                profile_save(p);
                break;
            case 2: // Temps par coup
                printf("\n--- Modification Temps (Min 15s, Max 30s) ---\n");
                do {
                    printf("Nouveau temps par coup (en secondes) : ");
                    p->temps_par_coup = utils_get_float();
                } while (p->temps_par_coup < 15.0 || p->temps_par_coup > 30.0);
                profile_save(p);
                break;
            case 3: // Forme
                printf("\n--- Modification Forme des Pions ---\n");
                printf(" 1. Style Lettres  : X et O\n");
                printf(" 2. Style Coeurs    : ♥ et ♡\n");
                printf(" 3. Style Etoiles   : ★ et ☆\n");
                do {
                    printf("Votre choix (1, 2 ou 3) : ");
                    p->forme_pions = utils_get_int();
                } while (p->forme_pions < 1 || p->forme_pions > 3);
                profile_save(p);
                break;
            case 4: // Mode
                printf("\n--- Modification Mode (1=PvP, 2=PvIA) ---\n");
                do {
                    printf("Votre choix (1 ou 2) : ");
                    p->mode_jeu = utils_get_int();
                } while (p->mode_jeu < 1 || p->mode_jeu > 2);
                profile_save(p);
                break;
            case 5: // Retour
                printf("-> Retour au menu principal.\n");
                break;
            default:
                printf("\n-> Choix invalide. Veuillez reessayer.\n");
                utils_pause_to_continue();
                break;
        }
    } while (choice != 5);
}
