#include "menu.h"

int menu(){
    printf("Bienvenue dans le jeu puissance 4\nVeuillez choisir une option:\n");
    printf("1-Jouer avec un ami\n");
    printf("2-Jouer contre l'ordi\n");
    printf("3-Charger une partie\n");
    printf("4-Options de jeu\n");
    printf("0-Retourner a windows\n");
    return getChoice();
}

int getChoice(){
    int choice;

    scanf("%d", &choice);
    switch (choice)
    {

    case 0 :
        return 0;
        break;

    case 1 :
        //twoPlayer();
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
        printf("Choix invalide \n");
        menu();
        break;
    }
}
