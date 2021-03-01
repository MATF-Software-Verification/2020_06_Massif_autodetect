#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    void** v_ptr_ptr = malloc(10 * sizeof (void*));
    for (size_t i = 0; i < 10; i++) {
        printf("Alociram %lu bajtova.\n", (i+1) * 30);
        v_ptr_ptr[i] = malloc((i+1) * 30);
        sleep((i+1)/4);
    }

    for (size_t i = 0; i < 10; i++) {
        printf("Oslobadjam %lu bajtova.\n", (i+1) * 30);
        free(v_ptr_ptr[i]);
        sleep((10 - i)/4);
    }
    free(v_ptr_ptr);
    return 0;
}
