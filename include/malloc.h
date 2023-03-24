#ifndef MALLOC_H
#define MALLOC_H

/****************************************************************************/
/*                          INCLUDES                                        */
/****************************************************************************/

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <unistd.h>

#define TINY_MAX        64
#define SMALL_MAX       1024
#define PAGE_SIZE       (getpagesize())
#define TINY_ZONE_SIZE  (PAGE_SIZE * 4)
#define SMALL_ZONE_SIZE (PAGE_SIZE * 32)

/****************************************************************************/
/*                          STRUCTS                                         */
/****************************************************************************/

typedef struct s_block {
    size_t size;
    struct s_block *next;
    int free;
} t_block;

// Memory zone structure
typedef struct s_zone {
    t_block *tiny;
    t_block *small;
    t_block *large;
} t_zone;

/****************************************************************************/
/*                          GLOBALS                                         */
/****************************************************************************/

extern t_zone g_zone;

/****************************************************************************/
/*                          FUNCTIONS DEFINITIONS                           */
/****************************************************************************/

// utils
t_block *find_free_block(t_block **last, size_t size);
t_block *request_space(t_block *last, size_t size);
t_block *split_block(t_block *block, size_t size);

// free
void free(void *ptr);

// malloc
void *malloc(size_t size);

// realloc
void *realloc(void *ptr, size_t size);

// calloc
void show_alloc_mem();

#endif