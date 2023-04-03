#include "malloc.h"

void *realloc(void *ptr, size_t size) {
    ZoneType zone_type;
    t_block *block;

    pthread_mutex_lock(&g_mutex);

    if (ptr == NULL) {
        // If ptr is NULL, realloc should behave like malloc
        pthread_mutex_unlock(&g_mutex);
        return malloc(size);
    }

    if (size == 0) {
        // If size is 0, realloc should behave like free
        pthread_mutex_unlock(&g_mutex);
        free(ptr);
        return NULL;
    }

    // Align size to a multiple of 8 for better memory alignment
    size = (size + 7) - (size + 7) % 8;

    block = (t_block *) ptr - 1;

    zone_type = is_valid_block(block);
    // Check if the block is a valid memory block
    if (zone_type == INVALID_ZONE) {
        pthread_mutex_unlock(&g_mutex);
        return NULL;
    }

    if (block->size >= size) {
        split_block(block, size);
    } else {
        pthread_mutex_unlock(&g_mutex);
        void *new_ptr = malloc(size);
        pthread_mutex_lock(&g_mutex);
        if (new_ptr) {
            ft_memcpy(new_ptr, ptr, block->size);
            pthread_mutex_unlock(&g_mutex);
            free(ptr);
        }
        pthread_mutex_unlock(&g_mutex);
        return new_ptr;
    }
    pthread_mutex_unlock(&g_mutex);
    return ptr;
}
