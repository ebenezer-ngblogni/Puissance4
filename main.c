#include "utils.h"
#include "menu.h"
#include "profil.h"
#include "display.c"
#include "gameplay.c"

void pause_pour_continuer(void) {
    printf("\nAppuyez sur Entree pour continuer...");
    // On utilise la fonction de lecture robuste de menu.c
    // pour attendre une entrée de l'utilisateur.
    char temp[10];
    menu_lire_chaine_securise(temp, 10);
}

int main()
{
    // La variable qui contiendra le profil du joueur actif
    Profil profil_actuel;

    int choix_menu = 0;
    int programme_actif = 1; // 1 = Vrai, 0 = Faux

    // Boucle principale du programme (tant qu'on ne quitte pas)
    while (programme_actif) {

        // --- ETAPE 1 : GESTION DU LOGIN ---
        // Cette fonction gère tout : accueil, saisie, validation,
        // chargement (profil_charger) ou création (profil_creer_defaut).
        // On récupère le profil valide dans notre variable.
        profil_actuel = profil_login_ou_creer();

        int utilisateur_connecte = 1;

        // --- ETAPE 2 : BOUCLE DU MENU PRINCIPAL ---
        // Tant que l'utilisateur est connecté (ne choisit pas "Changer d'utilisateur" ou "Quitter")
        while (utilisateur_connecte) {

            // On affiche le menu principal et on récupère le choix.
            // On passe l'ADRESSE (&) de profil_actuel, car la fonction
            // attend un POINTEUR (Profil*).
            choix_menu = menu_afficher_principal(&profil_actuel);

            switch (choix_menu) {

                case 1: // Nouvelle Partie
                    printf("\n-> Lancement de 'Nouvelle Partie'...\n(Bientot disponible !)");
                    pause_pour_continuer();
                    break;

                case 2: // Modifier les Paramètres
                    // On passe l'ADRESSE (&) car la fonction
                    // doit pouvoir MODIFIER le profil.
                    profil_modifier_parametres(&profil_actuel);
                    // (Pas besoin de pause, la fonction gère son propre menu)
                    break;

                case 3: // Consulter l'historique
                    printf("\n-> Affichage de 'Historique'...\n(Bientot disponible !)");
                    pause_pour_continuer();
                    break;

                case 4: // Reprendre une partie
                    printf("\n-> Chargement de 'Reprendre partie'...\n(Bientot disponible !)");
                    pause_pour_continuer();
                    break;

                case 5: // Changer d'utilisateur
                    utilisateur_connecte = 0; // Casse la boucle du menu principal
                    printf("\nDeconnexion...\n");
                    // (On retourne à la boucle 'programme_actif' pour relancer le login)
                    break;

                case 6: // Quitter
                    utilisateur_connecte = 0; // Casse la boucle du menu
                    programme_actif = 0;    // Casse la boucle du programme
                    break;

                default:
                    // Gère les saisies invalides (ex: 9)
                    // Cette fonction (de menu.c) inclut déjà une pause.
                    menu_afficher_erreur_choix_invalide();
                    break;
            }
        }
    }

    // --- ETAPE 3 : FIN DU PROGRAMME ---
    menu_afficher_au_revoir();

    // int cond = 1;
    // while(cond = menuPrincipal()){

    // }

    /* showPlayer1piece();
    printf("\n");
    showPlayer2piece(); */
    twoPlayer();

    return 0;
}
