#include <stdio.h>
#include <stdlib.h>


int main(int argc, char const *argv[])
{
    void** aptr = (void**)malloc(10 * sizeof(void*));
    for (int i = 0; i < 10; i++)
    {
        aptr[i] = malloc(1024);
        free(aptr[i]);
    }
    return 0;
}



void foo()
{
    int* arr = (int*)malloc(2 * sizeof(int));

    printf("arry %d", arr[0]);
    free(arr);

}