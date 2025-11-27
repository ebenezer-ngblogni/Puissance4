#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#define MIN_COL 7
#define MIN_LINE 6


#ifdef _WIN32
    #define IS_LINUX 0
    #define IS_WIN 1
    #define IS_MAC 0
#else
    #define IS_LINUX 1
    #define IS_WIN 0
    #define IS_MAC 0
#endif // UTILS_H_INCLUDED

enum niveau {FACILE, MOYEN, DIFFICILE};

// Efface l'écran (multi-plateforme)
void utils_clear_screen();

// Lit une chaine de manière robuste
void utils_get_secure_string(char* buffer, int taille);

// Vider le tampon stdin
void flush_stdin_buffer();

// Lit un entier de manière robuste
int utils_get_int();

// Lit un float de manière robuste
float utils_get_float();

// Met le programme en pause jusqu'à ce que l'utilisateur appuie sur Entrée
void utils_pause_to_continue();

#endif // UTILS_H_INCLUDED
