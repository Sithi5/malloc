#include "malloc.h"

int main() {
    t_block *next;
    t_block *current;

    for (int i = 0; i < 160; i++) {
        malloc(64);
    }
    for (int i = 0; i < 100; i++) {
        malloc(1024);
    }

    show_zone_memory_usage();

    current = g_malloc.zone.tiny;
    while (current) {
        next = current->next;
        free(current + 1);
        current = next;
    }

    current = g_malloc.zone.small;
    while (current) {
        next = current->next;
        free(current + 1);
        current = next;
    }
    show_zone_memory_usage();

    return 0;
}
