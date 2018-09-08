#include <common.h>
#include <arm.h>

#include <hw/irq.h>

#define NR_IRQS	(29)
#define REG_IRQ	((volatile uint32_t*)0x10001000)

static irq_cb callbacks[NR_IRQS];

void
irq_init(void)
{
	REG_IRQ[0] = 0;
	REG_IRQ[1] = ~0;
	memset(callbacks, 0, sizeof(callbacks));
}

int
irq_enable(int irq, irq_cb cb)
{
	if (irq < 0 || irq >= NR_IRQS)
		return -1;

	REG_IRQ[1] = BIT(irq);
	REG_IRQ[0] |= BIT(irq);
	callbacks[irq] = cb;
	return 0;
}

int
irq_disable(int irq)
{
	if (irq < 0 || irq >= NR_IRQS)
		return -1;

	REG_IRQ[1] = BIT(irq);
	REG_IRQ[0] &= ~BIT(irq);
	callbacks[irq] = NULL;
	return 0;
}

static int
irq_acknowledge(int irq)
{
	REG_IRQ[1] = BIT(irq);
	return 0;
}

static int
irq_pending(void)
{
	int pend = REG_IRQ[1];
	if (pend == 0) return -1;
	return 31 - __builtin_clz(pend);
}

void __attribute__((interrupt("IRQ")))
_exception_irq(void)
{
	int pend;
	while((pend = irq_pending()) >= 0) {
		irq_acknowledge(pend);

		//int_enable(); NEED TO PRESERVE SPSR, TODO IN THE ASM VERSION
		(callbacks[pend])(pend);
		//int_disable();
	}
}
