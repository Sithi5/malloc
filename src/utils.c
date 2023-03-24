#include "malloc.h"

t_block *find_free_block(t_block **last, size_t size) {
    t_block *current = NULL;

    if (size <= TINY_MAX) {
        current = g_zone.tiny;
    } else if (size <= SMALL_MAX) {
        current = g_zone.small;
    } else {
        current = g_zone.large;
    }

    while (current && !(current->free && current->size >= size)) {
        *last = current;
        current = current->next;
    }

    return current;
}

t_block *request_space(t_block *last, size_t size) {
    size_t total_size = sizeof(t_block) + size;
    size_t zone_size = 0;

    if (size <= TINY_MAX) {
        zone_size = TINY_ZONE_SIZE;
    } else if (size <= SMALL_MAX) {
        zone_size = SMALL_ZONE_SIZE;
    } else {
        zone_size = total_size;
    }

    // Request memory using mmap
    t_block *block =
        (t_block *) mmap(0, zone_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (block == MAP_FAILED) {
        return NULL;
    }

    // Initialize the block
    block->size = size;
    block->next = NULL;
    block->free = 0;

    // Update the last block's next pointer
    if (last) {
        last->next = block;
    }

    return block;
}

t_block *split_block(t_block *block, size_t size) {
    // Calculate the size of the remaining space after splitting
    size_t remaining_size = block->size - size - sizeof(t_block);

    // Check if the remaining space is large enough for a new block
    if (remaining_size >= sizeof(t_block) + 1) {
        // Create a new block in the remaining space
        t_block *new_block = (t_block *) ((char *) (block + 1) + size);
        new_block->size = remaining_size;
        new_block->next = block->next;
        new_block->free = 1;

        // Update the original block's information
        block->size = size;
        block->next = new_block;
    }

    return block;
}