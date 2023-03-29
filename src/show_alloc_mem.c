#include "malloc.h"

static size_t print_zone(t_block *zone) {
    size_t total = 0;
    t_block *block = zone;

    while (block) {
        if (!block->free) {
            ft_write_hex_pointer((block + 1));
            write(1, " - ", 5);
            ft_write_hex_pointer(((block + 1) + block->size));
            write(1, " : ", 3);
            ft_write_dec_number(block->size);
            write(1, " bytes\n", 7);
            total += block->size;
        }
        block = block->next;
    }

    return total;
}

static size_t print_large_blocks(t_block *zone) {
    size_t total = 0;
    t_block *block = zone;

    while (block) {
        if (!block->free) {
            ft_write_hex_pointer((block + 1));
            write(1, " - ", 5);
            ft_write_hex_pointer((block->next));
            write(1, " : ", 3);
            ft_write_dec_number(block->size);
            write(1, " bytes\n", 7);
            total += block->size;
        }
        block = block->next;
    }

    return total;
}

static size_t show_tiny_zone() {
    write(1, "TINY : ", 8);
    ft_write_hex_pointer(g_malloc.zone.tiny);
    write(1, "\n", 1);
    return print_zone(g_malloc.zone.tiny);
}

static size_t show_small_zone() {
    write(1, "SMALL : ", 9);
    ft_write_hex_pointer(g_malloc.zone.small);
    write(1, "\n", 1);
    return print_zone(g_malloc.zone.small);
}

static size_t show_large_zone() {
    write(1, "LARGE : ", 9);
    ft_write_hex_pointer(g_malloc.zone.large);
    write(1, "\n", 1);
    return print_large_blocks(g_malloc.zone.large);
}

void show_alloc_mem() {
    size_t total;

    total = 0;
    if (!g_malloc.zone.tiny && !g_malloc.zone.small && !g_malloc.zone.large) {
        write(1, "No allocated memory\n", 20);
        return;
    }
    if (g_malloc.zone.tiny)
        total += show_tiny_zone();
    if (g_malloc.zone.small)
        total += show_small_zone();
    if (g_malloc.zone.large) {
        total += show_large_zone();
    }
    write(1, "Total : ", 8);
    ft_write_dec_number(total);
    write(1, " bytes\n", 7);
}