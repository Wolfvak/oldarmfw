#pragma once

#define INT_NDMA(n)		((n) + 0)
#define INT_TIMER(n)	((n) + 8)

#define INT_PXI_SYNC			(12)
#define INT_PXI_SEND_NOT_FULL	(13)
#define INT_PXI_RECV_NOT_EMPTY	(14)

#define INT_AES	(15)
#define INT_RSA	(22)

typedef void (*irq_cb)(int);

void irq_init(void);

void irq_enable(int irq, irq_cb cb);
void irq_disable(int irq);
