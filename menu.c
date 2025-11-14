#include "menu.h"
#include "utils.h"
#include "gameplay.h"


/* ================================================================== */
/* FONCTIONS UTILITAIRES (INTERNES)                    */
/* ================================================================== */

/**
 * Helper interne pour nettoyer la console (multi-plateforme).
 */
void menu_clear_screen() {
    #ifdef _WIN32
        system("cls"); // Commande Windows
    #else
        system("clear"); // Commande Linux/MacOS
    #endif
}

/**
 * Helper interne pour lire une chaîne de manière sécurisée (remplace fgets)
 * et supprime le caractère '\n' final.
 */
void menu_lire_chaine_securise(char* buffer, int taille) {
    if (fgets(buffer, taille, stdin) != NULL) {
        // Supprime le '\n' capturé par fgets
        buffer[strcspn(buffer, "\n")] = '\0';
    }
}

/**
 * Helper interne pour lire un entier de manière robuste.
 * Lit une ligne complète et la convertit.
 */
int menu_lire_choix_int() {
    char buffer[100];
    menu_lire_chaine_securise(buffer, 100);
    // Convertit la chaîne en entier (base 10)
    return (int) strtol(buffer, NULL, 10);
}

/**
 * Helper interne pour lire un float de manière robuste.
 */
float menu_lire_choix_float() {
    char buffer[100];
    menu_lire_chaine_securise(buffer, 100);
    // Convertit la chaîne en float/double
    return (float) strtod(buffer, NULL);
}

/* ================================================================== */
/* FONCTIONS DE LOGIN / ACCUEIL                       */
/* ================================================================== */

void menu_afficher_bienvenue() {
    menu_clear_screen();
    printf("==============================\n");
    printf("  BIENVENUE DANS PUISSANCE 4 !\n");
    printf("==============================\n\n");
}

void menu_demander_pseudo(char* buffer_pseudo) {
    printf("Veuillez entrer votre pseudo (lettres, chiffres, _) : ");
    // Utilise notre helper sécurisé (taille max du pseudo = 50)
    menu_lire_chaine_securise(buffer_pseudo, 50);
}

void menu_afficher_erreur_pseudo() {
    printf("-> Erreur : Pseudo invalide. Le format est incorrect.\n\n");
}

void menu_afficher_bon_retour(char* pseudo) {
    printf("-> Bon retour, %s !\n", pseudo);
}

void menu_afficher_nouveau_profil(char* pseudo) {
    printf("-> Bienvenue, %s ! Creation de votre profil par defaut...\n", pseudo);
}

/* ================================================================== */
/* MENU PRINCIPAL                               */
/* ================================================================== */

int menu_afficher_principal(Profil* p) {
    menu_clear_screen();
    printf("\n=== MENU PRINCIPAL ===\n");
    printf("Utilisateur : %s\n", p->pseudo);
    printf("----------------------\n");

    // Affichage des réglages actuels
    printf(" Reglages actuels :\n");
    printf(" > Grille : %dx%d\n", p->grille_lignes, p->grille_cols);
    printf(" > Pions  : Forme %d\n", p->forme_pions);
    printf(" > Temps  : %.1f sec/coup\n", p->temps_par_coup);
    printf(" > Mode   : %s\n", (p->mode_par_defaut == 1 ? "Joueur vs Joueur" : "Joueur vs IA"));
    printf("----------------------\n");

    // Options du menu
    printf("Que souhaitez-vous faire ?\n");
    printf(" 1. Nouvelle Partie\n");
    printf(" 2. Modifier les Parametres\n");
    printf(" 3. Consulter l'historique des parties\n");
    printf(" 4. Reprendre une partie\n");
    printf(" 5. Changer d'utilisateur\n");
    printf(" 6. Quitter\n");
    printf("\nVotre choix : ");

    return menu_lire_choix_int();
}

void menu_afficher_au_revoir(void) {
    printf("\nMerci d'avoir joue ! A bientot.\n");
}

/* ================================================================== */
/* SOUS-MENU PARAMETRES                           */
/* ================================================================== */

int menu_afficher_menu_parametres(Profil* p) {
    menu_clear_screen();
    printf("\n=== PARAMETRES (Profil: %s) ===\n", p->pseudo);
    printf(" 1. Modifier la Grille (Actuel: %dx%d)\n", p->grille_lignes, p->grille_cols);
    printf(" 2. Modifier le Temps/coup (Actuel: %.1fs)\n", p->temps_par_coup);
    printf(" 3. Modifier la Forme des Pions (Actuel: %d)\n", p->forme_pions);
    printf(" 4. Modifier le Mode par defaut (Actuel: %s)\n", (p->mode_par_defaut == 1 ? "PvP" : "PvIA"));
    printf(" 5. Retour au menu principal\n");
    printf("\nVotre choix : ");

    return menu_lire_choix_int();
}

void menu_demander_nouvelle_grille(int* lignes, int* cols) {
    printf("\n--- Modification Grille (Min 4x4, Max 20x20) ---\n");

    do {
        printf("Nouvelle hauteur (lignes) : ");
        *lignes = menu_lire_choix_int();
    } while (*lignes < 4 || *lignes > 20);

    do {
        printf("Nouvelle largeur (colonnes) : ");
        *cols = menu_lire_choix_int();
    } while (*cols < 4 || *cols > 20);
}

float menu_demander_nouveau_tp() {
    float tp = 0;
    printf("\n--- Modification Temps (Min 5s, Max 60s) ---\n");

    do {
        printf("Nouveau temps par coup (en secondes) : ");
        tp = menu_lire_choix_float();
    } while (tp < 5.0 || tp > 60.0);

    return tp;
}

int menu_demander_nouvelle_forme() {
    int forme = 0;
    printf("\n--- Modification Pions ---\n");
    printf(" 1. Forme (X / O)\n");
    printf(" 2. Forme (A / B)\n");
    printf(" 3. Forme (R / B) (Couleurs)\n");

    do {
        printf("Votre choix (1, 2 ou 3) : ");
        forme = menu_lire_choix_int();
    } while (forme < 1 || forme > 3);

    return forme;
}

int menu_demander_nouveau_mode_defaut() {
    int mode = 0;
    printf("\n--- Modification Mode par defaut ---\n");

    do {
        printf(" 1. Joueur vs Joueur (PvP)\n");
        printf(" 2. Joueur vs IA (PvIA)\n");
        printf("Votre choix (1 ou 2) : ");
        mode = menu_lire_choix_int();
    } while (mode < 1 || mode > 2);

    return mode;
}

/* ================================================================== */
/* MESSAGES D'ERREUR                            */
/* ================================================================== */

void menu_afficher_erreur_choix_invalide() {
    printf("\n-> Choix invalide. Veuillez reessayer.\n");
    printf("Appuyez sur Entree pour continuer...");
    char temp[2]; // Buffer temporaire juste pour attendre une entrée
    menu_lire_chaine_securise(temp, 2);
}
