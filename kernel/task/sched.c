#include <common.h>
#include <arm.h>

#include <task/cpuctx.h>
#include <task/sched.h>

#include <lib/cq.h>

static sched_task deftsk;
static uint32_t deftsk_stk[32];

static void TASK_ENTRY
task_default(sched_task *thistask)
{
	uint8_t ctr = 0x40, *fb = (uint8_t*)0x18000000;
	while(1) {
		for (int i = 0; i < 0x46500; i++) {
			fb[i] = ctr;
			sched_yield();
		}
		ctr++;
	}
}


sched_task *sched_active;

static int sched_taskn;
static cq_n *sched_lastnode;

void load_usr_ctx(const cpuctx_t *ctx);

void
task_init(sched_task *t, uint32_t *sp, task_pc pc, const char *name)
{
	cpuctx_init(&t->cpu_state, t, sp, pc, SR_SYS);
	t->name = name;
	t->state = TASK_AWAKE;
	CQ_INIT(&t->node);
}

void
sched_init(void)
{
	task_init(&deftsk, &deftsk_stk[32], task_default, "bg");

	sched_taskn = 1;
	sched_active = NULL;
	sched_lastnode = &deftsk.node;
}

void
sched_add(sched_task *t)
{
	cq_add(&t->node, sched_lastnode);
	sched_lastnode = &t->node;
	sched_taskn++;
}

void
sched_del(sched_task *t)
{
	sched_lastnode = CQ_PREV(&t->node);
	cq_remove(&t->node);
	sched_taskn--;
}

void
sched_run(void)
{
	cq_n *first;
	int sleepcnt = 0;

	first = &deftsk.node;
	CQ_ITER(task_node, first) {
		sched_task *t = CQ_DATA(task_node, sched_task, node);

		switch(t->state) {
			case TASK_AWAKE:
				sched_active = t;
				load_usr_ctx(&t->cpu_state);
				sched_active = NULL;
				break;

			case TASK_SLEEP:
				sleepcnt++;
				break;

			default:
				breakup();
		}

		if (task_node == sched_lastnode) {
			if (sleepcnt == sched_taskn)
				wfi();
			sleepcnt = 0;
		}
	}
}
