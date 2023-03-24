#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <unistd.h>

#define ALIGNMENT 16
#define PAGE_SIZE (getpagesize())

void free(void *ptr);
void *malloc(size_t size);
void *realloc(void *ptr, size_t size);