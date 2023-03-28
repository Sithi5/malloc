#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int get_pages_reclamed(int max_resident_set_size_kb, int page_size_bytes) {
    // Convert max_resident_set_size_kb to bytes
    int max_resident_set_size_bytes = max_resident_set_size_kb * 1024;

    // Calculate the number of pages used by the program
    int pages_reclamed = ceil(max_resident_set_size_bytes / (double) page_size_bytes);

    return pages_reclamed;
}

int main(int argc, char **argv) {
    int max_resident_set_size_kb;
    int pages_reclamed;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s max_resident_set_size_kb\n", argv[0]);
        return 1;
    }
    int page_size = getpagesize();
    max_resident_set_size_kb = atoi(argv[1]);
    pages_reclamed = get_pages_reclamed(max_resident_set_size_kb, page_size);
    printf("%i pages reclaimed for %i max_resident_set_size_kb\n", pages_reclamed,
           max_resident_set_size_kb);
    return 0;
}