#include "utils.h"

void utils_clear_screen() {
    #ifdef _WIN32
        system("cls"); // Commande Windows
    #else
        system("clear"); // Commande Linux/MacOS
    #endif
}


void utils_get_secure_string(char* buffer, int taille) {
    if (fgets(buffer, taille, stdin) != NULL) {
        // Supprime le '\n' capturé par fgets
        buffer[strcspn(buffer, "\n")] = '\0';
    }
}

// Vider le tampon stdin
void flush_stdin_buffer() {
    int c;
    // Lit tous les caractères jusqu'à la fin de la ligne ou la fin du fichier
    while ((c = getchar()) != '\n' && c != EOF);
}


int utils_get_int() {
    int choix = 0;

    // On utilise " %d" (avec un espace) pour ignorer les 'Entrée'
    // ou espaces précédents, mais ce n'est pas parfait.
    if (scanf("%d", &choix) != 1) {
        choix = -1; // Renvoyer une valeur invalide
    }
    flush_stdin_buffer();

    return choix;
}


float utils_get_float() {
    float choix = 0.0;

    if (scanf("%f", &choix) != 1) {
        choix = -1.0;
    }
    flush_stdin_buffer();

    return choix;
}


void utils_pause_to_continue() {
    printf("\nAppuyez sur Entree pour continuer...");
    char temp[10];
    utils_get_secure_string(temp, 10);
}