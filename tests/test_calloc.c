#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int i;
    int *pointer;

    pointer = (int *) calloc(20, sizeof(int));
    if (!pointer)
        return (0);
    i = -1;
    while (++i < 19)
        pointer[i] = i;
    i = -1;
    while (++i < 20)
        printf("%d ", pointer[i]);
    printf("\n");
    free(pointer);
    return (0);
}