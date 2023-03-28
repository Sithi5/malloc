#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
    int num_pages;
    if (argc != 2) {
        fprintf(stderr, "Usage: %s num_pages\n", argv[0]);
        return 1;
    }
    int page_size = getpagesize();
    long page_reclaims = atoi(argv[1]);
    double mb = (double) page_reclaims * page_size / (1024 * 1024);
    printf("%ld page reclaims is %.2f MB, pagesize is %i\n", page_reclaims, mb, page_size);
    return 0;
}