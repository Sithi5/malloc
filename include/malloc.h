#ifndef MALLOC_H
#define MALLOC_H

/****************************************************************************/
/*                          INCLUDES                                        */
/****************************************************************************/

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <unistd.h>

#define TINY_MAX        64
#define SMALL_MAX       1024   // 1 KB
#define PAGE_SIZE       (getpagesize())
#define TINY_ZONE_SIZE  (PAGE_SIZE * 2)    // ( 4096 * 2) / 64 = 128
#define SMALL_ZONE_SIZE (PAGE_SIZE * 32)   // ( 4096 * 32) / 1024 = 128. Total size  = 131072

/****************************************************************************/
/*                          STRUCTS                                         */
/****************************************************************************/

typedef struct s_block {
    size_t size;
    size_t remaining_memory;
    struct s_block *next;
    bool free;
} t_block;

typedef struct s_zone {
    t_block *tiny;
    t_block *small;
    t_block *large;
} t_zone;

typedef struct s_malloc {
    t_zone zone;
    int number_of_requests;
} t_malloc;

/****************************************************************************/
/*                          GLOBALS                                         */
/****************************************************************************/

extern t_malloc g_malloc;

/****************************************************************************/
/*                          FUNCTIONS DEFINITIONS                           */
/****************************************************************************/

// blocks_utils
t_block *find_free_block(t_block **last, size_t size);
t_block *request_space(t_block *last, size_t size);
t_block *split_block(t_block *block, size_t size);

// utils
size_t ft_write_dec_number(size_t num);
size_t ft_write_hex_pointer(void *ptr);
void ft_putchar(char c);
void ft_putnbr(int nb);
void ft_putstr(char const *str);

// free
void free(void *ptr);

// malloc
void *malloc(size_t size);

// realloc
void *realloc(void *ptr, size_t size);

// calloc
void show_alloc_mem();

#endif