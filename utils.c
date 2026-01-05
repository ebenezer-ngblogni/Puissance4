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

// Lit un entier de manière sécurisée
int utils_get_int() {
    int choice = 0;

    // On utilise " %d" (avec un espace) pour ignorer les 'Entrée'
    // ou espaces précédents, mais ce n'est pas parfait.
    if (scanf("%d", &choice) != 1) {
        choice = -1; // Renvoyer une valeur invalide
    }
    flush_stdin_buffer();

    return choice;
}


float utils_get_float() {
    float choice = 0.0;

    if (scanf("%f", &choice) != 1) {
        choice = -1.0;
    }
    flush_stdin_buffer();

    return choice;
}


void utils_pause_to_continue() {
    printf("\nAppuyez sur Entree pour continuer...");
    char temp[10];
    utils_get_secure_string(temp, 10);
}