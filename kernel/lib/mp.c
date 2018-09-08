#include <lib/mp.h>

size_t
mp_init(mp *p, void *buf, size_t bufsz, size_t tsz)
{
	size_t pool_items = bufsz / tsz;

	if (pool_items >= 0x10000)
		pool_items = 0xFFFF;

	while(pool_items > 0) {
		size_t stack_bytes, pool_bytes;

		stack_bytes = pool_items * 2;
		pool_bytes = (pool_items * tsz + 1) & ~1; /* align up to 2 bytes */

		if ((pool_bytes + stack_bytes) <= bufsz) {
			p->pool = buf;
			p->stk = (uint16_t*)buf + (pool_bytes / 2);

			p->psz = pool_bytes;
			p->max = pool_items;
			p->cnt = pool_items;
			p->tsz = tsz;

			for (size_t i = 0; i < pool_items; i++)
				p->stk[i] = i;

			break;
		}

		pool_items--;
	}

	return pool_items;
}

void *
mp_request(mp *p)
{
	size_t i;

	if (p->cnt == 0)
		return NULL;

	i = p->stk[--(p->cnt)];
	return &p->pool[i * p->tsz];
}

int
mp_return(mp *p, void *addr)
{
	ptrdiff_t off;

	if (p->cnt == p->max)
		return 0; /* already full */

	off = (ptrdiff_t)addr - (ptrdiff_t)p->pool;
	if ((off >= p->psz) || (off % p->tsz))
		return 0; /* trying to return an invalid address */

	p->stk[(p->cnt)++] = off / p->tsz;
	return 1;
}
