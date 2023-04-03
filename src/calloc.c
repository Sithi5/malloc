#include "malloc.h"

void *calloc(size_t num_elements, size_t element_size) {
    void *ptr;

    ptr = malloc(num_elements * element_size);

    pthread_mutex_lock(&g_mutex);
    if (ptr != NULL) {
        ft_memset(ptr, 0, num_elements * element_size);
    }
    pthread_mutex_unlock(&g_mutex);

    return ptr;
}