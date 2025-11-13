#include "utils.h"
#include "menu.c"
#include "display.c"
#include "gameplay.c"




int main()
{

    if(IS_WIN){
        system("chcp 65001");
        //showPlayer2piece();
    }
 

    int cond = 1;
    while(cond = menuPrincipal()){

    }

    /* showPlayer1piece();
    printf("\n");
    showPlayer2piece();*/
    //twoPlayer();

    /*#ifdef _WIN32
    printf("Je suis sous Windows\n");
    #ifdef __linux__
        printf("Je suis sous Linux\n");
    #endif
    Nisrine Bajja
    19:12
    #ifdef _WIN32
        system("chcp 65001");
    #endif*/

    return 0;
}
