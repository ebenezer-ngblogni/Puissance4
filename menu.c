#include "menu.h"
#include "profil.h"
#include "display.h"
#include "gameplay.h"


void application_start() {

    // Initialisation
    profile_initialize_directory();

    Profil profil_actuel;
    int choix_menu = 0;
    int programme_actif = 1;

    // Boucle principale du programme
    while (programme_actif) {

        // Etape 1: Login
        profil_actuel = profile_login_or_create();

        int utilisateur_connecte = 1;

        // Boucle du menu principal (tant que l'utilisateur est connecté)
        while (utilisateur_connecte) {

            utils_clear_screen();
            printf("=== MENU PRINCIPAL ===\n");
            printf("Utilisateur : %s\n", profil_actuel.pseudo);
            printf("----------------------\n");
            printf(" Reglages : %dx%d | %.1fs | Mode: %s\n",
                   profil_actuel.grille_lignes, profil_actuel.grille_cols,
                   profil_actuel.temps_par_coup,
                   (profil_actuel.mode_par_defaut == 1 ? "PvP" : "PvIA"));
            printf("----------------------\n");

            printf(" 1. Nouvelle Partie\n");
            printf(" 2. Modifier les Parametres\n");
            printf(" 3. Consulter l'historique des parties\n");
            printf(" 4. Reprendre une partie\n");
            printf(" 5. Changer d'utilisateur\n");
            printf(" 6. Quitter\n");
            printf("\nVotre choix : ");

            choix_menu = utils_get_int();

            switch (choix_menu) {
                case 1:
                    printf("\n-> Lancement de 'Nouvelle Partie'...\n");
                    // twoPlayer(profil_actuel);
                    playerVsIa(profil_actuel, MOYEN);
                    break;
                case 2:
                    profile_modify_settings(&profil_actuel);
                    break;
                case 3:
                    printf("\n-> Affichage de 'Historique'...\n(Bientot disponible !)");
                    utils_pause_to_continue();
                    break;
                case 4:
                    printf("\n-> Chargement de 'Reprendre partie'...\n(Bientot disponible !)");
                    utils_pause_to_continue();
                    break;
                case 5: // Changer d'utilisateur
                    utilisateur_connecte = 0;
                    printf("\nDeconnexion...\n");
                    break;
                case 6: // Quitter
                    utilisateur_connecte = 0;
                    programme_actif = 0;
                    break;
                default:
                    printf("\n-> Choix invalide. Veuillez reessayer.\n");
                    utils_pause_to_continue();
                    break;
            }
        }
    }

    // Fin du programme
    utils_clear_screen();
    printf("\nMerci d'avoir joue ! A bientot.\n");
}
