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


void profile_save(Profil * p) {
    char nom_fichier[256];

    sprintf(nom_fichier, "files/%s.config.txt", p->pseudo);

    FILE * f = fopen(nom_fichier, "w");

    if (f == NULL) {
        printf("Erreur: Impossible d'ouvrir/creer le fichier de profil. Verifiez que le dossier 'files' existe.\n");
        return;
    }

    fprintf(f, "%s %d %d %f %d %d",
            p->pseudo,
            p->grille_cols,
            p->grille_lignes,
            p->temps_par_coup,
            p->forme_pions,
            p->mode_par_defaut);

    fclose(f);
}


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
               &p.grille_cols,
               &p.grille_lignes,
               &p.temps_par_coup,
               &p.forme_pions,
               &p.mode_par_defaut) == 6)
    {

    }
    else {
        p.pseudo[0] = '\0';
    }

    fclose(f);

    return p;
}


Profil profile_create_default(char* pseudo) {

    Profil p;

    strcpy(p.pseudo, pseudo);

    p.grille_cols = MIN_COL;
    p.grille_lignes = MIN_LINE;
    p.temps_par_coup = TIMER_PLAY;
    p.forme_pions = 1;
    p.mode_par_defaut = 1;

    profile_save(&p);

    return p;
}


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
        } else {
            printf("-> Bienvenue, %s ! Creation de votre profil par defaut...\n", pseudo_saisi);
            profil_actif = profile_create_default(pseudo_saisi);
        }

        utils_pause_to_continue();
        break;
    }
    return profil_actif;
}

void profile_modify_settings(Profil* p) {
    int choix = 0;
    do {
        utils_clear_screen();
        printf("=== PARAMETRES (Profil: %s) ===\n", p->pseudo);
        printf(" 1. Modifier la Grille (Actuel: %dx%d)\n", p->grille_lignes, p->grille_cols);
        printf(" 2. Modifier le Temps/coup (Actuel: %.1fs)\n", p->temps_par_coup);
        printf(" 3. Modifier la Forme des Pions (Actuel: %d)\n", p->forme_pions);
        printf(" 4. Modifier le Mode par defaut (Actuel: %s)\n", (p->mode_par_defaut == 1 ? "PvP" : "PvIA"));
        printf(" 5. Retour au menu principal\n");
        printf("\nVotre choix : ");

        choix = utils_get_int();

        switch (choix) {
            case 1: // Grille
                printf("\n--- Modification Grille (Min 4x4, Max 20x20) ---\n");
                do {
                    printf("Nouvelle hauteur (lignes) : ");
                    p->grille_lignes = utils_get_int();
                } while (p->grille_lignes < 4 || p->grille_lignes > 20);
                do {
                    printf("Nouvelle largeur (colonnes) : ");
                    p->grille_cols = utils_get_int();
                } while (p->grille_cols < 4 || p->grille_cols > 20);
                profile_save(p);
                break;
            case 2: // Temps
                printf("\n--- Modification Temps (Min 5s, Max 60s) ---\n");
                do {
                    printf("Nouveau temps par coup (en secondes) : ");
                    p->temps_par_coup = utils_get_float();
                } while (p->temps_par_coup < 5.0 || p->temps_par_coup > 60.0);
                profile_save(p);
                break;
            case 3: // Forme
                printf("\n--- Modification Pions (1, 2 ou 3) ---\n");
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
                    p->mode_par_defaut = utils_get_int();
                } while (p->mode_par_defaut < 1 || p->mode_par_defaut > 2);
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
    } while (choix != 5);
}
