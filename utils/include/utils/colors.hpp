#pragma once
#include <stdio.h>

inline void red(){
    printf("\033[0;31m");
}

inline void bold_red(){
    printf("\033[1;31m");
}

inline void reset(){
    printf("\033[0m");
}

inline void green(){
    printf("\033[32m");
}

inline void blue(){
    printf("\033[0;34m");
}
