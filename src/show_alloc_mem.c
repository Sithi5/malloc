#include "malloc.h"

static size_t ft_write_dec_number(size_t num) {
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
        // Compute the number of digits in the number
        digits = 0;
        tmp = num;
        while (tmp > 0) {
            tmp /= 10;
            digits++;
        }
        i = 0;
        while (i < digits) {
            c = '0' + (num % 10);
            write(1, &c, 1);
            num /= 10;
            i++;
        }
        len = digits;
    }

    return len;
}

static size_t ft_write_hex_pointer(void *ptr) {
    char hex[16];
    unsigned long value;
    size_t digit;
    size_t len;

    hex[0] = '0';
    hex[1] = 'x';
    value = (unsigned long) ptr;
    for (size_t i = 13; i >= 2; i--) {
        digit = value % 16;
        value /= 16;
        hex[i] = (digit < 10) ? ('0' + digit) : ('a' + digit - 10);
    }
    len = 0;
    while (hex[len] != '\0') {
        write(1, &hex[len], 1);
        len++;
    }
    return len;
}

static size_t print_zone(t_block *zone) {
    size_t total = 0;
    t_block *block = zone;
    size_t len;

    while (block) {
        if (!block->free) {
            len = 0;
            len += write(1, "0x", 2);
            len += ft_write_hex_pointer((unsigned long) (block + 1));
            len += write(1, " - 0x", 5);
            len += ft_write_hex_pointer((unsigned long) ((char *) (block + 1) + block->size));
            len += write(1, " : ", 3);
            len += ft_write_dec_number(block->size);
            len += write(1, " bytes\n", 7);
            total += block->size;
        }
        block = block->next;
    }

    return total;
}

static size_t print_large_blocks(t_block *zone) {
    size_t total = 0;
    t_block *block = zone;
    int len;

    while (block) {
        if (!block->free) {
            len = 0;
            len += write(1, "0x", 2);
            len += ft_write_hex_pointer((unsigned long) (block + 1));
            len += write(1, " - 0x", 5);
            len += ft_write_hex_pointer((unsigned long) (block->next));
            len += write(1, " : ", 3);
            len += ft_write_dec_number(block->size);
            len += write(1, " bytes\n", 7);
            total += block->size;
        }
        block = block->next;
    }

    return total;
}

void show_alloc_mem() {
    size_t total = 0;
    int len;

    len = write(1, "TINY : 0x", 10);
    len += ft_write_hex_pointer((unsigned long) g_zone.tiny);
    len += write(1, "\n", 1);
    total += print_zone(g_zone.tiny);

    len = write(1, "SMALL : 0x", 11);
    len += ft_write_hex_pointer((unsigned long) g_zone.small);
    len += write(1, "\n", 1);
    total += print_zone(g_zone.small);

    len = write(1, "LARGE : 0x", 11);
    len += ft_write_hex_pointer((unsigned long) g_zone.large);
    len += write(1, "\n", 1);
    total += print_large_blocks(g_zone.large);

    len = write(1, "Total : ", 8);
    len += ft_write_dec_number(total);
    len += write(1, " bytes\n", 7);
}