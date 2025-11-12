#include "utils.h"
#include "profil.h"
#include "menu.h"

int profil_valider_pseudo(char* pseudo) {

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


void profil_sauvegarder(Profil * p) {
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


Profil profil_charger(char* pseudo) {
    Profil p;

    p.pseudo[0] = '\0';

    char nom_fichier[256];

    // Construire le chemin du fichier, exactement comme dans profil_sauvegarder
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
        // Succès ! Le profil 'p' est maintenant rempli.
    }
    else {
        p.pseudo[0] = '\0';
    }

    fclose(f);

    return p;
}


Profil profil_creer_defaut(char* pseudo) {

    Profil p;

    strcpy(p.pseudo, pseudo);

    p.grille_cols = 7;
    p.grille_lignes = 6;
    p.temps_par_coup = 30.0;
    p.forme_pions = 1;
    p.mode_par_defaut = 1;

    profil_sauvegarder(&p);

    return p;
}


void profil_modifier_parametres(Profil* p) {

    // Si le pointeur de profil est invalide, on ne peut rien faire.
    if (p == NULL) {
        return;
    }

    int choix_menu_params = 0;

    // Boucle 'do-while' pour afficher le menu des paramètres
    // tant que l'utilisateur ne choisit pas "Retour" (ex: option 4)
    do {
        // 1. On appelle une fonction de menu.c qui AFFICHE les options
        //    et renvoie le choix de l'utilisateur.
        choix_menu_params = menu_afficher_menu_parametres(p);

        switch (choix_menu_params) {

            case 1: // Modifier la Grille
                { // On ouvre un bloc pour déclarer des variables temporaires
                    int nouvelles_lignes, nouvelles_cols;

                    // On appelle la fonction de menu.c qui gère la saisie
                    menu_demander_nouvelle_grille(&nouvelles_lignes, &nouvelles_cols);

                    // On met à jour le profil
                    p->grille_lignes = nouvelles_lignes;
                    p->grille_cols = nouvelles_cols;

                    // On sauvegarde immédiatement
                    profil_sauvegarder(p);
                    printf("-> Taille de la grille mise a jour.\n");
                }
                break;

            case 2: // Modifier le Temps par coup (tp)
                // La fonction de menu.c renvoie directement la nouvelle valeur
                p->temps_par_coup = menu_demander_nouveau_tp();

                profil_sauvegarder(p);
                printf("-> Temps par coup mis a jour.\n");
                break;

            case 3: // Modifier la Forme des pions
                // La fonction de menu.c renvoie le nouvel identifiant (1, 2 ou 3)
                p->forme_pions = menu_demander_nouvelle_forme();

                profil_sauvegarder(p);
                printf("-> Forme des pions mise a jour.\n");
                break;

            case 4:
                p->mode_par_defaut = menu_demander_nouveau_mode_defaut();
                profil_sauvegarder(p);
                printf("-> Mode par defaut mis a jour.\n");
                break;

            case 5:
                printf("-> Retour au menu principal.\n");
                break;

            default:
                menu_afficher_erreur_choix_invalide();
                break;
        }

    } while (choix_menu_params != 5);
}


Profil profil_login_ou_creer() {

    Profil profil_actif;
    char pseudo_saisi[50]; // Un buffer pour stocker la saisie

    // 1. Afficher le message d'accueil
    menu_afficher_bienvenue();

    // 2. Boucler jusqu'à ce qu'on obtienne un profil valide
    while (1) {

        // 3. Demander le pseudo à l'utilisateur
        menu_demander_pseudo(pseudo_saisi);

        // 4. Valider le format du pseudo
        if (profil_valider_pseudo(pseudo_saisi) == 0) {
            menu_afficher_erreur_pseudo(); // "Pseudo invalide..."
            continue; // Recommencer la boucle
        }

        // 5. Le format est bon, on tente de charger le fichier
        profil_actif = profil_charger(pseudo_saisi);

        // 6. Vérifier si le chargement a réussi
        if (profil_actif.pseudo[0] != '\0') {
            // SUCCÈS : Le profil existait
            menu_afficher_bon_retour(profil_actif.pseudo);
            break; // Sortir de la boucle while(1)

        } else {
            // ÉCHEC : Le profil n'existe pas, c'est un nouveau joueur
            menu_afficher_nouveau_profil(pseudo_saisi);

            // On crée le profil par défaut et on le stocke
            profil_actif = profil_creer_defaut(pseudo_saisi);
            break; // Sortir de la boucle while(1)
        }
    }

    // 7. Renvoyer le profil (soit chargé, soit nouvellement créé)
    return profil_actif;
}
