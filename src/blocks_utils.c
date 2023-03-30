#include "malloc.h"

/**
 * Find a free block of memory in the memory zone
 * @param size The size of the block to find
 * @return A pointer to the free block, or NULL if no free block was found
 */
t_block *find_free_block(t_block **last, size_t size) {
    t_block *current = NULL;

    if (size <= (size_t) TINY_MAX) {
        current = g_malloc.zone.tiny;
    } else if (size <= (size_t) SMALL_MAX) {
        current = g_malloc.zone.small;
    } else {
        current = g_malloc.zone.large;
    }

    while (current && !(current->free && current->size >= size)) {
        *last = current;
        current = current->next;
    }

    return current;
}

/*
 * Split a block into two blocks
 * @param block The block to split
 * @param size The size of the first block
 * @return A pointer to the first block
 */
t_block *split_block(t_block *block, size_t size) {
    // Calculate the size of the remaining space after splitting
    size_t remaining_size = block->size - size - sizeof(t_block);

    // Check if the remaining space is large enough for a new block
    if (remaining_size >= sizeof(t_block) + 1) {
        // Create a new block in the remaining space
        t_block *new_block = (t_block *) ((char *) (block + 1) + size);
        new_block->size = remaining_size;
        new_block->next = block->next;
        new_block->free = true;

        // Update the original block's information
        block->size = size;
        block->next = new_block;
    }

    return block;
}

ZoneType is_valid_block(t_block *block) {
    t_block *current_zone;

    // Check if the block is in the tiny zone
    current_zone = g_malloc.zone.tiny;
    while (current_zone) {
        if (block == current_zone) {
            return 1;
        }
        current_zone = current_zone->next;
    }

    // Check if the block is in the small zone
    current_zone = g_malloc.zone.small;
    while (current_zone) {
        if (block == current_zone) {
            return 1;
        }
        current_zone = current_zone->next;
    }

    // Check if the block is in the large zone
    current_zone = g_malloc.zone.large;
    while (current_zone) {
        if (block == current_zone) {
            return 1;
        }
        current_zone = current_zone->next;
    }

    return 0;
}