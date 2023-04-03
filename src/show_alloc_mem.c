#include "malloc.h"

static size_t print_zone(t_block *zone) {
    size_t total = 0;
    t_block *block = zone;

    while (block) {
        if (!block->free) {
            ft_write_hex_pointer((block + sizeof(t_block)));
            ft_putstr(" - ");
            ft_write_hex_pointer(((block + sizeof(t_block)) + block->size));
            ft_putstr(" : ");
            ft_putnbr(block->size);
            ft_putstr(" bytes\n");
            total += block->size;
        }
        block = block->next;
    }

    return total;
}

static size_t show_tiny_zone() {
    ft_putstr("TINY : ");
    ft_write_hex_pointer(g_malloc.zone.tiny);
    ft_putstr("\n");
    return print_zone(g_malloc.zone.tiny);
}

static size_t show_small_zone() {
    ft_putstr("SMALL : ");
    ft_write_hex_pointer(g_malloc.zone.small);
    ft_putstr("\n");
    return print_zone(g_malloc.zone.small);
}

static size_t show_large_zone() {
    ft_putstr("LARGE : ");
    ft_write_hex_pointer(g_malloc.zone.large);
    ft_putstr("\n");
    return print_zone(g_malloc.zone.large);
}

void show_alloc_mem() {
    size_t total;

    pthread_mutex_lock(&g_mutex);
    total = 0;
    if (!g_malloc.zone.tiny && !g_malloc.zone.small && !g_malloc.zone.large) {
        ft_putstr("No allocated memory\n");
        return;
    }
    if (g_malloc.zone.tiny != NULL)
        total += show_tiny_zone();
    if (g_malloc.zone.small != NULL)
        total += show_small_zone();
    if (g_malloc.zone.large != NULL) {
        total += show_large_zone();
    }
    ft_putstr("Total : ");
    ft_putdbl(total);
    ft_putstr(" bytes\n");
    pthread_mutex_unlock(&g_mutex);
}