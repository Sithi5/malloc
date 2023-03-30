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
 * Request a new block of memory from the system
 * @param last The last block in the memory zone
 * @param size The size of the block to request
 * @return A pointer to the new block, or NULL if the request failed
 */
t_block *request_space(size_t size) {
    size_t total_size = sizeof(t_block) + size;
    size_t zone_size = 0;
    t_block *block;
    t_block *zone;
    size_t block_offset;

    zone = NULL;
    if (size <= (size_t) TINY_MAX) {
        zone_size = TINY_ZONE_SIZE;
        zone = g_malloc.zone.tiny;
    } else if (size <= (size_t) SMALL_MAX) {
        zone_size = SMALL_ZONE_SIZE;
        zone = g_malloc.zone.small;
    } else {
        zone_size = size;
        zone = NULL;
    }

    if (zone != NULL && zone->remaining_memory >= total_size) {
        while (zone->next) {
            zone = zone->next;
        }
        if (size <= (size_t) TINY_MAX) {
            block_offset = TINY_ZONE_SIZE - g_malloc.zone.tiny->remaining_memory - total_size;
            block = (t_block *) ((char *) g_malloc.zone.tiny + sizeof(t_block) + block_offset);
            g_malloc.zone.tiny->remaining_memory -= total_size + sizeof(t_block);
        } else if (size <= (size_t) SMALL_MAX) {
            block_offset = SMALL_ZONE_SIZE - g_malloc.zone.small->remaining_memory - total_size;
            block = (t_block *) ((char *) g_malloc.zone.small + sizeof(t_block) + block_offset);
            g_malloc.zone.small->remaining_memory -= total_size + sizeof(t_block);
        }
        block->size = size;
        block->next = NULL;
        block->free = false;
        zone->next = block;
    } else {
        block =
            (t_block *) mmap(0, zone_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1,
                             0);   // Read and write, private, anonymous. fifth and sixth
                                   // arguments are ignored because of MAP_ANONYMOUS

        if (block == MAP_FAILED) {
            return NULL;
        }
        block->remaining_memory = zone_size - total_size > 0 ? zone_size - total_size : 0;
        block->size = size;
        block->next = NULL;
        block->free = false;

        if (size <= (size_t) TINY_MAX) {
            g_malloc.zone.tiny = block;
        } else if (size <= (size_t) SMALL_MAX) {
            g_malloc.zone.small = block;
        } else if (g_malloc.zone.large == NULL) {
            g_malloc.zone.large = block;
        } else {
            while (g_malloc.zone.large->next) {
                g_malloc.zone.large = g_malloc.zone.large->next;
            }
            g_malloc.zone.large->next = block;
        }
    }
    return block;
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