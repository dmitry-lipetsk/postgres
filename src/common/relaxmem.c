#include "common/relaxmem.h"
#ifdef FRONTEND
#include "postgres_fe.h"
#endif
#include "c.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

enum memkind_t
{
	memkind_STD = 0,
#ifdef FRONTEND
	memkind_PG = 1,
#endif
};

struct tag_memblock
{
	struct tag_memblock* next;
	enum memkind_t kind;
};

static size_t g_mem_block_count__STD = 0;
#ifdef FRONTEND
static size_t g_mem_block_count__PG = 0;
#endif

struct tag_memblock* g_mem_blocks = NULL;


static void*
reg_memblock_and_return_ptr(struct tag_memblock *memblock_ptr, size_t *counter_ptr)
{
	Assert(memblock_ptr != NULL);
	Assert(counter_ptr != NULL);
	
	/*
	* TODO: Use atomic setup (IntelockedCompareExchangePtr)
	*/
	memblock_ptr->next = g_mem_blocks;

	g_mem_blocks = memblock_ptr;

	/*
	* TODO: Use atomic increment (IntelockedIncrement)
	*/
	++(*counter_ptr);

	return (void*)(memblock_ptr + 1);
}


void* relaxmem__malloc(size_t sz)
{
	size_t const sz2 = sizeof(struct tag_memblock) + sz;

	void* const pv = malloc(sz2);

	if (!pv)
		return NULL;

	((struct tag_memblock*)pv)->kind = memkind_STD;

	return reg_memblock_and_return_ptr(
		((struct tag_memblock*)pv), &g_mem_block_count__STD);
}

void* relaxmem__strdup(const char* str)
{
	Assert(str != NULL);

	{
		size_t const sz = strlen(str) + 1;

		void* const pv = relaxmem__malloc(sz);

		if (!pv)
			return NULL;

		memcpy(pv, str, sz);

		return pv;
	}
}

#ifdef FRONTEND

void* relaxmem__pg_malloc(size_t sz)
{
	size_t const sz2 = sizeof(struct tag_memblock) + sz;

	void* const pv = pg_malloc(sz2);

	if (!pv)
		return NULL;

	((struct tag_memblock*)pv)->kind = memkind_PG;

	return reg_memblock_and_return_ptr(
		((struct tag_memblock*)pv), &g_mem_block_count__PG);
}

void* relaxmem__pg_strdup(const char* str)
{
	Assert(str != NULL);

	{
		size_t const sz = strlen(str) + 1;

		void* const pv = relaxmem__pg_malloc(sz);

		if (!pv)
			return NULL;

		memcpy(pv, str, sz);

		return pv;
	}
}

#endif

void relaxmem__cleanup(void)
{
	while (g_mem_blocks != NULL)
	{
		struct tag_memblock * const p = (struct tag_memblock*)(g_mem_blocks);

		g_mem_blocks = p->next;

		if (p->kind == memkind_STD)
		{
			Assert(g_mem_block_count__STD > 0);
			free(p);
			--g_mem_block_count__STD;
		}
#ifdef FRONTEND
		else
		if (p->kind == memkind_PG)
		{
			Assert(g_mem_block_count__PG > 0);
			pg_free(p);
			--g_mem_block_count__PG;
		}
#endif
		else
		{
			Assert(false);
		}
	}

	Assert(g_mem_block_count__STD == 0);
#ifdef FRONTEND
	Assert(g_mem_block_count__PG == 0);
#endif
}

