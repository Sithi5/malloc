#include "malloc.h"

t_zone g_zone = {NULL, NULL, NULL};

void *malloc(size_t size) {
    if (size == 0) {
        return NULL;
    }

    // Align size to a multiple of 8 for better memory alignment
    size = (size + 7) & ~7;

    t_block *last = NULL;
    t_block *free_block = find_free_block(&last, size);

    if (free_block == NULL) {
        // If no free block was found, request a new block from the system
        free_block = request_space(last, size);
        if (free_block == NULL) {
            // If the mmap request failed, return NULL
            return NULL;
        }

        if (size <= TINY_MAX && g_zone.tiny == NULL) {
            g_zone.tiny = free_block;
        } else if (size <= SMALL_MAX && g_zone.small == NULL) {
            g_zone.small = free_block;
        } else if (g_zone.large == NULL) {
            g_zone.large = free_block;
        }
    } else {
        // If a free block was found, split it if necessary
        free_block = split_block(free_block, size);
        free_block->free = 0;
    }

    // Return a pointer to the allocated memory (after the t_block structure)
    return (void *) (free_block + 1);
}
