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
#endif // UTILS_H_INCLUDED
