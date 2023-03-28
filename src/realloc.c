#include "malloc.h"

void *realloc(void *ptr, size_t size) {
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

    t_block *block = (t_block *) ptr - 1;   // Get the block structure from the pointer. The -1 is
                                            // because the pointer points to the data, not the
                                            // structure

    if (block->size >= size) {
        // If the current block is large enough, split it if necessary
        split_block(block, size);
    } else {
        // If the current block is too small, allocate a new block and copy the data
        void *new_ptr = malloc(size);
        if (new_ptr) {
            memcpy(new_ptr, ptr, block->size);
            free(ptr);
        }
        return new_ptr;
    }

    return ptr;
}
