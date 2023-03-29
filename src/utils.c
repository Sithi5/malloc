#include "malloc.h"

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

void ft_putchar(char c) { write(1, &c, 1); }

void ft_putnbr(int nb) {
    long n;

    n = nb;
    (n < 0 && (n = -n) ? ft_putchar('-') : n);
    (n >= 10) ? ft_putnbr(n / 10) : n;
    ft_putchar(n % 10 + '0');
}

void ft_putstr(char const *str) {
    int i;

    i = 0;
    if (!str)
        return;
    while (str[i] != '\0') {
        ft_putchar(str[i]);
        i++;
    }
}