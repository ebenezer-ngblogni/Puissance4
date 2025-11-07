#include "file.h"
#include "utils.h"

int savePreference (preference pref){
    FILE* fichier=NULL;
    fichier=fopen(pref.pseudo+'.txt', "a");

    fclose(fichier);

}