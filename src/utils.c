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

void ft_write_hex_pointer(void *ptr) {
    char hex[18] = "0x00000000000000";   // Maximum 64-bit pointer size
    unsigned long value = (unsigned long) ptr;
    size_t digit;
    size_t i;

    i = 15;
    while (value != 0) {
        digit = value % 16;
        value /= 16;
        hex[i] = (digit < 10) ? ('0' + digit) : ('A' + digit - 10);
        i--;
    }

    i = 0;
    while (hex[i] != '\0') {
        write(1, &hex[i], 1);
        i++;
    }
}

void ft_putchar(char c) { write(1, &c, 1); }

void ft_putnbr(int nb) {
    long n;

    n = nb;
    (n < 0 && (n = -n) ? ft_putchar('-') : n);
    (n >= 10) ? ft_putnbr(n / 10) : n;
    ft_putchar(n % 10 + '0');
}

void ft_putdbl(double nb) {
    long wholePart = (long) nb;            // get the whole number part
    double decimalPart = nb - wholePart;   // get the decimal part
    ft_putnbr(wholePart);                  // print the whole number part using ft_putnbr
    if (decimalPart > 0) {
        ft_putchar('.');         // print the decimal point
        int decimalPlaces = 6;   // number of decimal places to print
        while (decimalPlaces-- > 0) {
            decimalPart *= 10;               // shift the decimal point to the right
            int digit = (int) decimalPart;   // get the next digit
            ft_putnbr(digit);                // print the digit using ft_putnbr
            decimalPart -= digit;            // remove the printed digit
        }
    }
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

void *ft_memcpy(void *dest, const void *src, size_t n) {
    char *dest_ptr = dest;
    const char *src_ptr = src;

    for (size_t i = 0; i < n; i++) {
        dest_ptr[i] = src_ptr[i];
    }

    return dest;
}

void *ft_memset(void *b, int c, size_t len) {
    size_t i;

    i = 0;
    while (i < len) {
        ((char *) b)[i] = (char) c;
        i++;
    }
    return (b);
}