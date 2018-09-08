#include <common.h>
#include <arm.h>
#include <asm.h>

#include <task/sched.h>

void sched_yield(void);

static sched_task bgt;
static uint32_t bgt_stk[64];

void TASK_ENTRY
bgt_main(void)
{
	uint8_t *fb = (uint8_t*)0x18000000;
	int pxpos = 0;
	while(pxpos < 100*240*3) {
		fb[pxpos] = pxpos;
		pxpos++;
	}

	sched_yield();

	while(pxpos < 200*240*3) {
		fb[pxpos] = pxpos;
		pxpos++;
	}

	sched_sleep(&bgt);

	while(1) sched_yield();
}

void
kmain(void)
{
	task_init(&bgt, &bgt_stk[64], (uint32_t)&bgt_main, "bg");
	sched_init(&bgt);

	/* initialize all other tasks here */

	sched_run();
}
