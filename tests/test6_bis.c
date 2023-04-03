#include "malloc.h"
#include <stdlib.h>

int main() {
    malloc(10);
    malloc(30);
    malloc(64);
    malloc(400);
    malloc(500);
    malloc(1024);
    malloc(1025);
    show_alloc_mem();
    return (0);
}