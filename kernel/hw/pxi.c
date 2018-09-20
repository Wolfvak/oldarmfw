#include <common.h>

#include <hw/irq.h>
#include <hw/pxi.h>

void
pxi_respond_sync(int irq)
{
	REG_PXI_SYNC_SEND = ~REG_PXI_SYNC_RECV;
}

void
pxi_init(void)
{
	REG_PXI_CNT = PXI_SEND_CLEAR | PXI_FIFO_ERR | PXI_FIFO_ENABLE;
	for (int i = 0; i < PXI_FIFO_WIDTH; i++)
		pxi_recv();

	irq_enable(INT_PXI_SYNC, pxi_respond_sync);

	REG_PXI_SYNC_SEND = 0;
	REG_PXI_SYNC_CNT = PXI_SYNC_IRQ;
	REG_PXI_CNT = PXI_RECV_IRQ | PXI_FIFO_ERR | PXI_FIFO_ENABLE;
}
