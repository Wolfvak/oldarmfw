#include <common.h>
#include <arm.h>

#include <task/cpuctx.h>
#include <task/sched.h>

#include <lib/cq.h>

sched_task *sched_active;
int sched_taskn;

static cq_n *runq;

void load_usr_ctx(const cpuctx_t *ctx);

void
task_init(sched_task *t, uint32_t *sp, uint32_t pc, const char *name)
{
	cpuctx_init(&t->cpu_state, (uint32_t)sp, pc, SR_SYS);
	t->name = name;
	t->state = TASK_AWAKE;
	CQ_INIT(&t->node);
}

void
sched_init(sched_task *ktask)
{
	sched_taskn = 1;

	sched_active = ktask;
	runq = &ktask->node;
}

void
sched_add(sched_task *t)
{
	cq_add(&t->node, runq);
	sched_taskn++;
}

void
sched_del(sched_task *t)
{
	cq_remove(&t->node);
	sched_taskn--;
}

void
sched_run(void)
{
	int sleepcnt;

	CQ_ITER(task_node, runq) {
		if (task_node == runq)
			sleepcnt = 0;

		sched_active = CQ_DATA(task_node, sched_task, node);

		switch(sched_active->state) {
			case TASK_AWAKE:
				load_usr_ctx(&sched_active->cpu_state);
				break;

			case TASK_WAIT:
				sleepcnt++;
				break;

			default:
				/* break up mid-flight */
				breakup();
				break;
		}

		if (sleepcnt == sched_taskn) {
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
