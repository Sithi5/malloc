#include "malloc.h"

static void show_zone_usage(t_block *block, size_t zone_size) {
    size_t used_size;
    size_t i;
    t_block *tmp;

    tmp = block;
    used_size = 0;
    while (tmp) {
        if (!tmp->free) {
            used_size += tmp->size;
            used_size += sizeof(t_block);
        }
        tmp = tmp->next;
    }

    ft_putstr("used_size = ");
    ft_putnbr(used_size);
    ft_putstr(" / zone_size = ");
    ft_putnbr(zone_size);
    ft_putstr("\n[");

    for (i = 0; i < 20; i++) {
        if (i < used_size * 20 / zone_size) {
            ft_putchar('#');
        } else {
            ft_putchar('-');
        }
    }
    ft_putstr("]\n");
}

void show_zone_memory_usage() {
    ft_putstr("TINY: ");
    show_zone_usage(g_malloc.zone.tiny, TINY_ZONE_SIZE);
    ft_putstr("SMALL: ");
    show_zone_usage(g_malloc.zone.small, SMALL_ZONE_SIZE);
    ft_putstr("\n");
}