#pragma once

#include <stddef.h>
#include <stdint.h>

#define MP_BUFSZ(_c, _s)	((((_c) * (_s) + 1) & ~1) + ((_c) * 2))

typedef struct mp {
	char *pool;
	uint16_t *stk;

	size_t psz;
	size_t max;
	size_t cnt;
	size_t tsz;
} mp;

size_t mp_init(mp *p, void *buf, size_t bufsz, size_t tsz);

void *mp_request(mp *p);
int mp_return(mp *p, void *addr);
