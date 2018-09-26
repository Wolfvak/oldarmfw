#pragma once

#include <common.h>
#include <task/cpuctx.h>

#include <lib/cq.h>

#define TASK_ENTRY __attribute__((naked)) __attribute__((noreturn))

enum {
	TASK_SLEEP,
	TASK_AWAKE,
};

typedef struct {
	cpuctx_t cpu_state;
	const char *name;
	int state;
	cq_n node;
} sched_task;

extern sched_task *sched_active;

typedef void (*task_pc)(sched_task*);

void task_init(sched_task *t, uint32_t *sp, task_pc pc, const char *name);

void sched_init(void);
void sched_run(void);

void sched_add(sched_task *t);
void sched_del(sched_task *t);

/*
 * MUST ONLY BE CALLED FROM TASKS
 *
 * Returns CPU control back to the supervisor
 */
static inline void
sched_yield(void) {
	asm volatile("svc 0\n\t":::"r0","r1","r2","r3","memory");
}

static inline const char *
sched_name(const sched_task *t) {
	return t->name;
}

static inline cpuctx_t *
sched_cpuctx(sched_task *t) {
	return &(t->cpu_state);
}
