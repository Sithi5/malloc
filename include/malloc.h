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

/****************************************************************************/
/*                          DEFINES                                         */
/****************************************************************************/

#define PROGRAM_NAME    "malloc"
#define PROGRAM_VERSION "1.0.0"
#define PROGRAM_AUTHOR  "mabouce"

#ifdef DEBUG
#define DEBUG 1
#else
#define DEBUG 0
#endif

#define PAGE_SIZE       (getpagesize())
#define TINY_MAX        64
#define SMALL_MAX       1024
#define TINY_ZONE_SIZE  (PAGE_SIZE * 2 + sizeof(t_block))    // 128 * 64 if page size is 4096
#define SMALL_ZONE_SIZE (PAGE_SIZE * 32 + sizeof(t_block))   // 128 * 1024 if page size is 4096

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
t_block *request_space(size_t size);
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