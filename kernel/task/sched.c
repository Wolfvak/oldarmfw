#include <common.h>
#include <arm.h>

#include <task/cpuctx.h>
#include <task/sched.h>

#include <lib/cq.h>

static sched_task *activetsk;

static int taskcnt;
static cq_n *runq;

void load_sys_ctx(const cpuctx_t *ctx);

void
task_init(sched_task *t, uint32_t *sp, uint32_t pc, const char *name) {
	cpuctx_init(&t->cpu_state, (uint32_t)sp, pc, SR_SYS);
	t->name = name;
	t->state = TASK_AWAKE;
	CQ_INIT(&t->node);
}

void
sched_init(sched_task *ktask)
{
	taskcnt = 1;

	activetsk = ktask;
	runq = &ktask->node;
}

void
sched_add(sched_task *t)
{
	cq_add(&t->node, runq);
	taskcnt++;
}

void
sched_run(void)
{
	int sleepcnt;

	CQ_ITER(task_node, runq) {
		if (task_node == runq)
			sleepcnt = 0;

		activetsk = CQ_DATA(task_node, sched_task, node);

		switch(activetsk->state) {
			case TASK_AWAKE:
				load_sys_ctx(&activetsk->cpu_state);
				break;

			case TASK_WAIT:
				sleepcnt++;
				break;

			default:
				/* break up mid-flight */
				bkpt();
				break;
		}

		if (sleepcnt == taskcnt) {
			/*
			 * all tasks are sleeping
			 * wait for interrupt
			 */
			sleepcnt = 0;

			int_enable();
			/* <- TODO: FIX POTENTIAL RACE CONDITION */
			wfi();
			int_disable();
		}
	}
}

sched_task *
sched_active_task(void)
{
	return activetsk;
}

const char *
sched_active_name(void)
{
	return activetsk->name;
}

int
sched_taskcnt(void)
{
	return taskcnt;
}

/** FOR INTERNAL USE ONLY */
cpuctx_t *
sched_active_cpuctx(void)
{
	return &activetsk->cpu_state;
}
