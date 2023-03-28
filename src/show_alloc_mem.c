#include "malloc.h"

static size_t print_zone(t_block *zone) {
    size_t total = 0;
    t_block *block = zone;

    while (block) {
        if (!block->free) {
            printf("%p - %p : %zu bytes\n", (void *) (block + 1),
                   (void *) ((char *) (block + 1) + block->size), block->size);
            total += block->size;
        }
        block = block->next;
    }

    return total;
}

static size_t print_large_blocks(t_block *zone) {
    size_t total = 0;
    t_block *block = zone;

    while (block) {
        if (!block->free) {
            printf("%p - %p : %zu bytes\n", (void *) (block + 1), (void *) (block->next),
                   block->size);
            total += block->size;
        }
        block = block->next;
    }

    return total;
}

void show_alloc_mem() {
    size_t total = 0;

    printf("TINY : %p\n", g_zone.tiny);
    total += print_zone(g_zone.tiny);

    printf("SMALL : %p\n", g_zone.small);
    total += print_zone(g_zone.small);

    printf("LARGE : %p\n", g_zone.large);
    total += print_large_blocks(g_zone.large);

    printf("Total : %zu bytes\n", total);
}