#include "menu.h"
#include "utils.h"

int main()
{
    if(IS_WIN){
        // Configurer la console Windows pour utiliser l'encodage UTF-8
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
    }

    application_start();

    return 0;
}

