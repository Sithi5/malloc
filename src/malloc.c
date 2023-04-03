#include "malloc.h"

t_malloc g_malloc = {};

void *malloc(size_t size) {
    if (size == 0) {
        return NULL;
    }
    // Align size to a multiple of 8 for better memory alignment
    size = (size + 7) - (size + 7) % 8;

    t_block *last = NULL;
    t_block *free_block = find_free_block(&last, size);
    if (free_block == NULL) {
        // If no free block was found, request a new block from the system
        free_block = request_space(size);
        if (free_block == NULL) {
            return NULL;
        }
    } else {
        // If a free block was found, split it if necessary
        free_block = split_block(free_block, size);
        free_block->free = 0;
    }

    // Return a pointer to the allocated memory (after the t_block structure)
    return (void *) (free_block + 1);
}
