#ifndef PROTOBUF_C_ALLOC_H
#define PROTOBUF_C_ALLOC_H
#ifdef __cplusplus 
extern "C" { 
#endif
#include <stdlib.h>
#include <string.h>

char * _pbcM_malloc(size_t sz);
void _pbcM_free(void *p);
char * _pbcM_realloc(void *p, size_t sz);
void _pbcM_memory();

struct heap;
struct heap * _pbcH_new(int pagesize);
void _pbcH_delete(struct heap *);
void* _pbcH_alloc(struct heap *, int size);

#define HMALLOC(size) ((h) ? _pbcH_alloc(h, size) : _pbcM_malloc(size))

#define memory _pbcM_memory
#ifdef __cplusplus 
} 
#endif 
#endif
