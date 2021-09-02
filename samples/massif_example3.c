#include <stdio.h>
#include <stdlib.h>
#include "pomocni.h"

int main(int argc, char const *argv[])
{
    void** aptr = (void**)malloc(10 * sizeof(void*));
    for (int i = 0; i < 10; i++)
    {
        aptr[i] = malloc(1024);
        foo();
        free(aptr[i]);
    }
    return 0;
}

