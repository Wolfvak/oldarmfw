#pragma once

#include <stddef.h>
#include <stdint.h>

typedef struct cq_n {
	struct cq_n *next;
	struct cq_n *prev;
} cq_n;

#define CQ_INIT(n)	do{(n)->next = (n)->prev = (n);} while(0)

#define CQ_NEXT(n)	((n)->next)
#define CQ_PREV(n)	((n)->prev)

#define CQ_DATA(n, t, m)	((t*) (&((char*)(n))[-offsetof(t, m)]))
#define CQ_ITER(_ite, c)	for(cq_n *_ite = c;; _ite = _ite->next)

static inline void
cq_add(cq_n *n, cq_n *h) {
	h->next->prev = n;
	n->next = h->next;
	n->prev = h;
	h->next = n;
}

static inline void
cq_remove(cq_n *n) {
	n->prev->next = n->next;
	n->next->prev = n->prev;
	n->next = n->prev = n;
}
