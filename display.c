#include "display.h"

void showPlayer1piece() {
    printf("\033[1;34m");
    printf("\u2B24");
    printf("\033[0m");
}

void showPlayer2piece() {
    printf("\033[0;31m");
    printf("\u2B24");
    printf("\033[0m");
}

