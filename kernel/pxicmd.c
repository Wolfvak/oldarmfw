#include <common.h>

#include <arm.h>
#include <pxicmd.h>

#include <hw/irq.h>
#include <hw/pxi.h>

static void
pxicmd_process_cmd(int irq)
{
	return;
}

void
pxicmd_init(void)
{
	irq_enable(INT_PXI_RECV_NOT_EMPTY, pxicmd_process_cmd);
}
