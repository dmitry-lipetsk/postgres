#include "relaxmem.h"
#include "postgres_fe.h"

enum memkind_t
{
	memkind_STD = 0,
	memkind_PG = 1,
};

struct tag_memblock
{
	struct tag_memblock* next;
	enum memkind_t kind;
};

static size_t g_mem_block_count__STD = 0;
static size_t g_mem_block_count__PG = 0;

struct tag_memblock* g_mem_blocks = NULL;

void* relaxmem__malloc(size_t sz)
{
	size_t const sz2 = sizeof(struct tag_memblock) + sz;

	void* const pv = malloc(sz2);

	if (!pv)
		return NULL;

	((struct tag_memblock*)pv)->next = g_mem_blocks;
	((struct tag_memblock*)pv)->kind = memkind_STD;

	g_mem_blocks = ((struct tag_memblock*)pv);

	++g_mem_block_count__STD;

	return ((char*)pv) + sizeof(struct tag_memblock);
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

void* relaxmem__pg_malloc(size_t sz)
{
	size_t const sz2 = sizeof(struct tag_memblock) + sz;

	void* const pv = pg_malloc(sz2);

	if (!pv)
		return NULL;

	((struct tag_memblock*)pv)->next = g_mem_blocks;
	((struct tag_memblock*)pv)->kind = memkind_PG;

	g_mem_blocks = ((struct tag_memblock*)pv);

	++g_mem_block_count__PG;

	return ((char*)pv) + sizeof(struct tag_memblock);
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
		else
		if (p->kind == memkind_PG)
		{
			Assert(g_mem_block_count__PG > 0);
			pg_free(p);
			--g_mem_block_count__PG;
		}
		else
		{
			Assert(false);
		}
	}

	Assert(g_mem_block_count__STD == 0);
	Assert(g_mem_block_count__PG == 0);
}

