#include "malloc.h"

void *realloc(void *ptr, size_t size) {
    ZoneType zone_type;
    t_block *block;

    if (ptr == NULL) {
        // If ptr is NULL, realloc should behave like malloc
        return malloc(size);
    }

    if (size == 0) {
        // If size is 0, realloc should behave like free
        free(ptr);
        return NULL;
    }

    // Align size to a multiple of 8 for better memory alignment
    size = (size + 7) - (size + 7) % 8;

    block = (t_block *) ptr - 1;

    zone_type = is_valid_block(block);
    // Check if the block is a valid memory block
    if (zone_type == INVALID_ZONE) {
        return NULL;
    }

    if (block->size >= size) {
        split_block(block, size);
    } else {
        void *new_ptr = malloc(size);
        if (new_ptr) {
            ft_memcpy(new_ptr, ptr, block->size);
            free(ptr);
        }
        return new_ptr;
    }
    return ptr;
}
