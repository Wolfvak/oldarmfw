#pragma once

#include <common.h>
#include <asm.h>

static inline uint32_t
cpsr_get(void) {
	uint32_t cpsr;
	asm volatile("mrs %0, cpsr\n\t":"=r"(cpsr));
	return cpsr;
}

static inline void
cpsr_c_set(uint32_t cpsr) {
	asm volatile("msr cpsr_c, %0\n\t"::"r"(cpsr));
}

static inline uint32_t
spsr_get(void) {
	uint32_t spsr;
	asm volatile("mrs %0, spsr\n\t":"=r"(spsr));
	return spsr;
}

static inline void
spsr_set(uint32_t spsr) {
	asm volatile("msr spsr, %0\n\t"::"r"(spsr));
}

static inline uint32_t
int_disable(void) {
	uint32_t stat = cpsr_get();
	cpsr_c_set(stat | SR_NOINT);
	return stat & SR_NOINT;
}

static inline uint32_t
int_enable(void) {
	uint32_t stat = cpsr_get();
	cpsr_c_set(stat & ~SR_NOINT);
	return stat & SR_NOINT;
}

static inline void
int_restore(uint32_t stat) {
	cpsr_c_set((cpsr_get() & ~SR_NOINT) | stat);
}

static inline void
wfi(void) {
	asm volatile("mcr p15, 0, %0, c7, c0, 4"::"r"(0));
}
