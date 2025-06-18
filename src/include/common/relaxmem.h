#ifndef _RELAXMEM_H_
#define _RELAXMEM_H_

#include <stddef.h>

extern void* relaxmem__malloc(size_t sz);
extern void* relaxmem__strdup(const char* str);

extern void* relaxmem__pg_malloc(size_t sz);
extern void* relaxmem__pg_strdup(const char* str);
#define relaxmem__pg_malloc_object(type) ((type *) relaxmem__pg_malloc(sizeof(type)))

extern void* relaxmem__palloc(size_t sz);
extern void* relaxmem__pstrdup(const char* str);
#define relaxmem__palloc_array(type, count) ((type *) relaxmem__palloc(sizeof(type) * (count)))

extern void relaxmem__cleanup(void);

#endif
