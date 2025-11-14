#include "utils.h"
#include "menu.h"
#include "profil.h"
#include "display.c"
#include "gameplay.c"

void pause_pour_continuer(void) {
    printf("\nAppuyez sur Entree pour continuer...");
    char temp[10];
    menu_lire_chaine_securise(temp, 10);
}

int main()
{
    profil_initialiser_dossier();

    Profil profil_actuel;

    int choix_menu = 0;
    int programme_actif = 1;

    while (programme_actif) {

        profil_actuel = profil_login_ou_creer();

        int utilisateur_connecte = 1;

        while (utilisateur_connecte) {

            choix_menu = menu_afficher_principal(&profil_actuel);

            switch (choix_menu) {

                case 1:
                    printf("\n-> Lancement de 'Nouvelle Partie'...\n");
                    twoPlayer();
                    break;

                case 2:
                    profil_modifier_parametres(&profil_actuel);
                    break;

                case 3:
                    printf("\n-> Affichage de 'Historique'...\n(Bientot disponible !)");
                    pause_pour_continuer();
                    break;

                case 4:
                    printf("\n-> Chargement de 'Reprendre partie'...\n(Bientot disponible !)");
                    pause_pour_continuer();
                    break;

                case 5:
                    utilisateur_connecte = 0; // Deconnexion
                    break;

                case 6:
                    utilisateur_connecte = 0;
                    programme_actif = 0;
                    break;

                default:
                    menu_afficher_erreur_choix_invalide();
                    break;
            }
        }
    }

    menu_afficher_au_revoir();

    return 0;
}
