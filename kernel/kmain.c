#include <common.h>
#include <arm.h>
#include <asm.h>

#include <task/sched.h>

static sched_task bgm;
static uint32_t bgm_stk[32];

static inline void
setpixel(uint32_t x, uint32_t y, uint32_t c) {
	uint8_t *fb = (uint8_t*)0x18000000;
	fb += ((x * 240) + (239 - y)) * 3;

	*(fb++) = c;
	*(fb++) = c>>8;
	*(fb++) = c>>16;
}

void TASK_ENTRY
bgm_main(void)
{
	uint32_t ctr = 0;
	while(1) {
		for (int x = 0; x < 400; x++) {
			for (int y = 0; y < 240; y++) {
				setpixel(x, y, x * y * ctr);
			}
		}
		ctr += 0x10;
		sched_yield();
	}
}

void
kmain(void)
{
	task_init(&bgm, &bgm_stk[32], (uint32_t)&bgm_main, "bgmon");
	sched_init(&bgm);

	/* initialize all other tasks here */

	sched_run();
}
