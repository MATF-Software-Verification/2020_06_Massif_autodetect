#pragma once
#include <stdio.h>

void red(){
    printf("\033[0;31m");
}

void bold_red(){
    printf("\033[1;31m");
}

void reset(){
    printf("\033[0m");
}

void green(){
    printf("\033[32m");
}

void blue(){
    printf("\033[0;34m");
}
