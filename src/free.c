#include "malloc.h"

static void merge_free_blocks(t_block *block) {
    // Merge with the next block if it is free
    if (block->next && block->next->free) {
        block->size += block->next->size + sizeof(t_block);
        block->next = block->next->next;
    }
}

static int is_entire_zone_free(t_block *zone_start) {
    while (zone_start) {
        if (!zone_start->free) {
            return 0;
        }
        zone_start = zone_start->next;
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
    if (ptr == NULL) {
        return;
    }

    // Get the block pointer from the memory pointer
    t_block *block = (t_block *) ptr - 1;

    // Mark the block as free
    block->free = 1;

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
    try_release_zone(&g_malloc.zone.tiny);
    try_release_zone(&g_malloc.zone.small);
}
