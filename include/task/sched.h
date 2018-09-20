#pragma once

#include <common.h>
#include <task/cpuctx.h>

#include <lib/cq.h>

#define TASK_ENTRY __attribute__((naked)) __attribute__((noreturn))

enum {
	TASK_AWAKE,
	TASK_WAIT,
};

typedef struct {
	cpuctx_t cpu_state;
	const char *name;
	int state;
	cq_n node;
} sched_task;

extern sched_task *sched_active;
extern int sched_taskn;

void task_init(sched_task *t, uint32_t *sp, uint32_t pc, const char *name);

void sched_init(sched_task *ktask);
void sched_add(sched_task *t);
void sched_del(sched_task *t);
void sched_run(void);

/* ONLY USE FROM TASKS */
void sched_yield(void);

static inline void
sched_wakeup(sched_task *t) {
	t->state = TASK_AWAKE;
}

static inline void
sched_sleep(sched_task *t) {
	t->state = TASK_WAIT;
}

static inline const char *
sched_name(const sched_task *t) {
	return t->name;
}

static inline int
sched_state(const sched_task *t) {
	return t->state;
}

static inline cpuctx_t *
sched_cpuctx(sched_task *t) {
	return &(t->cpu_state);
}
