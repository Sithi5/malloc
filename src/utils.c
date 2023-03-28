#include "malloc.h"

/**
 * Find a free block of memory in the memory zone
 * @param last The last block in the memory zone
 * @param size The size of the block to find
 * @return A pointer to the free block, or NULL if no free block was found
 */
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

/*
 * Request a new block of memory from the system
 * @param last The last block in the memory zone
 * @param size The size of the block to request
 * @return A pointer to the new block, or NULL if the request failed
 */
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
        (t_block *) mmap(0, zone_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1,
                         0);   // Read and write, private, anonymous. fifth and sixth arguments are
                               // ignored because of MAP_ANONYMOUS
    if (block == MAP_FAILED) {
        return NULL;
    }

    // Initialize the block
    block->size = size;
    block->next = NULL;
    block->free = false;

    // Update the last block's next pointer
    if (last) {
        last->next = block;
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
        new_block->free = 1;

        // Update the original block's information
        block->size = size;
        block->next = new_block;
    }

    return block;
}

size_t ft_write_dec_number(size_t num) {
    size_t len;
    size_t digits;
    size_t tmp;
    char c;
    size_t i;

    len = 0;
    if (num == 0) {
        write(1, "0", 1);
        len = 1;
    } else {
        digits = 0;
        tmp = num;
        while (tmp > 0) {
            tmp /= 10;
            digits++;
        }
        i = 0;
        while (i++ < digits) {
            c = '0' + (num % 10);
            write(1, &c, 1);
            num /= 10;
        }
        len = digits;
    }
    return len;
}

size_t ft_write_hex_pointer(void *ptr) {
    char hex[16];
    unsigned long value;
    size_t digit;
    size_t len;
    size_t i;

    hex[0] = '0';
    hex[1] = 'x';
    value = (unsigned long) ptr;
    i = 14;
    while (--i >= 2) {
        digit = value % 16;
        value /= 16;
        hex[i] = (digit < 10) ? ('0' + digit) : ('A' + digit - 10);
    }
    len = 0;
    while (hex[len] != '\0') {
        write(1, &hex[len], 1);
        len++;
    }
    return len;
}