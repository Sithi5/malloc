#include "malloc.h"

ZoneType get_zone_type(size_t size) {
    if (size <= (size_t) TINY_MAX) {
        return TINY_ZONE;
    } else if (size <= (size_t) SMALL_MAX) {
        return SMALL_ZONE;
    } else {
        return LARGE_ZONE;
    }
}

t_block *create_block_in_existing_zone(t_block *zone, size_t size, size_t zone_size,
                                       size_t total_size) {
    size_t block_offset;
    t_block *block;
    ZoneType zone_type;

    zone_type = get_zone_type(size);
    if (zone_type == TINY_ZONE) {
        block_offset = TINY_ZONE_SIZE - g_malloc.zone.tiny->remaining_memory - total_size;
        block = (t_block *) ((char *) g_malloc.zone.tiny + sizeof(t_block) + block_offset);
        g_malloc.zone.tiny->remaining_memory -= total_size + sizeof(t_block);
    } else if (zone_type == SMALL_ZONE) {
        block_offset = SMALL_ZONE_SIZE - g_malloc.zone.small->remaining_memory - total_size;
        block = (t_block *) ((char *) g_malloc.zone.small + sizeof(t_block) + block_offset);
        g_malloc.zone.small->remaining_memory -= total_size + sizeof(t_block);
    }
    block->size = size;
    block->next = NULL;
    block->free = false;
    zone->next = block;
}

t_block *create_block_in_new_zone(size_t size, size_t zone_size, size_t total_size) {
    t_block *block =
        (t_block *) mmap(0, zone_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (block == MAP_FAILED) {
        return NULL;
    }
    block->remaining_memory = zone_size - total_size >= 0 ? zone_size - total_size : 0;
    block->size = size;
    block->next = NULL;
    block->free = false;
    return block;
}

/*
 * Request a new block of memory from the system
 * @param size The size of the block to request
 * @return A pointer to the new block, or NULL if the request failed
 */
t_block *request_space(size_t size) {
    size_t total_size = sizeof(t_block) + size;
    size_t zone_size = 0;
    t_block *block;
    t_block *zone;
    size_t block_offset;
    ZoneType zone_type;

    zone = NULL;
    zone_type = get_zone_type(size);
    if (zone_type == TINY_ZONE) {
        zone_size = TINY_ZONE_SIZE;
        zone = g_malloc.zone.tiny;
    } else if (zone_type == SMALL_ZONE) {
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
        block = create_block_in_existing_zone(zone, size, zone_size, total_size);
    } else {
        block = create_block_in_new_zone(size, zone_size, total_size);

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