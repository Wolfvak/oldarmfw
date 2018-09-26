#include <common.h>

#include <hw/irq.h>
#include <hw/pxi.h>

#include <pxicmd.h>

#include <task/sched.h>

void
ll_init(void)
{
	/* IRQ controller is always initialized first */
	irq_init();

	/* Initialize scheduler */
	sched_init();

	/* Initialize PXI hardware and comms protocol */
	pxi_init();
	pxicmd_init();

	/* Initialize all other hw here */
	// ndma_init();

	/* Initialize all other subsystems here */
	// mm_init();

	sched_run();
}
