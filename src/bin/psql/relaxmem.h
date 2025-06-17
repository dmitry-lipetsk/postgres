#ifndef _RELAXMEM_H_
#define _RELAXMEM_H_

#include <stddef.h>

extern void* relaxmem__malloc(size_t sz);

extern void* relaxmem__strdup(const char* str);

extern void* relaxmem__pg_malloc(size_t sz);

#define relaxmem__pg_malloc_object(type) ((type *) relaxmem__pg_malloc(sizeof(type)))

extern void* relaxmem__pg_strdup(const char* str);

extern void relaxmem__cleanup(void);

#endif
