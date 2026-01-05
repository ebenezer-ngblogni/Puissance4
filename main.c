#include "menu.h"
#include "utils.h"

int main()
{
        // Configurer la console Windows pour utiliser l'encodage UTF-8
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
    #endif

    application_start();

    return 0;
}

