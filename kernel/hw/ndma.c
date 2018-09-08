#include <common.h>
#include <hw/irq.h>
#include <hw/ndma.h>

#include <task/sched.h>

#define NDMA_CHANS	(8)

#define REG_NDMA_GLOBAL_CNT	(*(volatile uint32_t*)0x10002000)

#define REG_NDMA(n)	((((volatile uint32_t*)0x10002004) + ((n) * 7)))

#define REG_NDMA_SRC(n)			(REG_NDMA(n)[0])
#define REG_NDMA_DST(n)			(REG_NDMA(n)[1])
#define REG_NDMA_XFER_CNT(n)	(REG_NDMA(n)[2])
#define REG_NDMA_WRITE_CNT(n)	(REG_NDMA(n)[3])
#define REG_NDMA_BLOCK_CNT(n)	(REG_NDMA(n)[4])
#define REG_NDMA_FILL_DATA(n)	(REG_NDMA(n)[5])
#define REG_NDMA_CONTROL(n)		(REG_NDMA(n)[6])

void
ndma_interrupt(int irq)
{
	uint32_t chan = irq - INT_NDMA(0);
	//sched_wakeup(wakeup_tasks[chan]);
}

void
ndma_init(void)
{
	REG_NDMA_GLOBAL_CNT = BIT(31) | BIT(0);

	for (int i = 0; i < NDMA_CHANS; i++) {
		REG_NDMA_CONTROL(i) = 0;
		//wakeup_tasks[i] = NULL;
		//ndma_chan_stk[i] = i;

		irq_enable(INT_NDMA(i), ndma_interrupt);
	}
	//ndma_chan_free = 8;
}
