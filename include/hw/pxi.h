#pragma once

#include <common.h>

#define PXI_FIFO_WIDTH	(16)

#define REG_PXI	((volatile uint32_t*)0x10008000)

#define REG_PXI_SYNC_RECV	(((volatile uint8_t*)REG_PXI)[0])
#define REG_PXI_SYNC_SEND	(((volatile uint8_t*)REG_PXI)[1])
#define REG_PXI_SYNC_CNT	(((volatile uint8_t*)REG_PXI)[3])

#define REG_PXI_CNT			(((volatile uint16_t*)REG_PXI)[2])

#define REG_PXI_SEND		(REG_PXI[2])
#define REG_PXI_RECV		(REG_PXI[3])

#define PXI_SYNC_IRQ	(BIT(7))
#define PXI_SYNC_EXEC	(BIT(5))

#define PXI_SEND_EMPTY	(BIT(0))
#define PXI_SEND_FULL	(BIT(1))
#define PXI_SEND_CLEAR	(BIT(3))
#define PXI_RECV_EMPTY	(BIT(8))
#define PXI_RECV_FULL	(BIT(9))
#define PXI_RECV_IRQ	(BIT(10))
#define PXI_FIFO_ERR	(BIT(14))
#define PXI_FIFO_ENABLE	(BIT(15))

static inline bool
pxi_send(uint32_t w) {
	if (REG_PXI_CNT & PXI_SEND_FULL)
		return false;
	REG_PXI_SEND = w;
	return true;
}

static inline uint32_t
pxi_recv(void) {
	return REG_PXI_RECV;
}

static inline int
pxi_recvbuf(uint32_t *w, int m)
{
	for (int i = 0; i < m; i++) {
		if (REG_PXI_CNT & PXI_RECV_EMPTY)
			return i;

		w[i] = pxi_recv();
	}
	return m;
}

static inline void
pxi_sendbuf(const uint32_t *w, int c)
{
	for (int i = 0; i < c; i++) {
		if (pxi_send(w[i]) == false)
			break;
	}
}

static inline bool
pxi_recv_full(void) {
	return (REG_PXI_CNT & PXI_RECV_FULL);
}

static inline bool
pxi_recv_empty(void) {
	return (REG_PXI_CNT & PXI_RECV_EMPTY);
}

static inline bool
pxi_send_full(void) {
	return (REG_PXI_CNT & PXI_SEND_FULL);
}

static inline bool
pxi_send_empty(void) {
	return (REG_PXI_CNT & PXI_SEND_EMPTY);
}

void pxi_init(void);
