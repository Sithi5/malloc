#include "malloc.h"

static void merge_free_blocks(t_block *block) {
    // Merge with the next block if it is free
    if (block->next && block->next->free) {
        block->size += block->next->size + sizeof(t_block);
        block->next = block->next->next;
    }
}

static int is_entire_zone_free(t_block *zone) {
    while (zone) {
        if (!zone->free) {
            return 0;
        }
        zone = zone->next;
    }
    return 1;
}

static void try_release_zone(t_block **zone) {
    if (is_entire_zone_free(*zone)) {
        munmap(*zone, (*zone)->size + sizeof(t_block));
        *zone = NULL;
    }
}

void free(void *ptr) {
    ZoneType zone_type;
    t_block *block;

    pthread_mutex_lock(&g_mutex);
    if (ptr == NULL) {
        pthread_mutex_unlock(&g_mutex);
        return;
    }

    block = (t_block *) ptr - 1;

    zone_type = is_valid_block(block);
    // Check if the block is a valid memory block
    if (zone_type == INVALID_ZONE) {
        pthread_mutex_unlock(&g_mutex);
        return;
    }

    // Get the block pointer from the memory pointer

    // Mark the block as free
    block->free = true;
    // Merge with adjacent free blocks
    merge_free_blocks(block);

    // Check if the previous block is free and merge if necessary
    if (block->next) {
        t_block *prev = g_malloc.zone.tiny;
        while (prev && prev->next != block) {
            prev = prev->next;
        }

        if (prev && prev->free) {
            merge_free_blocks(prev);
        }
    }

    // Try releasing memory zones back to the system

    if (block->size <= (size_t) TINY_MAX) {
        try_release_zone(&g_malloc.zone.tiny);
    } else if (block->size <= (size_t) SMALL_MAX) {
        try_release_zone(&g_malloc.zone.small);
    } else {
        try_release_zone(&g_malloc.zone.large);
    }
    pthread_mutex_unlock(&g_mutex);
}
