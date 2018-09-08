#pragma once

#include <common.h>
#include <task/cpuctx.h>

#include <lib/cq.h>

#define TASK_ENTRY __attribute__((naked)) __attribute__((target("arm")))

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

void task_init(sched_task *t, uint32_t *sp, uint32_t pc, const char *name);

void sched_init(sched_task *ktask);
void sched_add(sched_task *t);
void sched_run(void);

sched_task *sched_active_task(void);
const char *sched_active_name(void);
int sched_taskcnt(void);

static inline void
sched_wakeup(sched_task *t) {
	t->state = TASK_AWAKE;
}

static inline void
sched_sleep(sched_task *t) {
	t->state = TASK_WAIT;
}
