#include "menu.h"
#include "gameplay.h"


int menuPrincipal(){
    printf("\n Bienvenue dans le jeu puissance 4 \n Veuillez choisir une option:\n");
    printf("1-Jouer avec un ami\n");
    printf("2-Jouer contre l'ordi\n");
    printf("3-Charger une partie\n");
    printf("4-Options de jeu\n");
    printf("0-Retourner à windows\n");
    return getChoiceMenuPrincipal();
}

int getChoiceMenuPrincipal(){
    int choice;

    scanf("%d", &choice);
    switch (choice)
    {

    case 0 :
        return 0;
        break;

    case 1 :
        twoPlayer();
        return 1;
        break;

    case 2 :
        return 1;
        break;

    case 3 :
        return 1;
        break;

    case 4 :
        return 1;
        break;

    default:
        printf("\n Choix invalide \n");
        menuPrincipal();
        break;
    }
}

/*int menuOptions(){
    printf("Options de jeu\n");
    printf("1-Modifier la taille de la grille\n");
    printf("2-Modifier les pièces\n");
    printf("0-Retourner au menu\n");
    return getChoiceOptions();
}

int getChoiceOptions(){
    int choice;

    scanf("%d", &choice);
    switch (choice)
    {
        case 0 :
            return 0;
            break;

        case 1 :
            return 1;
            break;

        case 2 :
            return 1;
            break;

        default:
            printf("Choix invalide");
            menuOptions();
            break;
    }
}*/
