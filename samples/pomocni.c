#include <stdio.h>
#include <stdlib.h>
#include "pomocni.h"

void foo()
{
    int* arr = (int*)malloc(2 * sizeof(int));

    printf("arry %d", arr[0]);
    free(arr);

}